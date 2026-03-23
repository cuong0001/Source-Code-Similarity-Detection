#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <string>
#include <vector>

// Added for multi-language support
enum class Language { CPP, PYTHON, UNKNOWN };

// Trim the leading character
std::string trimLeading(const std::string& str);

// Check if a line starts with some crucial keywords
bool isCodeStart(const std::string& line);

// Detect language and remove comments based on the language.
Language detectLanguage(const std::string& filename);
std::string removeCppComments(const std::string& raw);
std::string removePythonComments(const std::string& raw);

// Main pipeline
void preprocessForWinnowing(std::string inputFilename, std::string outputFilename);

#endif