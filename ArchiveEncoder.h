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

    bool processSubdir(std::string);
    bool validEntry(std::string);


};

#endif
