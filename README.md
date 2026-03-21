# Source-Code-Similarity-Detection

Source Code Similarity Detection project - Detecting structural similarities using the Winnowing algorithm.

## Overview

This project is part of the course CSC10004 - DSA at [VNU-HCMUS](https://hcmus.edu.vn/), providing a high-performance C++ engine that utilizes the **Winnowing algorithm** to efficiently detect structural similarities between source code submissions (C/C++, Python).

* **Video Presentation (10-minute):** 

[![Watch the video here](https://img.youtube.com/vi/_EnhDvOn-3w/mqdefault.jpg)](https://www.youtube.com/watch?v=_EnhDvOn-3w)
* **Dataset & Ground-truth:** Located in `./dataset` (Contains crawled [Codeforces](https://codeforces.com) submissions and the evaluation pair mapping).
* **Utility Scripts:** Located in `./scripts` (Python scripts for crawling and data generation).


---

## Build & Run
**Prerequisite:** GCC Compiler (C++14 or higher).

```bash
# 1. Compile the source code into an executable named 'main'
g++ src/main.cpp src/winnowing.cpp src/boilerplate.cpp -o main -O3

# 2. Run the evaluation
./main
```

---

## Project Report

### 1. Problem Statement
In academic and competitive programming environments, verifying the originality of source code is a major challenge. The system must distinguish between incidental similarities (e.g., extremely simple logic, shared `#include` templates). This project focuses on benchmarking "Similarity" percentages, providing a robust metric that, when combined with proper thresholding, accurately flags suspicious submissions.

### 2. Finalized Dataset
* **Source:** Real-world "Accepted" Codeforces submissions. 
* **Dataset Size:** **1350 File Pairs** mapped in `dataset_pairs.csv` (`Label 1`: Base-Variant, `Label 0`: Independent).
* **Data Generation:** We engineered a Python script (`similar_code_gen.py`) to automatically generate highly-obfuscated "cheat" variants (variable renaming, dead-comments, spacing, extra headers) to rigorously stress-test the algorithm.

### 3. Core Algorithm & Implementation
We utilized the **Winnowing algorithm** to extract document fingerprints, heavily optimized for both accuracy and execution speed:
* **Preprocessing:** Remove whitespaces, comments, and **common boilerplate** (e.g., `#include <bits/stdc++.h>`) to minimize False Positives. 
* **Rolling Hash:** Generates $k$-gram hashes in $O(1)$ time. 
* **Monotonic Queue (`std::deque`):** Maintains the minimum hash value within a sliding window $w$, reducing the overall algorithm complexity to an optimal $O(N)$.

### 4. Experiment Results
The algorithm was benchmarked against the 1350 file pairs using $k = 10$, $w = 5$, and a Similarity Threshold of $T = 60\%$.
* **Accuracy:** `[Insert %]`
* **False Positives (FP):** `[Insert Number]` pairs. (Mostly occurred in extremely short, fundamental problems where independent logical structures are naturally identical).
* **False Negatives (FN):** `[Insert Number]` pairs.

### 5. Discussion & Applicability
* **Significance:** The algorithm operates very fast due to its C++ core, capable of processing thousands of pairs in milliseconds. It serves perfectly as an automated pre-filter for grading systems, isolating highly suspicious submissions for manual review.
* **The "Label Noise" Challenge:** Our experiments highlighted a physical limitation of similarity detection: for trivial problems (under 15 lines of code), independent submissions naturally yield >80% similarity. The algorithm measures structural similarity correctly, but using this raw metric for cheating accusations on short code is a physical limitation.

### 6. Extensions 
* **AST-based Obfuscation:** Upgrading the dataset generation script from Regex to use Abstract Syntax Trees (AST) to automatically restructure loops (`for`/`while`) and branch logic for more rigorous testing. 
* **Minimum Token Filter:** Automatically bypass evaluation for extremely short files. 
* **Dynamic Thresholding:** Adjust the $60\%$ threshold based on problem complexity. 
* **Web GUI:** A drag-and-drop interface for easier usage.

---


## Authors:

* Nguyễn Phúc Gia Bảo ([KevinCrazy678](https://github.com/Kevincrazy678))
* Trần Kiên Cường ([cuong0001](https://github.com/cuong0001))
* Nguyễn Ngọc Phúc Nguyên ([Nguynie](https://github.com/Nguynie))



