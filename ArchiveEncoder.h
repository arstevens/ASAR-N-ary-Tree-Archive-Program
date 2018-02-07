#ifndef ARCHIVE_ENCODER_H
#define ARCHIVE_ENCODER_H

#include <string>
#include "FiledataExtractor.h"
#include "Archive.h"

class ArchiveEncoder : public Archive {
  public:
    ArchiveEncoder(std::string,std::string);
    bool write();

  private:
    FiledataExtractor extractor;
    std::string archive_fn;
    std::queue<std::string> file_queue;

    bool processSubdir(std::string);
    bool validEntry(std::string);


};

#endif
