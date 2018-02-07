#include "FiledataExtractor.h"
#include "IOFunctions.h"
#include "Tags.h"
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <queue>

typedef std::string::size_type str_sz;

void readHandler(std::queue<std::string> &file_queue,std::string wfile) {
    FiledataExtractor extractor = FiledataExtractor();
    std::queue<std::string> write_queue;
    std::thread write_thread(writeHandler,std::ref(write_queue),wfile);
    bool active = true;

    while(active) {
        if (!file_queue.empty()) {
          // Grab info from queue
            char ftype = file_queue.front().at(0); // get file type
            std::string f_entry = file_queue.front();
            str_sz seperator = f_entry.find_last_of(PATH_SEPERATOR);
            std::string path = f_entry.substr(1,seperator);
            std::string fname = f_entry.substr(seperator + 1); // get file name
            std::cout << path << " " << fname << std::endl;
            extractor.loadFile(path,fname);
            std::string write_entry;
            file_queue.pop();

            // Prepare entry for write
            write_entry = HEADER_START + extractor.getFilename() +
                        SEPERATOR + extractor.getOwnershipInfo() +
                        SEPERATOR + extractor.getPermInfo() +
                        ftype + HEADER_END;


            // Handle file data writes
            if (ftype == FILE_END) {
              std::string fdata = extractor.getFiledata();

              // Block out data into 64kb blocks
              while (fdata.size() > BLK_SIZE) {
                  write_entry += fdata.substr(0,BLK_SIZE);
                  write_queue.push(write_entry);
                  write_entry = "";
                  fdata = fdata.substr(BLK_SIZE);
              }

              // Write leftover data to queue
              write_queue.push(fdata);

            }
            else {
              write_queue.push(write_entry);
            }


            // Check if queue processing still active
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
      writer << write_queue.front();
      write_queue.pop();

      if (write_queue.front() == END_QUEUE)
        active = false;
    }
  }

  writer.close();

}
