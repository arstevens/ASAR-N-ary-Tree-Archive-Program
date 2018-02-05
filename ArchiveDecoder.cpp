#include "ArchiveDecoder.h"
#include "Archive.h"
#include "Tags.h"
#include <string>

// Constructor
ArchiveDecoder::ArchiveDecoder(std::string archive_file) :
															Archive(archive_file) { }


// Interface Methods
bool ArchiveDecoder::extract() {

}

// Helper Methods
bool ArchiveDecoder::readUntilEOE(std::ifstream &stream) {
	// Clear buffer
	archive_buff = "";

  // Read in enough data to satisfy EOE read loop
  for (int i = 0; i < 6; i++) {
      if (stream.fail())
              return false;
      archive_buff += stream.get();
  }

  // Read in until reach EOE or read fails
  int substr_location = archive_buff.size() - EOE.size();
  while (archive_buff.substr(substr_location) != EOE && !stream.fail()) {
      archive_buff += stream.get();
      substr_location = archive_buff.size() - EOE.size();
  }

	return true;

}

bool ArchiveDecoder::processRegexStream() {

}
