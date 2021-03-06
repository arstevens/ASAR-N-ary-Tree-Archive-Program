#include "FiledataExtractor.h"
#include "IOFunctions.h"
#include "Tags.h"
#include <unistd.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <queue>

typedef std::string::size_type str_sz;

void readHandler(std::queue<std::string> &file_queue,std::string wfile,std::string path_range) {
    FiledataExtractor extractor = FiledataExtractor();
    std::queue<std::string> write_queue;
    std::thread write_thread(writeHandler,std::ref(write_queue),wfile);
    bool active = true;

    while(active) {
        if (!file_queue.empty()) {
          // Grab info from queue
            char ftype = file_queue.front().at(0); // get file type
            std::string f_entry = file_queue.front();
            extractor.loadFile(f_entry.substr(1));
            std::string write_entry;
            file_queue.pop();

            // Prepare entry for write
            write_entry = HEADER_START + extractor.getFilename() +
                        SEPERATOR + extractor.getOwnershipInfo() +
                        SEPERATOR + extractor.getPermInfo() +
                        SEPERATOR + ftype + HEADER_END;

            write_queue.push(write_entry);

            // Handle file data writes
            if (ftype == FILE_END) {
              std::ifstream fdata = extractor.getFiledata();
              char *buff = new char[BLK_SIZE+1];

              // Find length of fdata
              fdata.seekg(0,fdata.end);
              size_t fd_length = fdata.tellg();
              fdata.seekg(0,fdata.beg);

              // Block out data into 64kb blocks
              while (fd_length > BLK_SIZE) {
                  // read in data
                  fdata.read(buff,BLK_SIZE);
                  buff[BLK_SIZE] = '\0';
                  write_entry += buff;

                  // push data to write queue
                  write_queue.push(write_entry);
                  write_entry = "";

                  fd_length = fd_length - BLK_SIZE;
              }

              // Write leftover data to queue
              fdata.read(buff,fd_length);
              buff[fd_length] = '\0';
              write_entry.assign(buff,fd_length);

              fdata.close();

            }
            else if (ftype == SYM_END)
              write_entry += getLinkPath(f_entry.substr(1),path_range);


            // Write leftover data if applicable
            if (write_entry.size() != 0)
              write_queue.push(write_entry);

            // Check if queue processing still active
            if (file_queue.front() == EOE) {
              write_queue.push(EOE);
              file_queue.pop();
            }
            if (file_queue.front() == END_QUEUE) {
                //  Make sure write thread knows to end
                write_queue.push(END_QUEUE);
                active = false;
            }
        }

    }

    // Wait for write thread to finish
    write_thread.join();
}

void writeHandler(std::queue<std::string> &write_queue,std::string wfile) {
  bool active = true;
  std::ofstream writer = std::ofstream(wfile,std::ofstream::out);

  while (active) {
    if (!write_queue.empty()) {
      if (write_queue.front() == END_QUEUE) {
        active = false;
      }

      writer << write_queue.front();
      write_queue.pop();
    }
  }

  writer.close();

}

// Helper Function to find where a link points to
std::string getLinkPath(std::string link,std::string path_range) {
  char buff[PATH_BUFF];
  ssize_t bufflen = readlink(link.c_str(),buff,sizeof(buff)-1);
  std::string path;

  if (bufflen != -1) {
    // set to null char so string convertion goes approp.
    buff[bufflen] = '\0';
    path = buff;
  }

  str_sz in_range = path.find(path_range);
  if (in_range != std::string::npos)
    path = path.substr(in_range);

  return path;
}
