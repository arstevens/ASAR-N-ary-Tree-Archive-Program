#include <queue>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include "Tags.h"
#include "ArchiveEncoder.h"
#include "Archive.h"
#include "FiledataExtractor.h"

// Constructors
ArchiveEncoder::ArchiveEncoder(std::string root_path,std::string archive_file) :
															Archive(root_path) {
    fwriter.open(archive_file,std::ofstream::out);
}


// Archive Creation Helpers
bool ArchiveEncoder::processSubdir(std::string path) {
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
        extractor.loadFile(path,dentry->d_name);

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

bool ArchiveEncoder::validEntry(std::string dir) {
  return (dir != CURRENT_DIR && dir != PREV_DIR);
}


// Archive Creation Method
bool ArchiveEncoder::write() {
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
