#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <string>
#include <vector>

// Defines the programming languages supported by the system.
enum class Language { CPP, PYTHON, UNKNOWN };

// Utility for detecting language and basic string cleaning.
Language detectLanguage(const std::string& filename);
std::string trim(const std::string& str);

// State-machine based comment strippers (Safe for strings and escapes).
std::string removeCppComments(const std::string& raw);
std::string removePythonComments(const std::string& raw);

// Boilerplate filters: Identify non-logic lines.
bool isCppBoilerplate(const std::string& line);
bool isPythonBoilerplate(const std::string& line);

// Main pipeline: Reads, cleans comments, filters boilerplate, and saves the logic-only result.
void preprocessForWinnowing(const std::string& inputFilename, const std::string& outputFilename);

#endif