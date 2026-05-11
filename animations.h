#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

// ANSI Constants
#define A_R "\033[0m"
#define A_RED "\033[1;31m"
#define A_GRN "\033[1;32m"
#define A_YEL "\033[1;33m"
#define A_BLU "\033[1;34m"
#define A_MAG "\033[1;35m"
#define A_CYN "\033[1;36m"
#define A_WHT "\033[1;37m"
#define A_DIM "\033[2m"
#define A_BOLD "\033[1m"
#define A_UL "\033[4m"
#define A_CLEAR_LINE "\033[2K\r"

inline void printSuccess(const string& msg) {
    cout << A_GRN << "  ✓ " << msg << A_R << endl;
}

inline void printError(const string& msg) {
    cout << A_RED << "  ✗ " << msg << A_R << endl;
}

inline void printInfo(const string& msg) {
    cout << A_CYN << "  ℹ " << msg << A_R << endl;
}

inline void printWarning(const string& msg) {
    cout << A_YEL << "  ⚠ " << msg << A_R << endl;
}

inline void showStartupBanner() {
    vector<string> banner = {
        "    ____  ____  ____ _       _______  __________",
        "   / __ )/ __ \\/ __ \\ |     / / ___/ / ____/ __ \\",
        "  / __  / /_/ / / / / | /| / /\\__ \\ / __/ / /_/ /",
        " / /_/ / _, _/ /_/ /| |/ |/ /___/ // /___/ _, _/ ",
        "/_____/_/ |_|\\____/ |__/|__//____//_____/_/ |_|  "
    };
    cout << "\n" << A_CYN << A_BOLD;
    for (const auto& line : banner) {
        cout << line << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << A_R << "\n\n";
}

inline void showSpinner(int durationMs) {
    vector<string> frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int frameDelay = 100;
    int elapsed = 0;
    int i = 0;
    while (elapsed < durationMs) {
        cout << A_CLEAR_LINE << A_YEL << "  " << frames[i] << " Loading..." << A_R << flush;
        this_thread::sleep_for(chrono::milliseconds(frameDelay));
        elapsed += frameDelay;
        i = (i + 1) % frames.size();
    }
    cout << A_CLEAR_LINE;
}

inline void showProgressBar() {
    vector<string> states = {
        "Resolving DNS...",
        "Connecting...",
        "Fetching page...",
        "Loading assets...",
        "Done"
    };
    int totalSteps = 40;
    for (int i = 0; i <= totalSteps; ++i) {
        float progress = (float)i / totalSteps;
        int stateIdx = min((int)(progress * (states.size() - 1)), (int)(states.size() - 1));
        
        cout << A_CLEAR_LINE << A_CYN << "  [";
        for (int j = 0; j < totalSteps; ++j) {
            if (j < i) cout << "█";
            else cout << "░";
        }
        cout << "] " << (int)(progress * 100) << "% " 
             << A_DIM << states[stateIdx] << A_R << flush;
        
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    cout << "\n";
}

inline void displayAnimatedHistory(const vector<string>& urls) {
    cout << "\n" << A_BOLD << A_CYN << "  ╭── Slide-in History ───────────╮" << A_R << "\n";
    for (size_t i = 0; i < urls.size(); ++i) {
        cout << "  │ " << A_DIM << i + 1 << ". " << A_R;
        for (char c : urls[i]) {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        cout << "\n";
    }
    cout << A_BOLD << A_CYN << "  ╰───────────────────────────────╯" << A_R << "\n";
}

inline void animateVisitURL(const string& url) {
    showSpinner(800);
    showProgressBar();
    printSuccess("Successfully loaded: " + url);
}
