# Browser History Manager

A C++ CLI-based application that simulates a web browser's back/forward navigation, history tracking, and bookmark management. It uses fundamental Data Structures and Algorithms (DSA) like Stacks, Linked Lists, and Hash Maps.

## 🚀 Features

### Core Navigation
- **Visit URL:** Pushes to the back-stack, clears the forward-stack, and appends to the history list with a timestamp.
- **Go Back / Forward:** Uses two generic stacks to simulate browser tab navigation.
- **Display History:** Shows full chronological browsing history using a singly linked list.
- **Search URL:** Case-insensitive linear search to find all occurrences of a visited URL.
- **Delete URL:** Removes a URL from the history.

### Custom Enhancements
- **Bookmarks:** Maintain a separate list of favorite URLs (even those not in your history).
- **Most Visited URLs (Top 5):** Tracks and displays the top 5 most frequently visited pages.
- **Undo Delete (Multi-level):** Restore deleted history entries across multiple levels using a stack.
- **Sort History:** Sorts history in 3 modes using Insertion Sort:
  - Alphabetically (A-Z)
  - By Visit Count
  - By Most Recent
- **File Persistence:** Automatically saves sessions to `browser_data.txt` on exit and loads on startup.
- **Premium Terminal UX:** ANSI color-coded CLI with a live navigation bar displaying the current page and back/forward stack status.

## 🛠️ Data Structures Used

| Structure | Purpose |
|-----------|---------|
| `Stack<T>` (Generic) | Used for back-stack, forward-stack, and undo-stack (capped at 50 for back-stack). |
| **Singly Linked List** | Stores full chronological browsing history with timestamps. |
| **Linked List** | Manages the separate list of bookmarked URLs. |
| `std::unordered_map` | Tracks visit frequency per URL for the most-visited feature. |

## 💻 How to Build and Run

1. **Clone the repository:**
   ```bash
   git clone https://github.com/kushal-sw/browser-cli-linkedlist.git
   cd browser-cli-linkedlist
   ```

2. **Compile the code:**
   Requires a C++ compiler (e.g., `g++`).
   ```bash
   g++ browser_history.cpp -o browser_history
   ```

3. **Run the executable:**
   ```bash
   ./browser_history
   ```

## 👥 Team / Roles

- **P1:** Stack<T> template, back-stack, forward-stack, undo-stack limits and logic.
- **P2:** Navigation logic (visit/back/forward), current page tracking, file persistence, and CLI navigation bar integration.
- **P3:** Linked lists implementation (history + bookmarks), timestamping, insert/delete/display.
- **P4:** Linear search, most-visited frequency map, insertion sorting, and core CLI menu integration.

---
*Developed as part of a DSA group project.*
