#ifndef FILEDATA_EXTRACTOR_H
#define FILEDATA_EXTRACTOR_H

#include <string>
#include <fstream>

class FiledataExtractor {
    public:
        std::string getFilename();
        std::ifstream getFiledata();
        std::string getOwnershipInfo();
        std::string getPermInfo();

        bool loadFile(std::string);

    private:
        bool readACInfo();

        std::string ownership_data;
        std::string perm_data;
        std::string path_prefix;
        std::string filename;
};
#endif
