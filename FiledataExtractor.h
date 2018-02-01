#ifndef FILEDATA_EXTRACTOR_H
#define FILEDATA_EXTRACTOR_H

#include <string>

class FiledataExtractor {
    public:
        FiledataExtractor();
        std::string getACInfo();
        std::string getFilename();
        std::string getFiledata();

        bool loadFile(std::string);

    private:
        bool readACInfo();
        bool readData();

        std::string ac_data;
        std::string filename;
        std::string filedata;
};
#endif
