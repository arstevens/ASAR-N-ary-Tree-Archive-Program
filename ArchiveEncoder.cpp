#include <queue>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <functional>
#include "Tags.h"
#include "ArchiveEncoder.h"
#include "Archive.h"
#include "FiledataExtractor.h"
#include "IOFunctions.h"

// Constructors
ArchiveEncoder::ArchiveEncoder(std::string root_path,std::string archive_file) :
															Archive(root_path),archive_fn(archive_file) { }


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
				std::string fname = dentry->d_name;

        if (is_dir) {
						file_queue.push(DIR_END + fname);
            dir_queue.push(path + PATH_SEPERATOR + dentry->d_name);
        }
        else {
						file_queue.push(FILE_END + fname);
        }
    }

		file_queue.push(EOE);
    return true;

}

bool ArchiveEncoder::validEntry(std::string dir) {
  return (dir != CURRENT_DIR && dir != PREV_DIR);
}


// Archive Creation Method
bool ArchiveEncoder::write() {
    // Initial Load
		std::thread read_thread(readHandler,std::ref(file_queue),archive_fn);
    processSubdir(root);

    // Subdirectory Processing
    while (!dir_queue.empty()) {
        std::string dir = dir_queue.front();
        std::cout << dir << std::endl;
        dir_queue.pop();
        processSubdir(dir);
    }

		// Tell the read thread to finish processing
		file_queue.push(END_QUEUE);
		read_thread.join();

    return true;
}
