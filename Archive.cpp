#include <queue>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include "Tags.h"
#include "Archive.h"
#include "FiledataExtractor.h"

// Constructors
Archive::Archive(std::string root_path,std::string wfile) : root(root_path) {
    fwriter.open(wfile,std::ofstream::out);
}

Archive::Archive(std::string archive_file) : root(archive_file) { }

// Archive Creation Helpers
bool Archive::processSubdir(std::string path) {
    // Create Directory Stream
    DIR *dstream = opendir(path.c_str());
    if (dstream == NULL)
        return false;

    struct dirent *dentry = NULL;

    // Process Files
    while ((dentry = readdir(dstream)) != NULL) {
        // Make sure the entry is Valid
        if (!validEntry(dentry->d_name))
            continue;

        bool is_dir = false;
        extractor.loadFile(root,dentry->d_name);

        // Start writing entry header
        fwriter << HEADER_START << extractor.getFilename() << SEPERATOR;
        fwriter << extractor.getOwnershipInfo() << SEPERATOR;
        fwriter << extractor.getPermInfo() << SEPERATOR;

        // Handle if file type is in directory or need Inode seek
        #ifndef _DIRENT_HAVE_D_TYPE
        if (dentry->d_type == DT_DIR)
            is_dir = true;

        #else
        struct stat f_stat_info;
        stat((path + PATH_SEPERATOR + dentry->d_name).c_str(),&f_stat_info);

        if (S_ISDIR(f_stat_info.st_mode) != 0)
            is_dir = true;

        #endif

        // Handle difference between Directory & File
        if (is_dir) {
            fwriter << DIR_END << HEADER_END;
            dir_queue.push(path + PATH_SEPERATOR + dentry->d_name);
        }
        else {
            fwriter << FILE_END << HEADER_END;
            fwriter << extractor.getFiledata();
        }
    }

    fwriter << EOE;

    return true;

}

bool Archive::validEntry(std::string dir) {
  return (dir != CURRENT_DIR && dir != PREV_DIR);
}

// Archive Extraction Helpers
void Archive::processRegexStream() {

}

std::string Archive::readUntilEOE(std::ifstream &stream) {
    std::string buffer;

    // Read in enough data to satisfy EOE read loop
    for (int i = 0; i < 6; i++) {
        if (stream.fail())
                return buffer;
        buffer += stream.get();
    }

    // Read in until reach EOE or read fails
    int substr_location = buffer.size() - EOE.size();
    while (buffer.substr(substr_location) != EOE && !stream.fail()) {
        buffer += stream.get();
        int substr_location = buffer.size() - EOE.size();
    }

    return buffer;

}

// Archive Creation Method
bool Archive::write() {
    // Initial Load
    processSubdir(root);

    // Subdirectory Processing
    while (!dir_queue.empty()) {
        std::string dir = dir_queue.front();
        std::cout << dir << std::endl;
        dir_queue.pop();
        processSubdir(dir);
    }

    return true;
}
