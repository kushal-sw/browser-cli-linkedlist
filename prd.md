# 08 Browser History Manager — PRD & Build Plan (v2)

Language: C++ | Interface: CLI / Terminal | Team: 4 members | Phases: 4
Architecture: Single `.cpp` file | Generic Template Stack

---

## Problem Statement
Simulate a browser's back/forward navigation using two stacks. Each URL visited is pushed to the back-stack. Back pops to forward-stack. Forward pushes back. History is stored as a singly linked list with timestamps.

---

## Design Decisions

| # | Decision | Details |
|---|----------|---------|
| 1 | Single file | All code in one `browser_history.cpp` |
| 2 | Generic Stack | `Stack<T>` template class (works with `string`, `HistoryEntry`, etc.) |
| 3 | History limit | Back-stack capped at 50 entries (oldest removed when full) |
| 4 | Timestamps | Every URL visit records a timestamp (`YYYY-MM-DD HH:MM:SS`) |
| 5 | Most Visited | Top 5 by frequency; ties broken by visit order (not alphabetical) |
| 6 | Bookmarks | User can bookmark ANY URL (from history or arbitrary) |
| 7 | Undo Delete | Multi-level undo — user chooses how many levels to restore |
| 8 | Current Page | Always displayed above the menu like a browser address bar |
| 9 | Colors | ANSI color-coded output (red=error, green=success, yellow=bookmark, cyan=URL) |
| 10 | Navigation Bar | Visual back/forward stack display like a real browser tab bar |
| 11 | Persistence | Save state to `browser_data.txt` on exit; load on startup |
| 12 | Duplicates | All visits kept with individual timestamps (google.com visited 3× = 3 entries) |
| 13 | Case Sensitivity | Case-insensitive — `Google.com` and `google.com` treated as same |
| 14 | Demo Script | Pre-planned demo sequence included for presentation |
| 15 | Sort Options | Three modes: A-Z, by visit count, by most recent — all using insertion sort |

---

## Core Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | Visit URL | Push to back-stack, clear forward-stack, append to linked list with timestamp, increment frequency |
| 2 | Back | Pop from back-stack, push current to forward-stack, update current page |
| 3 | Forward | Pop from forward-stack, push current to back-stack, update current page |
| 4 | Display History | Show full history as singly linked list (chronological) with timestamps |
| 5 | Linear Search | Search visited URLs using linear search (case-insensitive) |
| 6 | Delete URL | Remove URL from history (all occurrences), push to undo-stack |

## Custom Features (Group's Additions)

| # | Feature | DSA Concept |
|---|---------|-------------|
| 1 | Bookmark URLs | Separate linked list of favourite URLs (any URL allowed) |
| 2 | Most Visited URLs (Top 5) | Frequency counter using `unordered_map<string, int>` |
| 3 | Undo Delete (Multi-level) | Stack-based restore; user picks how many levels |
| 4 | Sort History (3 modes) | Insertion sort: A-Z, by visit count, by most recent |
| 5 | File Persistence | Save/load state to `browser_data.txt` |
| 6 | Navigation Bar | Visual display of back/forward stacks + current page |
| 7 | Color-coded CLI | ANSI escape codes for premium terminal UX |

---

## Data Structures Used

| Structure | Purpose |
|-----------|---------|
| `Stack<T>` (template) | Generic stack — used for back-stack, forward-stack, undo-stack |
| Singly Linked List | Stores full browsing history (chronological, with timestamps) |
| Linked List (bookmarks) | Separate list for bookmarked URLs with add/view/delete |
| `unordered_map<string,int>` | Tracks visit frequency per URL for most-visited feature |

---

## Role Division

| Member | Responsibilities |
|--------|-----------------|
| P1 | Stack<T> template — push/pop/peek/isEmpty/clear/toVector, back-stack, forward-stack, undo-stack |
| P2 | Visit / Back / Forward logic — navigation functions, clear forward on new visit, current page tracking |
| P3 | Linked list (history + bookmarks) — insert, traverse, display, deletion, bookmark list |
| P4 | Search + delete + frequency + sort — linear search, delete URL, most-visited map, insertion sort (3 modes) |

---

## Build Plan — 4 Phases

### Phase 1 — Foundation: Data Structures
P1 + P3 work in parallel

- [P1] Define `Stack<T>` template class with push(), pop(), peek(), isEmpty(), clear(), toVector(), size()
- [P1] Implement max-size limit with removeBottom() for back-stack (cap: 50)
- [P1] Instantiate back-stack (`Stack<string>`), forward-stack (`Stack<string>`), undo-stack (`Stack<HistoryEntry>`)
- [P3] Define `HistoryEntry` struct (url + timestamp)
- [P3] Define Node struct and LinkedList class
- [P3] Implement insertAtEnd(), display() (with timestamps + colors), deleteByURL() (all occurrences)

