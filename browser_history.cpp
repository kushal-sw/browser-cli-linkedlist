#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// ==================== ANSI Colors ====================
namespace C {
const string R = "\033[0m", RED = "\033[1;31m", GRN = "\033[1;32m",
             YEL = "\033[1;33m";
const string BLU = "\033[1;34m", MAG = "\033[1;35m", CYN = "\033[1;36m",
             WHT = "\033[1;37m";
const string DIM = "\033[2m", BOLD = "\033[1m", UL = "\033[4m";
} // namespace C

// ==================== Utilities ====================
string toLower(const string &s) {
  string r = s;
  transform(r.begin(), r.end(), r.begin(), ::tolower);
  return r;
}

string getTimestamp() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
  return string(buf);
}

// ==================== URL Validation ====================
bool isUrlReachable(const string &url) {
  // Use the Mac's built-in curl command instead of an external library!
  // -s: silent, -I: fetch headers only (fast), -m 3: max 3 seconds timeout
  string command = "curl -s -I -m 3 http://" + url + " > /dev/null 2>&1";
  int result = system(command.c_str());
  
  // system() returns 0 if the curl command successfully reached the URL
  return (result == 0);
}

// ==================== History Entry ====================
struct HistoryEntry {
  string url;
  string timestamp;
  HistoryEntry() : url(""), timestamp("") {}
  HistoryEntry(const string &u, const string &t) : url(u), timestamp(t) {}
};

// =========================================================================
//            [ P1's ROLE: STACK DATA STRUCTURE & UNDO LOGIC ]
// Handles: back-stack, forward-stack, undo-stack, push, pop, peek operations
// =========================================================================
template <typename T> class Stack {
  struct Node {
    T data;
    Node *next;
    Node(const T &v) : data(v), next(nullptr) {}
  };
  Node *top;
  int cnt, maxSz;

  void removeBottom() {
    if (!top)
      return;
    if (!top->next) {
      delete top;
      top = nullptr;
      cnt--;
      return;
    }
    Node *c = top;
    while (c->next->next)
      c = c->next;
    delete c->next;
    c->next = nullptr;
    cnt--;
  }

public:
  Stack(int mx = -1) : top(nullptr), cnt(0), maxSz(mx) {}
  ~Stack() { clear(); }

  void push(const T &v) {
    if (maxSz > 0 && cnt >= maxSz)
      removeBottom();
    Node *n = new Node(v);
    n->next = top;
    top = n;
    cnt++;
  }

  T pop() {
    if (isEmpty())
      throw runtime_error("Stack is empty");
    Node *tmp = top;
    T v = tmp->data;
    top = top->next;
    delete tmp;
    cnt--;
    return v;
  }

  T peek() const {
    if (isEmpty())
      throw runtime_error("Stack is empty");
    return top->data;
  }

  bool isEmpty() const { return top == nullptr; }
  int size() const { return cnt; }

  void clear() {
    while (top) {
      Node *t = top;
      top = top->next;
      delete t;
      cnt--;
    }
  }

  vector<T> toVector() const {
    vector<T> r;
    Node *c = top;
    while (c) {
      r.push_back(c->data);
      c = c->next;
    }
    return r;
  }
};

// =========================================================================
//              [ P3's ROLE: LINKED LIST & BOOKMARKS CORE ]
// Handles: chronological history, bookmarks list, insert and traverse logic
// =========================================================================
class LinkedList {
  struct Node {
    HistoryEntry data;
    Node *next;
    Node(const HistoryEntry &d) : data(d), next(nullptr) {}
  };
  Node *head;
  int cnt;

public:
  LinkedList() : head(nullptr), cnt(0) {}
  ~LinkedList() {
    while (head) {
      Node *t = head;
      head = head->next;
      delete t;
    }
  }

  void insertAtEnd(const HistoryEntry &e) {
    Node *n = new Node(e);
    if (!head) {
      head = n;
    } else {
      Node *c = head;
      while (c->next)
        c = c->next;
      c->next = n;
    }
    cnt++;
  }

