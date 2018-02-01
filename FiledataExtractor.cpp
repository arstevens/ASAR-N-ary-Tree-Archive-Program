#include <sys/stat.h>
#include <fstream>
#include "FiledataExtractor.h"

// Helper Methods
bool FiledataExtractor::readACInfo() {
    struct stat file_stat;
   if (stat(filename.c_str(),&file_stat) == -1)
       return false;

    int user_permission=0;
    int group_permission=0;
    int other_permission=0;
    int special_permission=0;

    ac_data = std::to_string(file_stat.st_uid) + ":" +
            std::to_string(file_stat.st_gid) + ":";
    // calculate owner permission value
    user_permission += (file_stat.st_mode & S_IRUSR) ? 4 : 0;
    user_permission += (file_stat.st_mode & S_IWUSR) ? 2 : 0;
    user_permission += (file_stat.st_mode & S_IXUSR) ? 1 : 0;

    // calculate group permission value
    group_permission += (file_stat.st_mode & S_IRGRP) ? 4 : 0;
    group_permission += (file_stat.st_mode & S_IWGRP) ? 2 : 0;
    group_permission += (file_stat.st_mode & S_IXGRP) ? 1 : 0;

    // calculate other permission value
    other_permission += (file_stat.st_mode & S_IROTH) ? 4 : 0;
    other_permission += (file_stat.st_mode & S_IWOTH) ? 2 : 0;
    other_permission += (file_stat.st_mode & S_IXOTH) ? 1 : 0;

    // calculate special permission value
    special_permission += (file_stat.st_mode & S_ISUID) ? 4 : 0;
    special_permission += (file_stat.st_mode & S_ISGID) ? 2 : 0;

    // construct octal string
    ac_data += (char)('0' + special_permission);
    ac_data += (char)('0' + user_permission);
    ac_data += (char)('0' + group_permission);
    ac_data += (char)('0' + other_permission);

    return true;
}

bool FiledataExtractor::readData() {
    std::ifstream reader;
    reader.open(filename,std::ifstream::in);

    // check if failbit is set
    if (reader.fail())
        return false;

    // read in data from file
    char current_char = reader.get();
    while (reader.good()) {
        filedata += current_char;
        current_char = reader.get();
    }

    return true;
}

// Getter Methods
std::string FiledataExtractor::getACInfo() {
    return ac_data;
}

std::string FiledataExtractor::getFilename() {
    return filename;
}

std::string FiledataExtractor::getFiledata() {
    return filedata;
}

// Mutator Methods
bool FiledataExtractor::loadFile(std::string path) {
    filename = path;
    return this->readACInfo() && this->readData();
}
