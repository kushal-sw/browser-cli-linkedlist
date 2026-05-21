# 🌐 Browser History & Bookmark Manager CLI

A premium, interactive Command-Line Interface (CLI) application built in C++ to simulate modern browser tab navigation, chronological history, bookmark management, search operations, and session persistence. 

This project demonstrates core Data Structures & Algorithms (DSA) including template **Stacks**, **Linked Lists**, and custom stable **Insertion Sort** algorithms.

<img width="2202" height="2368" alt="cli" src="https://github.com/user-attachments/assets/e790bcb0-174e-4cb0-aab9-53fb0fc5f1ac" />

---

## ✨ Features

- **🌐 Live URL Validation**: Leverages the native Mac `curl` CLI via standard subprocess execution to check if URLs are reachable before navigating to them.
- **🧭 Multi-Stack Navigation**:
  - `backStack`: Tracks past visited URLs for backwards navigation.
  - `forwardStack`: Restores next URLs on forward navigation (automatically clears when visiting a new URL).
- **📋 Chronological History & Bookmarks**: Custom linked-lists manage chronological browsing events and bookmark nodes.
- **🔍 Linear Search & Deletion**: Search occurrences of any URL in history, or execute complete node deletion with full multi-level undo support.
- **↩️ Multi-Level Undo Log**: A template `undoStack` enables multi-level restoration of deleted history items.
- **📊 Most Visited Statistics**: An integrated frequency counter displays top visited pages ranked in order.
- **🗂️ 3 Stable Insertion Sort Modes**:
  - **A-Z**: Alphabetical sorting of visited URLs.
  - **Most Visited**: Sorting history based on visit frequencies.
  - **Most Recent**: Reverse chronological order.
- **💾 Session Persistence**: Automatically restores the exact state from `browser_data.txt` at launch and exports complete session history logs under the `saved-history/` folder.

---

## 🛠️ Data Structures & Algorithms (DSA)

### 1. Template Stack (`Stack<T>`)
* **Logic**: Linked-node structure with a customized bottom-removal routine (`removeBottom()`) to enforce size limits (e.g., maximum history capacity).
* **Use Cases**:
  - `backStack` (Navigation history)
  - `forwardStack` (Forward navigation restoration)
  - `undoStack` (Undo log for deletions)

### 2. Singly Linked List (`LinkedList`)
* **Logic**: Custom singly-linked list containing dynamic historical/bookmark record structures.
* **Operations**: Chronological node append, linear search (`search()`), and exact node matching deletion (`deleteByURL()`).

### 3. Stable Insertion Sort
* **A-Z Sorting**: Stable sorting by comparing alphabetical URLs.
* **Most Visited**: Sorting nodes by frequency index mapping.
* **Most Recent**: Sorting by ISO-8601 formatting timestamp comparison.

---

## 🚀 Getting Started

### Prerequisites
Make sure you are on macOS and have `clang++` or `g++` installed.

### Compilation
Compile the project using standard C++17 configuration:
```bash
clang++ -std=c++17 -Wall -Wextra browser_history.cpp -o browser_history
```

### Run
Execute the compiled binary:
```bash
./browser_history
```

---

## 📖 Presentation Demo Walkthrough

Try the following sequence in the interactive menu to showcase all features:

1. **Option 1**: Visit `google.com` (Validates and updates the interface).
2. **Option 1**: Visit `github.com`.
3. **Option 1**: Visit `stackoverflow.com`.
4. **Option 2**: Go back (Back to `github.com`).
5. **Option 2**: Go back (Back to `google.com`).
6. **Option 3**: Go forward (Forward to `github.com`).
7. **Option 1**: Visit `youtube.com` (Notice that the forward-stack is now cleared!).
8. **Option 1**: Visit `google.com` (Second visit).
9. **Option 4**: Display History (Shows all 5 entries with timestamps).
10. **Option 5**: Search `google.com` (Informs you it's at positions #1 and #5).
11. **Option 11**: Most Visited (Shows `google.com` at the top with 2 visits).
12. **Option 8**: Bookmark `github.com`.
13. **Option 8**: Bookmark `reddit.com`.
14. **Option 9**: View Bookmarks.
15. **Option 6**: Delete `google.com` (Deletes both entries).
16. **Option 4**: Display History (Confirm `google.com` is removed).
17. **Option 7**: Undo Delete (Restore 2 levels).
18. **Option 4**: Display History (Confirm `google.com` is restored!).
19. **Option 12**: Sort History (Choose `1` for A-Z, `2` for Most Visited, or `3` for Recent).
20. **Option 10**: Remove Bookmark `reddit.com`.
21. **Option 13**: Exit Session (Saves to file, writes to `saved-history/` and quits).

---

## 📁 File Structure

- `browser_history.cpp`: Primary application code housing logic for `Stack`, `LinkedList`, sorting algorithms, UI layouts, and menus.
- `browser_data.txt`: Generated database file for recovering state across restarts.
- `saved-history/`: Target output folder containing formatted text logs of all individual session actions.
