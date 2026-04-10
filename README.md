# FensterSQL

**FensterSQL** is a relational database engine written from scratch in **C**. The project is a deep dive into database internals, focusing on low-level system interactions, manual memory management, and disk-based storage.

---

## 🛠 Project Architecture

The engine is designed to handle the full lifecycle of a SQL query, from parsing to persistent storage:

* **SQL Parser:** Built using **Flex** and **Bison** to interpret and process Data Definition Language (DDL) and Data Manipulation Language (DML) queries.
* **Storage Engine:** Implements a **page-based storage** system to manage data organization and disk access.
* **Indexing:** Utilizes **B+ Tree indexing** for efficient data retrieval.
* **Metadata Management:** Features a dedicated database catalog and directory structure for persistent metadata storage.

---

## 🚀 Key Features

* **Implementation from Scratch:** Developed entirely in C to maintain high performance and granular control over the "nervous system" of the software.
* **Disk-Backed Persistence:** Moves beyond in-memory structures to handle real-world storage and binary file I/O.
* **Custom Shell:** Includes a basic SQL shell for direct interaction with the engine.

---

## ⚠️ Current Status: In-Progress

FensterSQL is an **ongoing development project** and is currently **incomplete**. Key areas of active work include:

* Expanding the SQL grammar and parser support.
* Refining the B+ Tree logic and page-management stability.
* Improving error handling and query execution planning.

---

## 🛠 Technical Stack

* **Language:** C
* **Tools:** Flex (Lexer), Bison (Parser)
* **Concepts:** B+ Trees, Page Management, SQL Internal Architecture, Binary I/O.

---

## 📖 About

Developed by **Zaid Yusuf**, a Software Engineer interested in systems programming and database internals. This project is a continuation of the exploration started with **MiniKDB**, aimed at mastering low-level C development.
