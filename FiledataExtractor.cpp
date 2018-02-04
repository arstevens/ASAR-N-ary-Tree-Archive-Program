#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "FiledataExtractor.h"
#include "Tags.h"

// Helper Methods
bool FiledataExtractor::readACInfo() {
    struct stat file_stat;
    if (stat((path_prefix+"/"+filename).c_str(),&file_stat) == -1)
        return false;

    int user_permission=0;
    int group_permission=0;
    int other_permission=0;
    int special_permission=0;

    // Add UID & GID Information
    ac_data = std::to_string(file_stat.st_uid) + ":" +
            std::to_string(file_stat.st_gid) + ":";
    // Calculate owner permission value
    user_permission += (file_stat.st_mode & S_IRUSR) ? 4 : 0;
    user_permission += (file_stat.st_mode & S_IWUSR) ? 2 : 0;
    user_permission += (file_stat.st_mode & S_IXUSR) ? 1 : 0;

    // Calculate group permission value
    group_permission += (file_stat.st_mode & S_IRGRP) ? 4 : 0;
    group_permission += (file_stat.st_mode & S_IWGRP) ? 2 : 0;
    group_permission += (file_stat.st_mode & S_IXGRP) ? 1 : 0;

    // Calculate other permission value
    other_permission += (file_stat.st_mode & S_IROTH) ? 4 : 0;
    other_permission += (file_stat.st_mode & S_IWOTH) ? 2 : 0;
    other_permission += (file_stat.st_mode & S_IXOTH) ? 1 : 0;

    // Calculate special permission value
    special_permission += (file_stat.st_mode & S_ISUID) ? 4 : 0;
    special_permission += (file_stat.st_mode & S_ISGID) ? 2 : 0;

    // Construct octal string
    ac_data += (char)('0' + special_permission);
    ac_data += (char)('0' + user_permission);
    ac_data += (char)('0' + group_permission);
    ac_data += (char)('0' + other_permission);

    return true;
}

bool FiledataExtractor::readData() {
    std::ifstream reader;
    reader.open(path_prefix+"/"+filename,std::ifstream::in);

    // Check if failbit is set
    if (reader.fail())
        return false;

    // Read in data from file
    char current_char = reader.get();
    while (reader.good()) {
        filedata += current_char;
        current_char = reader.get();
    }
    return true;
}

// Getter Methods
std::string FiledataExtractor::getACInfo() {
    return AC_START + ac_data + AC_END;
}

std::string FiledataExtractor::getFilename() {
    return FNAME_START + filename + FNAME_END;
}

std::string FiledataExtractor::getFiledata() {
    return FDATA_START + filedata + FDATA_END;
}

// Mutator Methods
bool FiledataExtractor::loadFile(std::string prefix,std::string fname) {
    path_prefix = prefix;
    filename = fname;
    return readACInfo() && readData();
}
