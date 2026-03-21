#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <string>
#include <vector>

// Trim the leading character
std::string trimLeading(const std::string& str);

// CHeck if a line starts with some crucial keywords
bool isCodeStart(const std::string& line);


void preprocessForWinnowing(std::string inputFilename, std::string outputFilename);

#endif