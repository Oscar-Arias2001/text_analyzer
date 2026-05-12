# 📝 Linguistic Analysis & Readability Tool

A robust command-line text analyzer written in C that performs detailed linguistic analysis, readability evaluation, and persistent report storage.

This project was designed with a strong emphasis on:

- Defensive programming
- Memory safety
- Stream abstraction
- Readability metrics
- Terminal user experience (UX)
- Persistent data storage

The application analyzes text entered by the user and generates a complete linguistic report both in the terminal and in a persistent log file.

---

## 📌 Features

### 🔎 Detailed Linguistic Analysis

The program performs a complete character-by-character analysis of the input text, including:

- Word count
- Sentence count
- Vowel count
- Consonant count
- Digit count
- Space count
- Punctuation count
- Alphabetic character count
- Character count with spaces
- Character count without spaces
- Average characters per word
- Average characters per sentence
- Average words per sentence
- Longest word detection

---

### 📚 Coleman–Liau Readability Index

The analyzer automatically calculates the **Coleman–Liau readability index**, estimating the U.S. grade level required to understand the text.

Examples:

- `Before Grade 1`
- `Grade 5`
- `Grade 10`
- `Grade 16+`

---

### 💾 Persistent Report Storage

Every analysis is automatically appended to:

```txt
text_analysis_results.txt
```

This creates a chronological history of all analyses instead of overwriting previous results.

---

### 🛡️ Defensive Programming & Active Security

The project includes several security-focused mechanisms:

#### Anti-DoS Protection

A strict maximum word length is enforced:

```c
#define MAX_WORD_LENGTH 10000
```

This prevents malicious memory exhaustion attacks caused by extremely long words.

If the limit is exceeded:

```txt
[SECURITY ERROR]: The maximum length for a single word has been exceeded. Potential DoS detected.
```

---

### 🧠 Stream Abstraction

The project demonstrates a powerful C concept:

> Both terminal output (`stdout`) and physical files are treated as streams (`FILE *`).

A single reporting function is reused for:

- Console output
- File output

This avoids duplicated logic and improves maintainability.

---

### ⚡ Single-Pass Text Processing

The analyzer uses a highly optimized **single-loop character analysis strategy**.

Instead of iterating through the text multiple times, the program classifies every character during a single traversal.

Benefits:

- Better performance
- Lower complexity
- Reduced redundant operations

---

### 🧹 Input Sanitization

The custom `trim()` function removes:

- Leading spaces
- Trailing spaces
- Tabs
- New lines
- Multiple whitespace edge cases

This ensures accurate word and sentence calculations.

---

### 🧵 Dynamic Memory Management

The longest word is stored dynamically using:

- `malloc()`
- `free()`

The program safely manages heap memory and prevents leaks by:

- Releasing old buffers before reallocating
- Nullifying freed pointers
- Validating allocation success

---

### 🎛️ Improved Terminal UX

The application includes:

- ASCII banner interface
- Animated loading feedback
- Real-time buffering control using `fflush(stdout)`

Example:

```txt
[We are analyzing the text. Please be patient. . . ]
```

---

# 🖼️ Preview

## Startup Screen

```plaintext
 _____                                                                         _____ 
( ___ )-----------------------------------------------------------------------( ___ )
 |   |  ████████╗███████╗██╗  ██╗████████╗     █████╗ ███╗   ██╗ █████╗        |   | 
 |   |  ╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝    ██╔══██╗████╗  ██║██╔══██╗       |   | 
 |   |     ██║   █████╗   ╚███╔╝    ██║       ███████║██╔██╗ ██║███████║       |   | 
 |   |     ██║   ██╔══╝   ██╔██╗    ██║       ██╔══██║██║╚██╗██║██╔══██║       |   | 
 |   |     ██║   ███████╗██╔╝ ██╗   ██║       ██║  ██║██║ ╚████║██║  ██║       |   | 
 |   |     ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝       ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       |   | 
 |___|                                                                         |___| 
(_____)-----------------------------------------------------------------------(_____)
```

---

# 📖 Example Usage

## User Input

```txt
Enter the text you want to analyze and press ENTER to start the analysis
The text is = When he was nearly thirteen, my brother Jem got his arm badly broken at the elbow.
```

---

## Terminal Output

```txt
===============================================================
                    TEXT ANALYZER REPORT
===============================================================
Analyzed Text:                 When he was nearly thirteen, my brother Jem got his arm badly broken at the elbow.
Day Of Analysis:               Monday
Date Of Analysis:              05/11/26
Analysis Time:                 3.004000s
---------------------------------------------------------------
Amount Of Words:               18
Amount Of Vowels:              24
Amount Of Consonants:          45
Amount Of Spaces:              17
Number Of Digits:              0
Number Of Punctuation:         2
Characters (With Spaces):      84
Characters (Without Spaces):   67
Characters Per Words:          3.83
Alphabetic Characters:         69
Amount Of Sentences:           1
Characters Per Sentences:      69.00
Words Per Sentences:           18.00
Longest Word:                  thirteen with 8 characters
Coleman-Liau Index:            Grade 5
```

