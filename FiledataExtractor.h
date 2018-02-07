#ifndef FILEDATA_EXTRACTOR_H
#define FILEDATA_EXTRACTOR_H

#include <string>

class FiledataExtractor {
    public:
        std::string getFilename();
        std::string getFiledata();
        std::string getOwnershipInfo();
        std::string getPermInfo();

        bool loadFile(std::string,std::string);

    private:
        bool readACInfo();
        bool readData();

        std::string ownership_data;
        std::string perm_data;
        std::string path_prefix;
        std::string filename;
        std::string filedata;
};
#endif
