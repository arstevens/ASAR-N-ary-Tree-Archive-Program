#include <filesystem>
#include <queue>
#include "Archive.h"
#include "FiledataExtractor.h"

namespace fs = std::filesystem;

// Constructors
Archive::Archive(std::string root_path) : root(root_path) { }

// Archive Creation Helpers
void Archive::processSubdir(std::string path) {

    // Process Files
    for (auto &entry : fs::directory_iterator(path)) {
        extractor.loadFile(entry.path().string());
        fwriter << extractor.getFilename() << extractor.getACInfo();

        // Handle Differences between Dirs & Files
        if (entry.is_directory()) {
            dir_queue.push(entry.path().string());
            fwriter << Tags::DIR_END;
        }
        else {
            fwriter << extractor.getFiledata();
            fwriter << Tags::FILE_END;
        }
    }

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
    int substr_location = buffer.size() - Tags::EOE.size();
    while (buffer.substr(substr_location) != Tags::EOE && !stream.fail()) {
        buffer += stream.get();
        int substr_location = buffer.size() - Tags::EOE.size();
    }

    return buffer;

}

// Archive Creation Method
bool Archive::write() {
    // Initial Load
    processSubdir(root);

    // Subdirectory Processing
    while (!dir_queue.empty()) {
        std::string dir = dir_queue.pop();
        processSubdir(dir);
    }

    return true;
}
