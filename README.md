# FensterSQL

**FensterSQL** is a relational database engine written from scratch in **C** to explore the fundamental "nervous system" of database internals. The project focuses on low-level system interactions, moving away from high-level abstractions to implement core database components manually.

---

## 🛠 Project Architecture

The engine is designed to handle the full lifecycle of a SQL query, from parsing to disk-based storage:

* [cite_start]**SQL Parser:** Built using **Flex** and **Bison** to interpret and process Data Definition Language (DDL) and Data Manipulation Language (DML) queries[cite: 71, 75].
* [cite_start]**Storage Engine:** Implements a **page-based storage** system to manage how data is organized and accessed on disk[cite: 76].
* [cite_start]**Indexing:** Utilizes **B+ Tree indexing** for efficient data retrieval, ensuring performance even as data scales[cite: 76].
* [cite_start]**Metadata Management:** Includes a dedicated database catalog and directory structure for persistent storage of schemas and internal metadata[cite: 77].

---

## 🚀 Key Features

* **Written in C:** Developed entirely in C to maintain high performance and granular control over memory management.
* [cite_start]**Disk-Backed:** Designed for persistence, moving beyond in-memory structures to handle real-world storage constraints[cite: 74, 76].
* **Custom Shell:** Includes a basic SQL shell for interacting with the engine.

---

## ⚠️ Current Status: In-Progress

FensterSQL is an **ongoing educational project** and is currently **incomplete**. Many features are in active development:

* The SQL grammar and parser are being expanded to support more complex queries.
* The page-based storage and B+ Tree logic are undergoing continuous refinement for stability and performance.
* The system is currently intended for learning purposes and is not yet suitable for production workloads.

---

## 🛠 Technical Stack

* [cite_start]**Language:** C [cite: 71]
* [cite_start]**Tools:** Flex (Lexer), Bison (Parser) [cite: 71]
* [cite_start]**Concepts:** B+ Trees, Page Management, SQL Internal Architecture [cite: 64, 76]

---

## 📖 About the Author

[cite_start]Developed by **Zaid Yusuf**, a Senior Software Engineer with a focus on systems programming and a background in the International Mathematical Olympiad (IMO)[cite: 58, 61, 99]. FensterSQL is part of a broader journey into building database internals from the ground up.
