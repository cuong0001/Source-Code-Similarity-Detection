# Source Code Similarity Detection

Source Code Similarity Detection project - Detecting structural similarities using the Winnowing algorithm.

## Overview

This project is part of the course CSC10004 - DSA at [VNU-HCMUS](https://hcmus.edu.vn/), providing a high-performance C++ engine that utilizes the **Winnowing algorithm** to efficiently detect structural similarities between source code submissions (C/C++, Python).

* **Video Presentation (10-minute):** 

[![Watch the video here](https://img.youtube.com/vi/_EnhDvOn-3w/mqdefault.jpg)](https://www.youtube.com/watch?v=_EnhDvOn-3w)
* **Dataset & Ground-truth:** Located in `./dataset` (Contains crawled [Codeforces](https://codeforces.com/) submissions and the evaluation pair mapping).
* **Utility Scripts:** Located in `./scripts` (Python scripts for crawling and data generation).


---

## Build & Run
**Prerequisite:** GCC Compiler (C++14 or higher).

```bash
# 1. Compile the source code into an executable named 'main'
g++ src/main.cpp src/winnowing.cpp src/boilerplate.cpp src/tokenizer.cpp -o main -O3

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
* **Data Generation:** We engineered a Python script (`similar_code_gen.py`) to automatically generate highly-obfuscated "cheat" variants (variable renaming, useless comments, spacing, extra headers) to rigorously stress-test the algorithm.

### 3. Core Algorithm & Implementation
We utilized the **Winnowing algorithm** to extract document fingerprints, heavily optimized for both accuracy and execution speed:
* **Preprocessing:** Remove whitespaces, comments, and **common boilerplate** (e.g., `#include <bits/stdc++.h>`) to minimize False Positives. 
* **Rolling Hash:** Generates $k$-gram hashes in $O(1)$ time. 
* **Monotonic Queue (`std::deque`):** Maintains the minimum hash value within a sliding window $w$, reducing the overall algorithm complexity to an optimal $O(N)$.

### 4. Experiment Results

The experiment was conducted on a dataset of **1350 pairs**, yielding an **Accuracy of 93.19%**. Below is the technical analysis of the experiment:

#### Similarity Measurement

To measure similarity between two source code files, we use **Jaccard Similarity** over the fingerprint sets:

$$
Similarity(A, B) = \frac{|F(A) \cap F(B)|}{|F(A) \cup F(B)|}
$$

In which: 
* $|F(A) \cap F(B)|$: The number of shared fingerprints between two files. 
* $|F(A) \cup F(B)|$: The total number of unique fingerprints from both files.

####  Experimental Configuration 
| Parameter | Value | Technical Description | 
| :--- | :--- | :--- | 
| **K-gram size ($k$)** | **25** | The token sequence length used to generate a single hash. | 
| **Window size ($w$)** | **5** | The sliding window range used to select the minimum fingerprint. |
| **Similarity Threshold ($T$)** | **60.0%** | The minimum overlap percentage to flag a pair as "Highly Similar". | 

#### Result Summary
* **True Positives (TP): 147 (10.89%)**
    * **Context:** Engineered variants with modified names, comments, headers and formatting.
    * **Outcome:** The Winnowing engine successfully "unmasked" most of these attempts, proving the system is highly resilient against surface-level obfuscation.
* **True Negatives (TN): 1111 (82.30%)**
    * **Context:** Pairs consisting of independent solutions or modified versions derived from entirely different original sources.
    * **Outcome:** The engine accurately identified these as distinct logical signatures. Whether comparing two completely original files or variants of different problems, the system avoided "false matches". This proves that the algorithm doesn't just look for generic coding patterns but for specific structural fingerprints, ensuring **high credibility and fairness** for all unique submissions.
* **False Positives (FP): 89 (6.59%)**
    * **Context:** Flagged as "Highly Similar" by the engine, but labeled "Independent" in the dataset.
    * **Heuristic Value:** While technically an error, these 89 cases are significant. In trivial problems (e.g., *71A - Way Too Long Words*), independent logic naturally converges. Furthermore, these flags often identify **"Stealthy Similarity"**—cases where students likely shared logic before the deadline, making this a powerful audit tool.
* **False Negatives (FN): 3 (0.22%)**
    * **Context:** Copied variants that fell below the similarity threshold despite being derived from the same source.
    * **Outcome:** These "misses" were primarily due to **extreme sensitivity in ultra-short programs.** In tiny scripts (e.g., under 15 lines), even a trivial change—such as adding a `\n` to a string or shifting the position of a `return` statement—disrupted a significant portion of the total fingerprints ($k=25$). Since these files produce so few hashes to begin with, these minor "formatting ripples" were enough to drag the score below our 60% threshold, exposing a physical limit of structural detection on very small code snippets.

####  Key Performance Indicators:

We utilize standard data science metrics to evaluate the reliability of our detection engine at the $60\%$ threshold:
* **Recall ($R = \frac{TP}{TP + FN}$): 98.00%**
    * With a near-perfect score, the system acts as a **comprehensive safety net**. It ensures that almost no suspicious similarity goes undetected, providing peace of mind for instructors. In a high-stakes academic environment, catching $98\%$ of variants—even those with significant obfuscation—is a major success for structural-based detection.
* **Precision ($P = \frac{TP}{TP + FP}$): 62.29%**
    * This represents a **conservative filtering strategy**. While a $62.29\%$ precision indicates some "noise" (mostly from very short, fundamental problems), it is a deliberate trade-off. By casting a wider net, we ensure high Recall, acting as an automated "first responder" that isolates suspicious cases for a final human audit.
* **F1-Score ($F1 = 2 \cdot \frac{P \cdot R}{P + R}$): 76.15%**
    * As the harmonic mean of Precision and Recall, the F1-Score of **$76.15\%$** proves the system's **overall robustness**. It demonstrates that the Winnowing algorithm remains highly effective and balanced, even when challenged by the inherent "label noise" of competitive programming datasets.

### 5. Discussion & Applicability

* **High-Speed Scalability:** Powered by a C++ core and $O(N)$ algorithmic complexity, the engine processes thousands of pairs in milliseconds. It serves as a high-performance **automated pre-filter**, allowing instructors to bypass thousands of "clean" submissions and focus audit efforts exclusively on the ~17% of high-risk cases.
* **The "Label Noise" Challenge in Short Code:** Our experiments revealed a critical sensitivity in **ultra-short files** (e.g., < 5 lines). With a fixed $k=25$, even minor "formatting ripples"—such as adding a `\n` to a string or slightly relocating a `return` statement—disrupted a disproportionately large percentage of the total fingerprints. This leads to **"False Divergence"**, where logically identical snippets appear structurally different to the algorithm simply because the $k$-gram size is too stiff relative to the total file length.
* **Strategic Parameters & Trade-offs:** The choice of $k=25$ was a deliberate move to prioritize **Precision** in larger projects by ignoring incidental overlap. However, the drop in similarity for micro-codes suggests that a **Dynamic Parameter** approach—where $k$ scales automatically with the file's token count—would be the ideal evolution for competitive programming environments.
* **Heuristic Value of False Positives:** The 89 FP cases demonstrate that the system is often **more sensitive than the ground-truth labels**. By identifying high structural overlap that manual observation might miss, the engine acts as a proactive tool for flagging "Stealthy Similarity"—cases where logic was likely shared or derived from a common source before the deadline.


### 6. Extensions 

While the current engine demonstrates high reliability, we have identified several key areas for future optimization to handle more complex scenarios:

* **Dynamic K-gram Scaling:** To address the "False Divergence" in micro-codes, we plan to implement an **Adaptive Windowing** system. This would automatically scale the value of $k$ based on the file's token count, ensuring high sensitivity for short snippets without sacrificing precision in large projects.
* **Minimum Complexity Filter:** Implementing an automated "Triviality Filter" to bypass files that lack enough structural information (e.g., under 10 tokens or simple "Hello World" programs), thereby reducing unnecessary False Positives.
* **Cross-Language Detection:** Expanding the Tokenizer to support a universal intermediate representation. This would allow the engine to detect structural similarity even when a solution is ported from **C++ to Python** or vice versa.
* **Visual Web Dashboard:** Developing a **Web-based GUI** with side-by-side code highlighting. This would allow instructors to drag-and-drop submissions and instantly see "Heatmaps" of overlapping code blocks for faster manual auditing.


---


## Authors:

* Nguyễn Phúc Gia Bảo ([Kevincrazy678](https://github.com/Kevincrazy678))
* Trần Kiên Cường ([cuong0001](https://github.com/cuong0001))
* Nguyễn Ngọc Phúc Nguyên ([Nguynie](https://github.com/Nguynie))



