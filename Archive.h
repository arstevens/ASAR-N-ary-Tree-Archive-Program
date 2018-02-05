#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string>
#include <fstream>
#include <queue>
#include "FiledataExtractor.h"

class Archive {

	public:
		Archive(std::string);

	protected:
		std::string root;
		std::ofstream fwriter;
		std::queue<std::string> dir_queue;

};

#endif
