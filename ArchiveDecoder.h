#ifndef ARCHIVE_DECODER_H
#define ARCHIVE_DECODER_H

#include <string>
#include <fstream>
#include "Archive.h"

class ArchiveDecoder : public Archive {
	public:
		ArchiveDecoder(std::string);
		bool extract();

	private:
		// Fields
		std::string archive_buff;
		bool readUntilEOE(std::ifstream&);
		bool processRegexStream(/*regex stream*/);

};


#endif
