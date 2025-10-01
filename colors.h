// colors.h - Cross-platform color definitions
#ifndef COLORS_H
#define COLORS_H

// ANSI color codes (work on Mac/Linux/Windows 10+)
#ifdef _WIN32
    #include <windows.h>
    // Enable ANSI colors on Windows 10+
    static inline void enableColors() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
#else
    static inline void enableColors() {
        // Colors work by default on Mac/Linux
    }
#endif

// Color scheme for basketball simulation
#define COLOR_RESET         "\033[0m"
#define COLOR_BOLD          "\033[1m"

// Team and score colors
#define COLOR_TEAM_NAME     "\033[1;36m"  // Bright Cyan - for team names
#define COLOR_WINNER        "\033[1;32m"  // Bright Green - for winning team
#define COLOR_LOSER         "\033[0;37m"  // White - for losing team
#define COLOR_SCORE_WIN     "\033[1;33m"  // Bright Yellow - winning score
#define COLOR_SCORE_LOSE    "\033[0;90m"  // Dark Gray - losing score

// Headers and sections
#define COLOR_HEADER        "\033[1;35m"  // Bright Magenta - for headers
#define COLOR_SUBHEADER     "\033[1;34m"  // Bright Blue - for subheaders
#define COLOR_BORDER        "\033[0;36m"  // Cyan - for borders/lines

// Stats and info
#define COLOR_STAT_LABEL    "\033[0;33m"  // Yellow - for stat labels
#define COLOR_STAT_VALUE    "\033[1;37m"  // Bright White - for stat values
#define COLOR_PLAYER        "\033[0;37m"  // White - for player names
#define COLOR_NUMBER        "\033[1;36m"  // Bright Cyan - for numbers

// Special highlights
#define COLOR_SUCCESS       "\033[1;32m"  // Bright Green - success messages
#define COLOR_ERROR         "\033[1;31m"  // Bright Red - error messages
#define COLOR_WARNING       "\033[1;33m"  // Bright Yellow - warnings
#define COLOR_INFO          "\033[0;36m"  // Cyan - info messages

// Game stage colors
#define COLOR_REGULAR       "\033[0;34m"  // Blue - regular season
#define COLOR_PLAYOFFS      "\033[1;35m"  // Bright Magenta - playoffs
#define COLOR_FINALS        "\033[1;31m"  // Bright Red - finals

// Conference colors
#define COLOR_EAST          "\033[1;34m"  // Bright Blue - Eastern Conference
#define COLOR_WEST          "\033[1;31m"  // Bright Red - Western Conference

#endif // COLORS_H