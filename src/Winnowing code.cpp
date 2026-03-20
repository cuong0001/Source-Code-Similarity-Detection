#include <bits/stdc++.h>

using namespace std;

/**
 * Preprocess the text: remove whitespaces and convert to lowercase.
 * This ensures formatting differences don't affect the fingerprints.
 */
string preprocess(const string& text) {
    string cleaned = "";
    for (char c : text) {
        if (!isspace(c)) {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

/**
 * Winnowing Algorithm: Extracts a set of representative hash values (fingerprints).
 * Uses Rolling Hash and a Monotonic Queue (deque) for O(N) complexity.
 */
set<uint64_t> getFingerprintHashes(const string& text, int k, int w) {
    int n = text.length();
    if (n < k) return {};

    // 1. Rolling Hash (Rabin-Karp style)
    const uint64_t P = 31; 
    uint64_t high_p = 1;
    for (int i = 0; i < k - 1; i++) high_p *= P;

    vector<uint64_t> hashes;
    uint64_t current_hash = 0;

    // Calculate hash for the first k-gram
    for (int i = 0; i < k; i++) {
        current_hash = current_hash * P + (uint8_t)text[i];
    }
    hashes.push_back(current_hash);

    // Roll the hash for subsequent k-grams: O(N)
    for (int i = 1; i <= n - k; i++) {
        current_hash = (current_hash - (uint8_t)text[i - 1] * high_p) * P + (uint8_t)text[i + k - 1];
        hashes.push_back(current_hash);
    }

    // 2. Select Fingerprints using a Sliding Window
    set<uint64_t> fingerprint_hashes;
    deque<int> dq; // Stores indices of hashes to maintain the minimum in the current window
    
    int num_hashes = hashes.size();
    for (int i = 0; i < num_hashes; i++) {
        // Remove indices that are out of the current window range
        if (!dq.empty() && dq.front() <= i - w) {
            dq.pop_front();
        }
        // Maintain monotonic property (minimum at the front)
        while (!dq.empty() && hashes[dq.back()] >= hashes[i]) {
            dq.pop_back();
        }
        dq.push_back(i);

        // Once the window is full, the element at the front is the minimum
        if (i >= w - 1) {
            fingerprint_hashes.insert(hashes[dq.front()]);
        }
    }

    return fingerprint_hashes;
}

/**
 * Calculate similarity using Jaccard Coefficient: |A n B| / |A ? B|
 */
double calculateSimilarity(const set<uint64_t>& set1, const set<uint64_t>& set2) {
    if (set1.empty() && set2.empty()) return 100.0;
    if (set1.empty() || set2.empty()) return 0.0;

    vector<uint64_t> intersect;
    vector<uint64_t> unite;

    // Compute intersection and union in O(N)
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(intersect));
    set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), back_inserter(unite));

    return (double)intersect.size() / unite.size() * 100.0;
}

/**
 * Helper function to read entire file content into a string.
 */
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

int main() {
    // Standard I/O optimization for large inputs
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Configuration parameters
    int k = 10; // K-gram length
    int w = 5;  // Window size

    // File paths - You can change these to your actual file names
    string file1_path = "input1.txt";
    string file2_path = "input2.txt";

    // Step 1: Read files
    string content1 = readFile(file1_path);
    string content2 = readFile(file2_path);

    if (content1.empty() || content2.empty()) {
        cout << "One or both files are empty or missing. Exiting..." << endl;
        return 1;
    }

    // Step 2: Preprocess and extract fingerprints
    set<uint64_t> fp1 = getFingerprintHashes(preprocess(content1), k, w);
    set<uint64_t> fp2 = getFingerprintHashes(preprocess(content2), k, w);

    // Step 3: Compare and output results
    double similarity = calculateSimilarity(fp1, fp2);

    cout << "--- Winnowing Similarity Report ---" << endl;
    cout << "File 1: " << file1_path << " (" << fp1.size() << " fingerprints)" << endl;
    cout << "File 2: " << file2_path << " (" << fp2.size() << " fingerprints)" << endl;
    cout << "Similarity Percentage: " << similarity << "%" << endl;

    return 0;
}
