#ifndef WINNOWING_H
#define WINNOWING_H

#include <string>
#include <vector>
#include <set>
#include <cstdint>

/**
 * Normalizes text by removing whitespaces and converting all characters to lowercase.
 */
std::string preprocess(const std::string& text);

/**
 * Generates a set of representative hash values (fingerprints) from the text.
 * Uses a sliding window over k-grams to select the minimum hash in each window.
 * @param k The length of the k-gram.
 * @param w The window size.
 */
std::set<uint64_t> getFingerprintHashes(const std::string& text, int k, int w);

/**
 * Calculates the Jaccard similarity coefficient between two sets of fingerprints.
 * Returns a percentage value between 0.0 and 100.0.
 */
double calculateSimilarity(const std::set<uint64_t>& set1, const std::set<uint64_t>& set2);

/**
 * Reads the entire content of a file into a single string.
 */
std::string readFile(const std::string& filename);

#endif