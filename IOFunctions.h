#ifndef IO_FUNCTIONS_H
#define IO_FUNCTIONS_H

#include <string>
#include <queue>

// Definition of handler functions
void readHandler(std::queue<std::string>&,std::string,std::string);
void writeHandler(std::queue<std::string>&,std::string);

// Definition of helper functions
std::string getLinkPath(std::string,std::string);

#endif
