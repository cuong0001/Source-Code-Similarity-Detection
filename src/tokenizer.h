#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

std::string tokenizeCode(const std::string& code);

void tokenizeFile(const std::string& inputFilename, const std::string& outputFilename);

#endif