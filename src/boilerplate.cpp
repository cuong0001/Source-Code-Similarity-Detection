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