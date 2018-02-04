#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string>
#include <fstream>
#include <queue>
#include "FiledataExtractor.h"

class Archive {
    public:
        Archive(std::string,std::string);
        Archive(std::string);
        bool write();
        bool extract();

    private:
        // Fields
        std::string root;
        std::ofstream fwriter;
        std::queue<std::string> dir_queue;
        FiledataExtractor extractor;


        // Archive Creation Helpers
        bool processSubdir(std::string);
        bool validEntry(std::string);

        // Archive Extraction Helpers
        void processRegexStream(/*RStream dtype*/);
        std::string readUntilEOE(std::ifstream&);


};

#endif
