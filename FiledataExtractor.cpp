#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "FiledataExtractor.h"
#include "Tags.h"

// Helper Methods
bool FiledataExtractor::readACInfo() {
    struct stat file_stat;
    if (stat((path_prefix+PATH_SEPERATOR+filename).c_str(),&file_stat) == -1)
        return false;

    int user_permission=0;
    int group_permission=0;
    int other_permission=0;
    int special_permission=0;

    // Add UID & GID Information
    ownership_data = std::to_string(file_stat.st_uid) + SEPERATOR +
                    std::to_string(file_stat.st_gid);

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
    perm_data = (char)('0' + special_permission);
    perm_data += (char)('0' + user_permission);
    perm_data += (char)('0' + group_permission);
    perm_data += (char)('0' + other_permission);

    return true;
}

bool FiledataExtractor::readData() {
    std::ifstream reader;
    reader.open(path_prefix+PATH_SEPERATOR+filename,std::ifstream::in);

    // Check if failbit is set
    if (reader.fail())
        return false;

    // Read in data from file
    char current_char = reader.get();
    while (reader.good()) {
        filedata += current_char;
        current_char = reader.get();
    }

    reader.close();
    return true;
}

// Getter Methods
std::string FiledataExtractor::getOwnershipInfo() {
    return ownership_data;
}

std::string FiledataExtractor::getPermInfo() {
  return perm_data;
}

std::string FiledataExtractor::getFilename() {
    return filename;
}

std::string FiledataExtractor::getFiledata() {
  readData();
  return filedata;
}

// Mutator Methods
bool FiledataExtractor::loadFile(std::string path) {
  std::string::size_type seperator = path.find_last_of('/');
    path_prefix = path.substr(0,seperator);
    filename = path.substr(seperator+1);
    filedata = "";
    return readACInfo();
}
