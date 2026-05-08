# Browser History Manager

A CLI-based Browser History Manager built in C++ as a Data Structures and Algorithms (DSA) project. This project simulates a browser's back/forward navigation using two stacks, and stores the full browsing history as a singly linked list with timestamps.

## Features

*   **Visit URL**: Push to back-stack, clear forward-stack, append to linked list with timestamp.
*   **Back/Forward Navigation**: Pop from one stack and push to the other.
*   **Display History**: Show full chronological history with timestamps.
*   **Search**: Linear search through visited URLs (case-insensitive).
*   **Delete & Undo**: Remove URLs from history (all occurrences) and undo deletions using a multi-level stack.
*   **Bookmarks**: Separate linked list to save favourite URLs.
*   **Most Visited**: Tracks frequency of visits and displays top 5 URLs.
*   **Sorting**: Sort history alphabetically, by visit count, or by most recent (using insertion sort).
*   **Persistence**: Automatically saves session to `browser_data.txt` and loads on startup.
*   **Rich CLI Interface**: Color-coded ANSI output with a visual navigation bar mimicking a real browser tab.

## Data Structures Used

*   **Generic `Stack<T>`**: Used for back-stack, forward-stack, and undo-stack operations.
*   **Singly Linked List**: Stores complete chronological browsing history and bookmarks.
*   **`unordered_map<string, int>`**: Tracks URL visit frequency.

## Getting Started

1.  Compile the C++ source code:
    ```bash
    g++ -std=c++11 browser_history.cpp -o browser_history
    ```
2.  Run the application:
    ```bash
    ./browser_history
    ```
3.  Follow the interactive on-screen menu to test navigation, bookmarks, sorting, and other features.
