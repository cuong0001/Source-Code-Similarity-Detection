#include "tokenizer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <cctype>

using namespace std;

string tokenizeCode(const string& code) {
    unordered_set<string> types = {"int", "float", "double", "char", "void", "long", "bool", "string", "vector", "map", "set", "list", "dict", "tuple"};
    unordered_set<string> bool_literals = {"true", "false", "True", "False"};

    string result = "";
    int n = code.length();
    
    for (int i = 0; i < n; ) {
        char c = code[i];
        
        if (isspace(c) || c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',') {
            i++;
            continue;
        }

        if (isalpha(c) || c == '_') {
            string word = "";
            while (i < n && (isalnum(code[i]) || code[i] == '_')) {
                word += code[i++];
            }
            
            if (types.count(word)) result += "[TYPE]";
            else if (word == "for") result += "[FOR]";
            else if (word == "while") result += "[WHILE]";
            else if (word == "if") result += "[IF]";
            else if (word == "else" || word == "elif") result += "[ELSE]";
            else if (word == "return") result += "[RETURN]";
            else if (word == "def" || word == "class") result += "[DEF]";
            else if (word == "and" || word == "or" || word == "not") result += "[LOGIC]";
            else if (word == "const") result += "[CONST]";
            else if (bool_literals.count(word)) result += "[BOOL]"; 
            else result += "[ID]";
        } 

        else if (isdigit(c)) {
            while (i < n && (isdigit(code[i]) || code[i] == '.')) i++; 
            result += "[NUM]";
        } 
        else if (c == '=') {
            if (i + 1 < n && code[i+1] == '=') { result += "[EQ]"; i += 2; }
            else { result += "[ASSIGN]"; i++; }
        }
        else if (c == '+') {
            if (i + 1 < n && code[i+1] == '+') { result += "[INC]"; i += 2; }
            else { result += "[PLUS]"; i++; }
        }
        else if (c == '-') {
            if (i + 1 < n && code[i+1] == '-') { result += "[DEC]"; i += 2; }
            else { result += "[MINUS]"; i++; }
        }
        else if (c == '<') {
            if (i + 1 < n && code[i+1] == '=') { result += "[LTE]"; i += 2; }
            else { result += "[LT]"; i++; }
        }
        else if (c == '>') {
            if (i + 1 < n && code[i+1] == '=') { result += "[GTE]"; i += 2; }
            else { result += "[GT]"; i++; }
        }
        else if (c == '.') { result += "[DOT]"; i++; }
        else if (c == '[') { result += "[LBRACKET]"; i++; }
        else if (c == ']') { result += "[RBRACKET]"; i++; }
        else if (c == '!') {
            if (i + 1 < n && code[i+1] == '=') { result += "[NEQ]"; i += 2; }
            else { result += "[NOT]"; i++; }
        }
        else {
            result += c;
            i++;
        }
    }
    return result;
}

void tokenizeFile(const string& inputFilename, const string& outputFilename) {
    ifstream inFile(inputFilename);
    if (!inFile) {
        cerr << "Error: Could not open " << inputFilename << " for tokenization." << endl;
        return;
    }
    stringstream buffer;
    buffer << inFile.rdbuf();
    string rawContent = buffer.str();
    inFile.close();

    string tokenizedContent = tokenizeCode(rawContent);

    ofstream outFile(outputFilename);
    if (outFile) {
        outFile << tokenizedContent;
        outFile.close();
    }
}