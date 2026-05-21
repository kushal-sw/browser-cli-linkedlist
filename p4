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
    printSuccess(m);
  }
  void printErr(const string &m) {
    printError(m);
  }
  void printInfo(const string &m) {
    ::printInfo(m);
  }

  void printLine() {
    cout << C::DIM << "  ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈"
         << C::R << endl;
  }

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