---

# 🧠 Internal Architecture

## Program Flow

The application follows a layered and defensive workflow:

```text
User Input
   ↓
Input Validation
   ↓
Whitespace Trimming
   ↓
Single-Pass Linguistic Analysis
   ↓
Longest Word Detection
   ↓
Coleman–Liau Calculation
   ↓
Report Generation
   ↓
Persistent File Storage
```

---

# 🔬 Core Components

## `trim()`

Removes leading and trailing whitespace safely using:

- Pointer arithmetic
- `memmove()`
- `isspace()`

---

## `getValidString()`

Ensures:

- No empty strings
- Safe user input
- Proper error handling
- Immediate termination on memory failure

---

## `analyzeText()`

The core analysis engine.

Responsibilities:

- Word counting
- Sentence counting
- Character classification
- Punctuation handling
- Digit detection
- Sentence boundary detection

The logic is implemented hierarchically to progressively eliminate character classification possibilities.

---

## `findLongestWord()`

Detects the longest valid word while supporting:

- Hyphenated words
- Apostrophes within words

Examples considered valid:

```txt
mother-in-law
don't
```

---

## `updateCharsCount()`

Security-focused helper function that:

- Prevents excessive word lengths
- Stops unsafe counter growth
- Detects potential DoS scenarios

---

## `updateLongestWord()`

Handles:

- Dynamic memory allocation
- Safe copying using `strncpy()`
- Proper null termination
- Previous memory cleanup

---

## `colemanLiauIndex()`

Calculates readability using:

```txt
CLI = 0.0588L - 0.296S - 15.8
```

Where:

- `L` = average letters per 100 words
- `S` = average sentences per 100 words

---

## `printTextAnalysisReport()`

Demonstrates stream abstraction by generating output for both:

- Terminal (`stdout`)
- Persistent log file

using the same function.

---

# 🧱 Technologies & Concepts Used

## Language

- C

---

## Libraries

### Standard Libraries

```c
<stdio.h>
<string.h>
<ctype.h>
<stdlib.h>
<stdbool.h>
<math.h>
<time.h>
<unistd.h>
```

---

### External Library

```c
cs50.h
```

Used for secure string input handling.

---

# 🧮 Algorithms & Programming Concepts

This project demonstrates practical usage of:

- Dynamic memory allocation
- Pointer arithmetic
- Stream abstraction
- Defensive programming
- Character classification
- Readability algorithms
- File persistence
- Heap memory management
- Struct-based state management
- Boolean flag state machines
- Relative indexing techniques
- Buffer flushing
- Input sanitization

---

# 🛠️ Installation

## Requirements

- GCC compiler
- `cs50.h`
- `cs50.c`

---

# 🚀 Compilation & Execution

## Linux / macOS / Codespaces

### Compile

```bash
gcc -o analyzer main.c cs50.c -lm
```

### Run

```bash
./analyzer
```

---

## Windows (MSYS2 / MinGW)

### Compile

```bash
gcc main.c cs50.c -o analyzer.exe
```

### Run

```bash
./analyzer.exe
```

---

# 📂 Project Structure

```txt
.
├── main.c
├── cs50.c
├── cs50.h
├── text_analysis_results.txt
└── README.md
```

---

## File Descriptions

### `main.c`

Contains:

- Core linguistic analysis
- Readability calculations
- Dynamic memory management
- Persistence logic
- Reporting system

---

### `cs50.c / cs50.h`

Harvard CS50 helper library used for safer string input handling.

---

### `text_analysis_results.txt`

Automatically generated persistent log file containing all previous analyses.

---

# 🧪 Edge Cases Handled

The analyzer was intentionally designed to handle problematic scenarios safely.

Examples:

- Empty input
- Multiple spaces
- Leading/trailing spaces
- Extremely large words
- Consecutive punctuation
- Mixed digits and text
- Hyphenated words
- Apostrophes inside words

---

# 🔒 Memory Safety Considerations

The program actively avoids:

- Memory leaks
- Dangling pointers
- Unsafe allocations
- Invalid pointer access
- Buffer misuse

Examples of safeguards include:

```c
if (phrase->longest_word != NULL)
    free(phrase->longest_word);
```

and:

```c
phrase->longest_word = NULL;
```

after freeing memory.

---

# 📈 Future Improvements

Potential future enhancements include:

- UTF-8 multilingual support
- Additional readability indexes
- Batch file analysis
- Statistical visualization

---

# 👨‍💻 Author

Developed by Oscar Arias.

This project represents an important transition from:

> Volatile runtime data → Persistent disk storage

while applying advanced concepts in:

- C programming
- Memory management
- Defensive programming
- Software architecture
- Stream abstraction
- Readability analysis

---

# 📜 License

This project is open for educational and personal use.