  // =====================================================================
  //                   [ P4's ROLE: SEARCH & DELETE ]
  // Handles: Linear search and full node deletion within the Linked List
  // =====================================================================
  vector<HistoryEntry> deleteByURL(const string &url) {
    vector<HistoryEntry> del;
    string tgt = toLower(url);
    while (head && toLower(head->data.url) == tgt) {
      Node *t = head;
      del.push_back(t->data);
      head = head->next;
      delete t;
      cnt--;
    }
    if (!head)
      return del;
    Node *c = head;
    while (c->next) {
      if (toLower(c->next->data.url) == tgt) {
        Node *t = c->next;
        del.push_back(t->data);
        c->next = t->next;
        delete t;
        cnt--;
      } else
        c = c->next;
    }
    return del;
  }

  vector<int> search(const string &url) const {
    vector<int> pos;
    string tgt = toLower(url);
    Node *c = head;
    int p = 1;
    while (c) {
      if (toLower(c->data.url) == tgt)
        pos.push_back(p);
      c = c->next;
      p++;
    }
    return pos;
  }

  bool contains(const string &url) const {
    Node *c = head;
    while (c) {
      if (toLower(c->data.url) == toLower(url))
        return true;
      c = c->next;
    }
    return false;
  }

  vector<HistoryEntry> toVector() const {
    vector<HistoryEntry> r;
    Node *c = head;
    while (c) {
      r.push_back(c->data);
      c = c->next;
    }
    return r;
  }

  int getCount() const { return cnt; }
  bool isEmpty() const { return head == nullptr; }
};

