# Source-Code-Similarity-Detection

Source Code Similarity Detection project - Detecting structural similarities using the Winnowing algorithm.

## Overview

This project is part of the course CSC10004 - DSA at VNU-HCMUS, focusing on detecting structural similarities between source code submissions (C/C++, Python) while significantly reducing false positives caused by boilerplate code and common templates.

* **Video Presentation (10-minute):** [Insert YouTube Link Here]
* **Dataset & Ground-truth:** Located in `./dataset` (Contains crawled Codeforces submissions and the evaluation pair mapping).
* **Utility Scripts:** Located in `./scripts` (Python scripts for crawling and data generation).

---

## Project Report

### 1. Problem Statement
In academic and competitive programming environments, verifying the originality of source code is a major challenge. The system must distinguish between incidental similarities (e.g., extremely simple logic, shared `#include` templates). This project focuses on benchmarking "Similarity" percentages, providing a robust metric that, when combined with proper thresholding, accurately flags suspicious submissions.

### 2. Finalized Dataset
* **Data Collection:** Real-world submissions were crawled from Codeforces with the "Accepted" status.
* **Advanced Manual Injection:** To stress-test the algorithm, we intentionally generated complex "cheat" variants from the base files by modifying loops (`for` to `while`), inverting conditional statements, and obfuscating variable names.
* **Ground-truth Mapping (`dataset_pairs.csv`):** Instead of labeling individual files, we strictly evaluate **File Pairs**. The dataset comprises **1351 pairs**, mapped as:
  * `Label 1 (Base-Variant)`: Files sharing the same origin (Plagiarized).
  * `Label 0 (Independent)`: Submissions from different authors solving the same/different problems (Independent).

### 3. Core Algorithm & Implementation
We utilized the **Winnowing algorithm** to extract document fingerprints, heavily optimized for both accuracy and execution speed:
* **Robust Preprocessing:** Before hashing, the source code undergoes aggressive cleaning: removing whitespaces, comments, and crucially, **ignoring common boilerplate code** (e.g., `#include <bits/stdc++.h>`, standard I/O loops). This drastically minimizes False Positives.
* **C++ Core Engine:** The algorithm is implemented entirely in C++. We applied the **Rolling Hash** technique for $k$-grams generation and utilized a **Monotonic Queue (`std::deque`)** to maintain the minimum hash value within the sliding window $w$. This reduces the time complexity to an optimal $O(N)$.

### 4. Experiment Results
The system was benchmarked against the 1351 file pairs using $k = 10$, $w = 5$, and a Similarity Threshold of $T = 60\%$.
* **Total Pairs Evaluated:** 1351
* **Accuracy:** `[Insert %]`
* **False Positives (FP):** `[Insert Number]` pairs. (Mostly occurred in extremely short, fundamental problems where independent logical structures are naturally identical).
* **False Negatives (FN):** `[Insert Number]` pairs.

### 5. Discussion & Applicability
* **Significance:** The system operates at lightning speed due to its C++ core, capable of processing thousands of pairs in milliseconds. It serves perfectly as an automated pre-filter for grading systems, isolating highly suspicious submissions for manual review.
* **The "Label Noise" Challenge:** Our experiments highlighted a physical limitation of similarity detection: for trivial problems (under 15 lines of code), independent submissions can yield >80% similarity. The algorithm is mathematically correct, but using this raw metric directly for plagiarism accusations can be misleading.

### 6. Extensions
To address the aforementioned limitations, future development could include:
1. **Minimum Token Length:** Automatically bypassing evaluation for files lacking sufficient logical tokens.
2. **Dynamic Thresholding:** Adjusting the $60\%$ threshold dynamically based on the problem's average complexity.
3. **Web-based GUI:** Transitioning from the Command Line Interface to a user-friendly drag-and-drop web application.

---


## Authors:

* Nguyễn Phúc Gia Bảo ([KevinCrazy678](https://github.com/Kevincrazy678))
* Trần Kiên Cường ([cuong0001](https://github.com/cuong0001))
* Nguyễn Ngọc Phúc Nguyên ([Nguynie](https://github.com/Nguynie))

