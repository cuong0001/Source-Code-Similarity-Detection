#include "boilerplate.h"
#include <iostream>
#include <fstream>

using namespace std;

string trimLeading(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    return str.substr(first);
}

bool isCodeStart(const string& line) {
    string trimmed = trimLeading(line);
    if (trimmed.empty()) return false;

    const vector<string> keywords = {"void", "int", "long", "struct", "class", "template", "double", "float"};

    for (const string& key : keywords) {
        if (trimmed.compare(0, key.length(), key) == 0) {
            if (trimmed.length() == key.length() || !isalpha(trimmed[key.length()])) {
                return true;
            }
        }
    }
    return false;
}

void preprocessForWinnowing(string inputFilename, string outputFilename) {
    ifstream inFile(inputFilename);
    ofstream outFile(outputFilename);

    if (!inFile || !outFile) {
        cerr << "Error: Could not open files." << endl;
        return;
    }

    string line;
    bool logicStarted = false;

    while (getline(inFile, line)) {
        if (!logicStarted) {
            if (isCodeStart(line)) {
                logicStarted = true;
                outFile << line << "\n";
            }
        } else {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();
}

Language detectLanguage(const string& filename) {
    if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".cpp") return Language::CPP;
    if (filename.length() >= 2 && filename.substr(filename.length() - 2) == ".c") return Language::CPP;
    if (filename.length() >= 3 && filename.substr(filename.length() - 3) == ".py") return Language::PYTHON;
    return Language::UNKNOWN;
}

string removeCppComments(const string& raw) {
    string result = "";
    bool inString = false, inChar = false, inSingleComment = false, inMultiComment = false;
    for (size_t i = 0; i < raw.length(); ++i) {
        if (inSingleComment) { if (raw[i] == '\n') { inSingleComment = false; result += raw[i]; } } 
        else if (inMultiComment) { if (raw[i] == '*' && i + 1 < raw.length() && raw[i+1] == '/') { inMultiComment = false; i++; } } 
        else if (inString) { result += raw[i]; if (raw[i] == '\\' && i + 1 < raw.length()) result += raw[++i]; else if (raw[i] == '"') inString = false; } 
        else if (inChar) { result += raw[i]; if (raw[i] == '\\' && i + 1 < raw.length()) result += raw[++i]; else if (raw[i] == '\'') inChar = false; } 
        else {
            if (raw[i] == '/' && i + 1 < raw.length() && raw[i+1] == '/') { inSingleComment = true; i++; } 
            else if (raw[i] == '/' && i + 1 < raw.length() && raw[i+1] == '*') { inMultiComment = true; i++; } 
            else if (raw[i] == '"') { inString = true; result += raw[i]; } 
            else if (raw[i] == '\'') { inChar = true; result += raw[i]; } 
            else { result += raw[i]; }
        }
    }
    return result;
}

string removePythonComments(const string& raw) {
    string result = "";
    bool inString1 = false, inString2 = false, inDocstring1 = false, inDocstring2 = false, inComment = false; 
    for (size_t i = 0; i < raw.length(); ++i) {
        if (inComment) { if (raw[i] == '\n') { inComment = false; result += raw[i]; } } 
        else if (inDocstring1) { if (raw[i] == '\\' && i + 1 < raw.length()) i++; else if (raw[i] == '\'' && i+2 < raw.length() && raw[i+1] == '\'' && raw[i+2] == '\'') { inDocstring1 = false; i += 2; } } 
        else if (inDocstring2) { if (raw[i] == '\\' && i + 1 < raw.length()) i++; else if (raw[i] == '"' && i+2 < raw.length() && raw[i+1] == '"' && raw[i+2] == '"') { inDocstring2 = false; i += 2; } } 
        else if (inString1) { result += raw[i]; if (raw[i] == '\\' && i + 1 < raw.length()) result += raw[++i]; else if (raw[i] == '\'') inString1 = false; } 
        else if (inString2) { result += raw[i]; if (raw[i] == '\\' && i + 1 < raw.length()) result += raw[++i]; else if (raw[i] == '"') inString2 = false; } 
        else {
            if (raw[i] == '#') { inComment = true; } 
            else if (raw[i] == '\'' && i+2 < raw.length() && raw[i+1] == '\'' && raw[i+2] == '\'') { inDocstring1 = true; i += 2; } 
            else if (raw[i] == '"' && i+2 < raw.length() && raw[i+1] == '"' && raw[i+2] == '"') { inDocstring2 = true; i += 2; } 
            else if (raw[i] == '\'') { inString1 = true; result += raw[i]; } 
            else if (raw[i] == '"') { inString2 = true; result += raw[i]; } 
            else { result += raw[i]; }
        }
    }
    return result;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool isCppBoilerplate(const string& line) {
    if (line.empty() || line[0] == '#' || line.compare(0, 5, "using") == 0) return true; 
    return false;
}

bool isPythonBoilerplate(const string& line) {
    if (line.empty() || line.compare(0, 6, "import") == 0 || line.compare(0, 4, "from") == 0) return true;   
    return false;
}