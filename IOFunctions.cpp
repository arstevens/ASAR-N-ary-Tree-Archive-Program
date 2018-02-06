#include "FiledataExtractor.h"
#include "IOFunctions.h"
#include "Tags.h"
#include <queue>

typedef std::string::size_type str_sz;

void readHandler(std::queue<std::string> &file_queue) {
    FiledataExtractor extractor = FiledataExtractor();
    bool active = true;

    while(active) {
        if (!file_queue.empty()) {
            // Grab info from queue
            char ftype = file_queue.front().at(0); // get file type
            std::string f_entry = file_queue.front();
            str_sz seperator = f_entry.find_last_of(PATH_SEPERATOR);
            std::string path = f_entry.substr(1,seperator);
            std::string fname = f_entry.substr(seperator + 1); // get file name
            extractor.loadFile(path,fname);
            file_queue.pop();
            std::string write_entry;

            // Prepare entry for write
            write_entry = HEADER_START + extractor.getFilename() +
                        SEPERATOR + extractor.getOwnershipInfo() +
                        SEPERATOR + extractor.getPermInfo() +
                        ftype + HEADER_END;


            if (ftype == FILE_END)


            // Check if queue processing still active
            if (file_queue.front() == END_QUEUE)
                active = false;
        }

    }
}
