#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <iostream>
#include <fstream>

class Archive {
    public:
        bool write();
        bool extract();

    private:
        // Archive Creation Helpers
        void processSubdir(std::string);

        // Archive Extraction Helpers
        void processRegexStream(/*RStream dtype*/);
        std::string readUntilEOE(std::ifstream&);


};

#endif
