#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdio> 
#include <vector>
#include "winnowing.h"
#include "boilerplate.h"
#include "tokenizer.h"

using namespace std;

int main() {
    auto trimPath = [](string s) -> string {
        while (!s.empty() && (isspace(s.back()) || s.back() == '\r')) s.pop_back();
        while (!s.empty() && (isspace(s.front()) || s.front() == '\r')) s.erase(0, 1);
        return s;
    };

    int k = 25;
    int w = 5;  
    double threshold = 60.0; 

    int tp = 0, fp = 0, tn = 0, fn = 0;

    cout << "Config: k=" << k << ", w=" << w << ", threshold=" << threshold << "%\n";

    string inputCSV = "dataset/dataset_pairs.csv";
    string outputCSV = "experiment_results.csv";

    ifstream inFile(inputCSV);
    if (!inFile) {
        cerr << "Error: Unable to open " << inputCSV << ". Please run from root folder." << endl;
        return 1;
    }

    ofstream outFile(outputCSV);
    if (!outFile) {
        cerr << "Error: Could not create file." << outputCSV << endl;
        return 1;
    }

    string header;
    if (getline(inFile, header)) {
        while (!header.empty() && header.back() == '\r') header.pop_back();
        outFile << header << ",similarity,evaluation\n";
    }

    string cleanFile1 = "temp1.clean", cleanFile2 = "temp2.clean";
    string tokenFile1 = "temp1.token", tokenFile2 = "temp2.token";

    string line;
    int count = 0;

    cout << "Scanning data.." << endl;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string f1, f2, problem, pair_type, gt_str;

        getline(ss, f1, ',');
        getline(ss, f2, ',');
        getline(ss, problem, ',');
        getline(ss, pair_type, ',');
        getline(ss, gt_str, ',');

        f1 = trimPath(f1);
        f2 = trimPath(f2);
        int ground_truth = stoi(trimPath(gt_str));

        preprocessForWinnowing(f1, cleanFile1);
        preprocessForWinnowing(f2, cleanFile2);
        tokenizeFile(cleanFile1, tokenFile1);
        tokenizeFile(cleanFile2, tokenFile2);

        string text1 = readFile(tokenFile1);
        string text2 = readFile(tokenFile2);

        double similarity = 0.0;
        if (!text1.empty() && !text2.empty()) {
            string proc1 = preprocess(text1);
            string proc2 = preprocess(text2);
            auto fp1 = getFingerprintHashes(proc1, k, w);
            auto fp2 = getFingerprintHashes(proc2, k, w);
            similarity = calculateSimilarity(fp1, fp2);
        }

        string evaluation = "";
        if (similarity >= threshold) {
            if (ground_truth == 1) { evaluation = "TP"; tp++; }
            else { evaluation = "FP"; fp++; }
        } else {
            if (ground_truth == 1) { evaluation = "FN"; fn++; }
            else { evaluation = "TN"; tn++; }
        }

        outFile << f1 << "," << f2 << "," << problem << "," << pair_type << "," 
                << ground_truth << "," << fixed << setprecision(2) << similarity 
                << "," << evaluation << "\n";

        count++;
        if (count % 20 == 0) cout << "\rProgress: " << count << " file pairs" << flush;
    }

    remove(cleanFile1.c_str()); remove(cleanFile2.c_str());
    remove(tokenFile1.c_str()); remove(tokenFile2.c_str());
    inFile.close(); outFile.close();

    cout << "\n Results:\n";
    cout << "Total file pairs: " << count << "\n";
    cout << " - [TP] True Positives:  " << tp << "\n";
    cout << " - [FP] False Positives: " << fp << "\n";
    cout << " - [TN] True Negatives:  " << tn << "\n";
    cout << " - [FN] False Negatives: " << fn << "\n";
    
    double precision = (tp + fp > 0) ? (double)tp / (tp + fp) * 100 : 0;
    double recall = (tp + fn > 0) ? (double)tp / (tp + fn) * 100 : 0;
    
    cout << "Precision: " << fixed << setprecision(2) << precision << "%\n";
    cout << "Recall:    " << fixed << setprecision(2) << recall << "%\n";
    cout << "Detailed results in " << outputCSV << ".\n";

    return 0;
}