### Phase 2 — Core Navigation Logic
P2 integrates output from P1 + P3

- [P2] Implement visitURL() — push to back-stack, clear forward-stack, append to linked list, record timestamp, increment frequency
- [P2] Implement goBack() — pop from back-stack, push current to forward-stack
- [P2] Implement goForward() — pop from forward-stack, push current to back-stack
- [P2] Implement displayNavigationBar() — visual back/forward/current display
- [P2] Build main CLI menu loop with color-coded output and current page display

### Phase 3 — Search, Delete & Bookmarks
P3 + P4 work in parallel

- [P4] Linear search over linked list — return positions + found/not-found message (case-insensitive)
- [P4] Delete URL from linked list (all occurrences, push each to undo-stack)
- [P1] Multi-level undo delete — user chooses how many levels to restore from undo-stack
- [P3] Bookmark linked list — addBookmark(), viewBookmarks(), removeBookmark() (any URL allowed)

### Phase 4 — Custom Features + Integration
P4 leads, all members integrate and test

- [P4] Most visited — use `unordered_map<string,int>`, display top 5, ties by visit order
- [P4] Sort history — copy to vector, apply insertion sort (3 modes: A-Z, by count, by recent), display
- [P2] File persistence — saveToFile() on exit, loadFromFile() on startup (`browser_data.txt`)
- [All] Full CLI integration — wire all features into menu with ANSI colors
- [All] Edge case testing — empty stacks, duplicate URLs, deleting non-existent URL, case sensitivity

---

## CLI Menu Structure

```
╔══════════════════════════════════════════════╗
║         BROWSER HISTORY MANAGER              ║
╠══════════════════════════════════════════════╣
║                                              ║
║  ◀ [3]  │  📍 current-page.com  │  ▶ [1]    ║
║  ← page1 → page2 → page3 → ● HERE ● → pg4  ║
║                                              ║
╠══════════════════════════════════════════════╣
║  1.  Visit URL                               ║
║  2.  Go Back                                 ║
║  3.  Go Forward                              ║
║  4.  Display History                         ║
║  5.  Search URL                              ║
║  6.  Delete URL                              ║
║  7.  Undo Delete                             ║
║  8.  Bookmark URL                            ║
║  9.  View Bookmarks                          ║
║  10. Remove Bookmark                         ║
║  11. Most Visited URLs (Top 5)               ║
║  12. Sort History                            ║
║  13. Exit (saves session)                    ║
╚══════════════════════════════════════════════╝
```

---

## Edge Cases to Handle

- Back/Forward on empty stack → print "Nothing to go back/forward to"
- Delete a URL that doesn't exist → print "URL not found"
- Undo delete on empty undo-stack → print "Nothing to undo"
- Search on empty history → print "History is empty"
- Visiting a new URL should always clear the forward-stack
- Case-insensitive matching for all URL operations
- Back-stack overflow (>50) → remove oldest entry automatically
- Empty history for sort/display → show appropriate message
- Bookmark duplicate → warn "Already bookmarked"
- Load missing data file → start fresh silently

---

## Demo Script (Presentation Sequence)

```
1.  Visit "google.com"         → shows current page
2.  Visit "github.com"         → back-stack grows
3.  Visit "stackoverflow.com"  → 3 pages in back-stack
4.  Go Back                    → now on github.com, forward has stackoverflow
5.  Go Back                    → now on google.com
6.  Go Forward                 → now on github.com
7.  Visit "youtube.com"        → forward-stack cleared!
8.  Visit "google.com"         → duplicate visit (2nd time)
9.  Display History            → show all entries with timestamps
10. Search "google.com"        → found at positions 1 and 5
11. Most Visited               → google.com (2), others (1)
12. Bookmark "github.com"      → added to bookmarks
13. Bookmark "reddit.com"      → bookmark arbitrary URL (not in history)
14. View Bookmarks             → shows both
15. Delete "google.com"        → removes all occurrences, pushed to undo
16. Display History            → google.com entries gone
17. Undo Delete (2 levels)     → both google.com entries restored
18. Sort History (A-Z)         → alphabetical view
19. Sort History (by count)    → frequency-sorted view
20. Exit                       → saves to browser_data.txt
21. Restart program            → data loaded from file!
```

---

Project: PS08 | Team of 4 | C++ CLI DSA Project
