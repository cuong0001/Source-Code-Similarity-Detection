#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Trims leading whitespaces/tabs from a string.
 * This ensures we can detect keywords even if they are indented.
 */
string trimLeading(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    return str.substr(first);
}

/**
 * Checks if a line starts with common C++ "logic" keywords.
 * These usually mark the end of the boilerplate section.
 */
bool isCodeStart(const string& line) {
    string trimmed = trimLeading(line);
    if (trimmed.empty()) return false;

    // List of common keywords that indicate the start of actual logic
    const vector<string> keywords = {"void", "int", "long", "struct", "class", "template", "double", "float"};

    for (const string& key : keywords) {
        // Check if the trimmed line starts with the keyword followed by a space or '('
        if (trimmed.compare(0, key.length(), key) == 0) {
            // Check boundary to avoid matching words like "integrated" when searching for "int"
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
        // Once logicStarted is true, we keep every line until the end of the file
        if (!logicStarted) {
            if (isCodeStart(line)) {
                logicStarted = true;
                outFile << line << "\n";
            }
            // Lines before the first keyword (includes, namespaces) are ignored
        } else {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();
    cout << "Preprocessing complete. Logic extracted to: " << outputFilename << endl;
}

int main() {
    // Example usage
    string sourceFile = "code_mau.cpp";
    string cleanFile = "code_da_loc.cpp";

    preprocessForWinnowing(sourceFile, cleanFile);

    return 0;
}