// =========================================================================
//                  [ P4's ROLE: SORTING ALGORITHMS ]
// Handles: Insertion sort logic across 3 different sorting modes
// =========================================================================
void insertionSortAZ(vector<HistoryEntry> &arr) {
  for (int i = 1; i < (int)arr.size(); i++) {
    HistoryEntry key = arr[i];
    int j = i - 1;
    while (j >= 0 && toLower(arr[j].url) > toLower(key.url)) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void insertionSortByCount(vector<HistoryEntry> &arr,
                          unordered_map<string, int> &freq) {
  for (int i = 1; i < (int)arr.size(); i++) {
    HistoryEntry key = arr[i];
    int j = i - 1;
    while (j >= 0 && freq[toLower(arr[j].url)] < freq[toLower(key.url)]) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void insertionSortByRecent(vector<HistoryEntry> &arr) {
  // Reverse chronological — most recent first (reverse the list)
  for (int i = 1; i < (int)arr.size(); i++) {
    HistoryEntry key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j].timestamp < key.timestamp) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

// ==================== Browser History Manager ====================
class BrowserHistoryManager {
  Stack<string> backStack;
  Stack<string> forwardStack;
  Stack<HistoryEntry> undoStack;
  LinkedList history;
  LinkedList bookmarks;
  unordered_map<string, int> frequency;
  string currentPage;
  static const int MAX_BACK = 50;
  const string DATA_FILE = "browser_data.txt";
  const string SESSION_DIR = "saved-history";
  vector<string> sessionLog;
  string sessionStartTime;

  void printOK(const string &m) {
    cout << C::GRN << "  ✓ " << m << C::R << endl;
  }
  void printErr(const string &m) {
    cout << C::RED << "  ✗ " << m << C::R << endl;
  }
  void printInfo(const string &m) {
    cout << C::CYN << "  ℹ " << m << C::R << endl;
  }

  void printLine() {
    cout << C::DIM << "  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈"
         << C::R << endl;
  }

  // =====================================================================
  //             [ P2's ROLE: UI RENDERING & PERSISTENCE ]
  // Handles: Browser navigation bar, CLI menu layout, session save/load
  // =====================================================================
  void displayNavBar() {
    cout << endl;
    cout << C::DIM
         << "  ╭──────────────────────────────────────────────────────────╮"
         << C::R << endl;

    cout << C::DIM << "  │ " << C::R;
    // Navigation buttons
    cout << (backStack.isEmpty() ? C::DIM : C::WHT) << "◀" << C::R << "  ";
    cout << (forwardStack.isEmpty() ? C::DIM : C::WHT) << "▶" << C::R << "  ";
    cout << C::WHT << "↻" << C::R << "  ";

    // URL Bar
    cout << C::DIM << "│ " << C::R;
    if (currentPage.empty()) {
      cout << C::DIM << " Search or enter address..." << string(26, ' ')
           << C::R;
    } else {
      string star =
          bookmarks.contains(currentPage) ? C::YEL + " 🔖" + C::R : "   ";
      string urlStr = "🔒 https://" + currentPage;
      int pad = max(0, 40 - (int)urlStr.length());
      cout << C::GRN << "🔒 " << C::R << C::WHT << "https://" << currentPage
           << C::R << string(pad, ' ') << star;
    }
    cout << C::DIM << " │" << C::R << endl;
    cout << C::DIM
         << "  ├──────────────────────────────────────────────────────────┤"
         << C::R << endl;

    // Visual tab bar (Breadcrumbs)
    cout << C::DIM << "  │ " << C::R;
    vector<string> back = backStack.toVector();
    vector<string> fwd = forwardStack.toVector();

    int visualLen = 0;

    // Show max 2 from back stack for clean look
    int num_back = min((int)back.size(), 2);
    if (back.size() > 2) {
      cout << C::DIM << "... → " << C::R;
      visualLen += 6;
    }
    for (int i = num_back - 1; i >= 0; i--) {
      cout << C::DIM << back[i] << " → " << C::R;
      visualLen += (int)back[i].length() + 3;
    }

    // Current
    if (!currentPage.empty()) {
      cout << C::GRN << C::BOLD << "● " << currentPage << " ●" << C::R;
      visualLen += (int)currentPage.length() + 4;
    } else {
      cout << C::DIM << "New Tab" << C::R;
      visualLen += 7;
    }

    // Show max 2 from forward stack
    int f_end = min((int)fwd.size(), 2);
    for (int i = 0; i < f_end; i++) {
      cout << C::DIM << " → " << fwd[i] << C::R;
      visualLen += (int)fwd[i].length() + 3;
    }
    if ((int)fwd.size() > 2) {
      cout << C::DIM << " → ..." << C::R;
      visualLen += 6;
    }

    int pad = max(0, 57 - visualLen);
    cout << string(pad, ' ') << C::DIM << "│" << C::R << endl;
    cout << C::DIM
         << "  ╰──────────────────────────────────────────────────────────╯"
         << C::R << endl;
  }

  void displayMenu() {
    cout << endl;
    cout << C::MAG << C::BOLD
         << "  ╭───────────────── BROWSER MENU ─────────────────╮" << C::R
         << endl;
    cout << C::WHT << "  │   " << C::CYN << "1." << C::WHT
         << " 🌐 Visit URL         " << C::CYN << "8." << C::WHT
         << " 🔖 Bookmark URL   │" << C::R << endl;
    cout << C::WHT << "  │   " << C::CYN << "2." << C::WHT
         << " ◀  Go Back           " << C::CYN << "9." << C::WHT
         << " 📂 View Bookmarks │" << C::R << endl;
    cout << C::WHT << "  │   " << C::CYN << "3." << C::WHT
         << " ▶  Go Forward       " << C::CYN << "10." << C::WHT
         << " 🗑️  Rm Bookmark    │" << C::R << endl;
    cout << C::WHT << "  │   " << C::CYN << "4." << C::WHT
         << " 🕒 History          " << C::CYN << "11." << C::WHT
         << " 📊 Most Visited   │" << C::R << endl;
    cout << C::WHT << "  │   " << C::CYN << "5." << C::WHT
         << " 🔍 Search           " << C::CYN << "12." << C::WHT
         << " 🗂️  Sort History   │" << C::R << endl;
    cout << C::WHT << "  │   " << C::CYN << "6." << C::WHT
         << " 🗑️  Delete URL       " << C::CYN << "13." << C::RED
         << " 🚪 Exit Session   │" << C::R << endl;
    cout << C::WHT << "  │   " << C::CYN << "7." << C::WHT
         << " ↩️  Undo Delete                         │" << C::R << endl;
    cout << C::MAG << C::BOLD
         << "  ╰────────────────────────────────────────────────╯" << C::R
         << endl;
    cout << C::YEL << "  ❯ Enter choice: " << C::R;
  }

  // Get next session ID by scanning saved-history folder
  int getNextSessionId() {
    if (!fs::exists(SESSION_DIR))
      return 1;
    int maxId = 0;
    for (auto &entry : fs::directory_iterator(SESSION_DIR)) {
      string fname = entry.path().filename().string();
      // Parse "id:N.txt" format
      if (fname.substr(0, 3) == "id:" && fname.size() > 7) {
        string numStr =
            fname.substr(3, fname.size() - 7); // strip "id:" and ".txt"
        try {
          int id = stoi(numStr);
          if (id > maxId)
            maxId = id;
        } catch (...) {
        }
      }
    }
    return maxId + 1;
  }

  void saveSessionLog() {
    if (sessionLog.empty())
      return;

    fs::create_directories(SESSION_DIR);
    int id = getNextSessionId();
    string filename = SESSION_DIR + "/id:" + to_string(id) + ".txt";

    ofstream f(filename);
    if (!f.is_open()) {
      printErr("Could not save session log");
      return;
    }

    f << "============================================" << endl;
    f << "  SESSION LOG #" << id << endl;
    f << "  Started:  " << sessionStartTime << endl;
    f << "  Ended:    " << getTimestamp() << endl;
    f << "  Actions:  " << sessionLog.size() << endl;
    f << "============================================" << endl << endl;

    for (int i = 0; i < (int)sessionLog.size(); i++) {
      f << "  [" << (i + 1) << "] " << sessionLog[i] << endl;
    }

    f << endl << "============================================" << endl;
    f << "  END OF SESSION #" << id << endl;
    f << "============================================" << endl;
    f.close();

    printOK("Session log saved to " + filename);
  }

  void logAction(const string &action) {
    sessionLog.push_back("[" + getTimestamp() + "] " + action);
  }

public:
  BrowserHistoryManager() : backStack(MAX_BACK) {
    sessionStartTime = getTimestamp();
    loadFromFile();
    logAction("Session started");
  }

  // =====================================================================
  //               [ P2's ROLE: CORE NAVIGATION LOGIC ]
  // Handles: Visiting new URLs, Go Back, Go Forward, clearing forward stack
  // =====================================================================
  void visitURL() {
    string url;
    cout << C::CYN << "  Enter URL: " << C::R;
    cin >> url;
    url = toLower(url);

    cout << C::DIM << "  Validating URL..." << C::R << endl;
    if (!isUrlReachable(url)) {
      printErr("URL is unreachable or does not exist.");
      logAction("Failed visit attempt (unreachable): " + url);
      return;
    }

    printOK("Successfully loaded: " + url);

    if (!currentPage.empty())
      backStack.push(currentPage);
    forwardStack.clear();
    currentPage = url;

    string ts = getTimestamp();
    history.insertAtEnd(HistoryEntry(url, ts));
    frequency[url]++;

    logAction("Visited URL: " + url);
  }

  void goBack() {
    if (backStack.isEmpty()) {
      printErr("Nothing to go back to");
      return;
    }
    forwardStack.push(currentPage);
    currentPage = backStack.pop();
    printOK("Navigated back to: " + currentPage);
    logAction("Navigated back to: " + currentPage);
  }

  void goForward() {
    if (forwardStack.isEmpty()) {
      printErr("Nothing to go forward to");
      return;
    }
    backStack.push(currentPage);
    currentPage = forwardStack.pop();
    printOK("Navigated forward to: " + currentPage);
    logAction("Navigated forward to: " + currentPage);
  }

  // =====================================================================
  //             [ P3's ROLE: HISTORY & BOOKMARK UI ]
  // Handles: Displaying chronological history and managing bookmarks
  // =====================================================================
  void displayHistory() {
    cout << endl
         << C::BOLD << C::WHT << "  🕒 Full Browsing History ──" << C::R
         << endl;
    if (history.isEmpty()) {
      cout << C::DIM << "  (empty)" << C::R << endl;
    } else {
      vector<HistoryEntry> arr = history.toVector();
      for (int i = 0; i < (int)arr.size(); i++) {
        string star =
            bookmarks.contains(arr[i].url) ? C::YEL + " 🔖" + C::R : "";
        cout << C::DIM << "  " << setw(2) << (i + 1) << ". " << C::R << C::CYN
             << arr[i].url << C::R << star << C::DIM << "  @  "
             << arr[i].timestamp << C::R << endl;
      }
    }
    cout << C::DIM << "  Total entries: " << history.getCount() << C::R << endl;
    logAction("Displayed history (" + to_string(history.getCount()) +
              " entries)");
  }

  // =====================================================================
  //                 [ P4's ROLE: ADVANCED FEATURES ]
  // Handles: Searching, deleting URLs, frequency maps, and sorting modes
  // =====================================================================
  void searchURL() {
    if (history.isEmpty()) {
      printErr("History is empty");
      return;
    }
    string url;
    cout << C::CYN << "  Enter URL to search: " << C::R;
    cin >> url;
    url = toLower(url);

    vector<int> pos = history.search(url);
    if (pos.empty()) {
      printErr("URL not found in history");
      logAction("Searched for \"" + url + "\" — not found");
    } else {
      printOK("Found \"" + url + "\" at position(s): ");
      for (int p : pos)
        cout << C::GRN << "    #" << p << C::R << endl;
      logAction("Searched for \"" + url + "\" — found at " +
                to_string(pos.size()) + " position(s)");
    }
  }

  void deleteURL() {
    if (history.isEmpty()) {
      printErr("History is empty");
      return;
    }
    string url;
    cout << C::CYN << "  Enter URL to delete: " << C::R;
    cin >> url;
    url = toLower(url);

    vector<HistoryEntry> deleted = history.deleteByURL(url);
    if (deleted.empty()) {
      printErr("URL not found in history");
      logAction("Tried to delete \"" + url + "\" — not found");
    } else {
      for (auto &e : deleted)
        undoStack.push(e);
      printOK("Deleted " + to_string(deleted.size()) + " occurrence(s) of \"" +
              url + "\"");
      printInfo("You can undo up to " + to_string(undoStack.size()) +
                " deletion(s)");
      logAction("Deleted " + to_string(deleted.size()) +
                " occurrence(s) of \"" + url + "\"");
    }
  }

  // =====================================================================
  //                 [ P1's ROLE: UNDO DELETE LOGIC ]
  // Handles: Stack-based restore of previously deleted URLs
  // =====================================================================
  void undoDelete() {
    if (undoStack.isEmpty()) {
      printErr("Nothing to undo");
      return;
    }
    int avail = undoStack.size();
    printInfo("Undo stack has " + to_string(avail) + " entry(ies)");

    int levels;
    cout << C::CYN << "  How many to restore? (1-" << avail << "): " << C::R;
    cin >> levels;
    if (levels < 1 || levels > avail) {
      printErr("Invalid number");
      return;
    }

    for (int i = 0; i < levels; i++) {
      HistoryEntry e = undoStack.pop();
      history.insertAtEnd(e);
      cout << C::GRN << "    Restored: " << e.url << " (" << e.timestamp << ")"
           << C::R << endl;
    }
    printOK(to_string(levels) + " entry(ies) restored");
    logAction("Undo delete: restored " + to_string(levels) + " entry(ies)");
  }

  void bookmarkURL() {
    string url;
    cout << C::CYN << "  Enter URL to bookmark: " << C::R;
    cin >> url;
    url = toLower(url);

    if (bookmarks.contains(url)) {
      printErr("Already bookmarked");
      return;
    }

    string ts = getTimestamp();
    bookmarks.insertAtEnd(HistoryEntry(url, ts));
    printOK("Bookmarked: " + url);
    logAction("Bookmarked: " + url);
  }

  void viewBookmarks() {
    cout << endl << C::BOLD << C::YEL << "  🔖 ★ Bookmarks ──" << C::R << endl;
    if (bookmarks.isEmpty()) {
      cout << C::DIM << "  (empty)" << C::R << endl;
    } else {
      vector<HistoryEntry> arr = bookmarks.toVector();
      for (int i = 0; i < (int)arr.size(); i++) {
        cout << C::DIM << "  " << setw(2) << (i + 1) << ". " << C::R << C::YEL
             << arr[i].url << " ★" << C::R << C::DIM << "  @  "
             << arr[i].timestamp << C::R << endl;
      }
    }
    cout << C::DIM << "  Total: " << bookmarks.getCount() << C::R << endl;
    logAction("Viewed bookmarks (" + to_string(bookmarks.getCount()) +
              " total)");
  }

  void removeBookmark() {
    if (bookmarks.isEmpty()) {
      printErr("No bookmarks to remove");
      return;
    }
    string url;
    cout << C::CYN << "  Enter URL to remove from bookmarks: " << C::R;
    cin >> url;
    url = toLower(url);

    vector<HistoryEntry> del = bookmarks.deleteByURL(url);
    if (del.empty()) {
      printErr("Bookmark not found");
      logAction("Tried to remove bookmark \"" + url + "\" — not found");
    } else {
      printOK("Removed bookmark: " + url);
      logAction("Removed bookmark: " + url);
    }
  }

  void mostVisited() {
    if (frequency.empty()) {
      printErr("No visit data available");
      return;
    }

    // Copy to vector of pairs
    vector<pair<string, int>> freqVec(frequency.begin(), frequency.end());

    // Insertion sort descending by count (stable — preserves insertion order
    // for ties)
    for (int i = 1; i < (int)freqVec.size(); i++) {
      auto key = freqVec[i];
      int j = i - 1;
      while (j >= 0 && freqVec[j].second < key.second) {
        freqVec[j + 1] = freqVec[j];
        j--;
      }
      freqVec[j + 1] = key;
    }

    cout << endl
         << C::BOLD << C::WHT << "  📊 Top 5 Most Visited ──" << C::R << endl;
    int limit = min(5, (int)freqVec.size());
    for (int i = 0; i < limit; i++) {
      string star =
          bookmarks.contains(freqVec[i].first) ? C::YEL + " 🔖" + C::R : "";
      cout << C::GRN << "  " << setw(2) << (i + 1) << ". " << C::CYN
           << freqVec[i].first << C::R << star << C::YEL << "  ("
           << freqVec[i].second << " visits)" << C::R << endl;
    }
    logAction("Viewed most visited URLs (top " + to_string(limit) + ")");
  }

  void sortHistory() {
    if (history.isEmpty()) {
      printErr("History is empty");
      return;
    }

    cout << C::YEL << "  Sort by:" << C::R << endl;
    cout << "    1. Alphabetical (A-Z)" << endl;
    cout << "    2. Most Visited" << endl;
    cout << "    3. Most Recent" << endl;
    cout << C::CYN << "  Choice: " << C::R;

    int ch;
    cin >> ch;
    vector<HistoryEntry> arr = history.toVector();

    switch (ch) {
    case 1:
      insertionSortAZ(arr);
      cout << endl << C::BOLD << "  ── Sorted A-Z ──" << C::R << endl;
      break;
    case 2:
      insertionSortByCount(arr, frequency);
      cout << endl
           << C::BOLD << "  ── Sorted by Visit Count ──" << C::R << endl;
      break;
    case 3:
      insertionSortByRecent(arr);
      cout << endl
           << C::BOLD << "  ── Sorted by Most Recent ──" << C::R << endl;
      break;
    default:
      printErr("Invalid choice");
      return;
    }

    string modes[] = {"", "A-Z", "visit count", "most recent"};
    for (int i = 0; i < (int)arr.size(); i++) {
      string star = bookmarks.contains(arr[i].url) ? C::YEL + " 🔖" + C::R : "";
      cout << C::DIM << "  " << setw(2) << (i + 1) << ". " << C::R << C::CYN
           << arr[i].url << C::R << star;
      if (ch == 2)
        cout << C::YEL << "  (" << frequency[toLower(arr[i].url)] << " visits)"
             << C::R;
      cout << C::DIM << "  @  " << arr[i].timestamp << C::R << endl;
    }
    logAction("Sorted history by " + modes[ch]);
  }

  void saveToFile() {
    ofstream f(DATA_FILE);
    if (!f.is_open()) {
      printErr("Could not save session");
      return;
    }

    f << "#CURRENT" << endl << currentPage << endl;

    f << "#HISTORY" << endl;
    vector<HistoryEntry> h = history.toVector();
    for (auto &e : h)
      f << e.url << "|" << e.timestamp << endl;

    f << "#BOOKMARKS" << endl;
    vector<HistoryEntry> b = bookmarks.toVector();
    for (auto &e : b)
      f << e.url << "|" << e.timestamp << endl;

    f << "#FREQUENCY" << endl;
    for (auto &p : frequency)
      f << p.first << "|" << p.second << endl;

    f << "#BACKSTACK" << endl;
    vector<string> bs = backStack.toVector();
    for (int i = (int)bs.size() - 1; i >= 0; i--)
      f << bs[i] << endl;

    f << "#FORWARDSTACK" << endl;
    vector<string> fs = forwardStack.toVector();
    for (int i = (int)fs.size() - 1; i >= 0; i--)
      f << fs[i] << endl;

    f << "#UNDOSTACK" << endl;
    vector<HistoryEntry> u = undoStack.toVector();
    for (int i = (int)u.size() - 1; i >= 0; i--)
      f << u[i].url << "|" << u[i].timestamp << endl;

    f << "#END" << endl;
    f.close();
  }

  void loadFromFile() {
    ifstream f(DATA_FILE);
    if (!f.is_open())
      return; // Fresh start

    string line, section;
    while (getline(f, line)) {
      if (line[0] == '#') {
        section = line;
        continue;
      }
      if (line.empty())
        continue;

      if (section == "#CURRENT") {
        currentPage = line;
      } else if (section == "#HISTORY") {
        size_t sep = line.find('|');
        if (sep != string::npos)
          history.insertAtEnd(
              HistoryEntry(line.substr(0, sep), line.substr(sep + 1)));
      } else if (section == "#BOOKMARKS") {
        size_t sep = line.find('|');
        if (sep != string::npos)
          bookmarks.insertAtEnd(
              HistoryEntry(line.substr(0, sep), line.substr(sep + 1)));
      } else if (section == "#FREQUENCY") {
        size_t sep = line.find('|');
        if (sep != string::npos)
          frequency[line.substr(0, sep)] = stoi(line.substr(sep + 1));
      } else if (section == "#BACKSTACK") {
        backStack.push(line);
      } else if (section == "#FORWARDSTACK") {
        forwardStack.push(line);
      } else if (section == "#UNDOSTACK") {
        size_t sep = line.find('|');
        if (sep != string::npos)
          undoStack.push(
              HistoryEntry(line.substr(0, sep), line.substr(sep + 1)));
      }
    }
    f.close();
    if (!currentPage.empty())
      printOK("Session restored from " + DATA_FILE);
  }

  void run() {
    int choice;
    while (true) {
      displayNavBar();
      displayMenu();

      if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(10000, '\n');
        printErr("Invalid input. Enter a number 1-13.");
        continue;
      }

      printLine();
      switch (choice) {
      case 1:
        visitURL();
        break;
      case 2:
        goBack();
        break;
      case 3:
        goForward();
        break;
      case 4:
        displayHistory();
        break;
      case 5:
        searchURL();
        break;
      case 6:
        deleteURL();
        break;
      case 7:
        undoDelete();
        break;
      case 8:
        bookmarkURL();
        break;
      case 9:
        viewBookmarks();
        break;
      case 10:
        removeBookmark();
        break;
      case 11:
        mostVisited();
        break;
      case 12:
        sortHistory();
        break;
      case 13:
        logAction("Session ended by user");
        saveToFile();
        saveSessionLog();
        printOK("Session saved. Goodbye!");
        return;
      default:
        printErr("Invalid choice. Try 1-13.");
      }
      printLine();
    }
  }
};

// ==================== Startup Banner ====================
void showStartupBanner() {
  vector<string> banner = {
      "    ____  ____  ____ _       _______  __________",
      "   / __ )/ __ \\/ __ \\ |     / / ___/ / ____/ __ \\",
      "  / __  / /_/ / / / / | /| / /\\__ \\ / __/ / /_/ /",
      " / /_/ / _, _/ /_/ /| |/ |/ /___/ // /___/ _, _/ ",
      "/_____/_/ |_|\\____/ |__/|__//____//_____/_/ |_|  "
  };
  cout << "\n" << C::CYN << C::BOLD;
  for (const auto& line : banner) {
      cout << line << endl;
  }
  cout << C::R << "\n\n";
}

// ==================== Main ====================
int main() {
  showStartupBanner();
  BrowserHistoryManager browser;
  browser.run();
  return 0;
}

/*
 * ==================== DEMO SCRIPT ====================
 * Use this sequence to showcase all features in a presentation:
 *
 *  1.  Choose 1  → Visit "google.com"
 *  2.  Choose 1  → Visit "github.com"
 *  3.  Choose 1  → Visit "stackoverflow.com"
 *  4.  Choose 2  → Go Back (now on github.com)
 *  5.  Choose 2  → Go Back (now on google.com)
 *  6.  Choose 3  → Go Forward (now on github.com)
 *  7.  Choose 1  → Visit "youtube.com" (forward-stack cleared!)
 *  8.  Choose 1  → Visit "google.com" (2nd visit)
 *  9.  Choose 4  → Display History (5 entries with timestamps)
 * 10.  Choose 5  → Search "google.com" (found at positions 1 & 5)
 * 11.  Choose 11 → Most Visited (google.com: 2 visits at top)
 * 12.  Choose 8  → Bookmark "github.com"
 * 13.  Choose 8  → Bookmark "reddit.com" (arbitrary URL)
 * 14.  Choose 9  → View Bookmarks (2 bookmarks)
 * 15.  Choose 6  → Delete "google.com" (removes 2 entries)
 * 16.  Choose 4  → Display History (google.com gone)
 * 17.  Choose 7  → Undo Delete (restore 2 levels)
 * 18.  Choose 4  → Display History (google.com back)
 * 19.  Choose 12 → Sort History (choose 1: A-Z)
 * 20.  Choose 12 → Sort History (choose 2: by count)
 * 21.  Choose 12 → Sort History (choose 3: by recent)
 * 22.  Choose 10 → Remove Bookmark "reddit.com"
 * 23.  Choose 13 → Exit (saves to browser_data.txt)
 * 24.  Re-run program → Data loaded from file!
 */
