#include "winnowing.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <deque>

using namespace std;

string preprocess(const string& text) {
    string cleaned = "";
    for (char c : text) {
        if (!isspace(c)) {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

set<uint64_t> getFingerprintHashes(const string& text, int k, int w) {
    int n = text.length();
    if (n < k) return {};

    const uint64_t P = 31; 
    uint64_t high_p = 1;
    for (int i = 0; i < k - 1; i++) high_p *= P;

    vector<uint64_t> hashes;
    uint64_t current_hash = 0;

    for (int i = 0; i < k; i++) {
        current_hash = current_hash * P + (uint8_t)text[i];
    }
    hashes.push_back(current_hash);

    for (int i = 1; i <= n - k; i++) {
        current_hash = (current_hash - (uint8_t)text[i - 1] * high_p) * P + (uint8_t)text[i + k - 1];
        hashes.push_back(current_hash);
    }

    set<uint64_t> fingerprint_hashes;
    deque<int> dq;
    
    int num_hashes = hashes.size();
    for (int i = 0; i < num_hashes; i++) {
        if (!dq.empty() && dq.front() <= i - w) {
            dq.pop_front();
        }
        while (!dq.empty() && hashes[dq.back()] >= hashes[i]) {
            dq.pop_back();
        }
        dq.push_back(i);

        if (i >= w - 1) {
            fingerprint_hashes.insert(hashes[dq.front()]);
        }
    }

    return fingerprint_hashes;
}

double calculateSimilarity(const set<uint64_t>& set1, const set<uint64_t>& set2) {
    if (set1.empty() && set2.empty()) return 100.0;
    if (set1.empty() || set2.empty()) return 0.0;

    vector<uint64_t> intersect;
    vector<uint64_t> unite;

    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(intersect));
    set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(unite));

    return (double)intersect.size() / unite.size() * 100.0;
}

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}