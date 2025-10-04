#include "basketball_sim.h"
#include "colors.h"
#include <time.h>

// Headers needed for cross-platform directory creation
#ifdef _WIN32
    #include <direct.h> // For _mkdir on Windows
#else
    #include <sys/stat.h> // For mkdir on Linux/macOS
#endif

// Delete all files in a directory (used for cleaning up old saves)
void deleteFilesInDirectory(const char *path) {
    #ifdef _WIN32
        char searchPath[260];
        sprintf(searchPath, "%s\\*.*", path);
        WIN32_FIND_DATA fd;
        HANDLE hFind = FindFirstFile(searchPath, &fd);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                char filePath[260];
                sprintf(filePath, "%s\\%s", path, fd.cFileName);
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    DeleteFile(filePath);
                }
            } while (FindNextFile(hFind, &fd) != 0);
            FindClose(hFind);
        }
    #else
        DIR *d = opendir(path);
        if (!d) return;
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                char filePath[260];
                sprintf(filePath, "%s/%s", path, dir->d_name);
                unlink(filePath); // unlink is the POSIX function to delete a file
            }
        }
        closedir(d);
    #endif
}


void factoryReset() {
    printf(COLOR_WARNING "\nThis will delete ALL saved seasons and reset the season tracker.\n" COLOR_RESET);
    printf("Are you sure you want to continue? (1 for Yes, 0 for No): ");
    int choice = getUserChoice(0, 1);

    if (choice == 1) {
        // Delete all files in 'saves' and then remove the directory
        deleteFilesInDirectory("saves");
        #ifdef _WIN32
                _rmdir("saves");
        #else
                rmdir("saves");
        #endif

            // Delete the season tracking file and then remove the 'data' directory
        deleteFilesInDirectory("data");
        #ifdef _WIN32
                _rmdir("data");
        #else
                rmdir("data");
        #endif

    printf(COLOR_SUCCESS "All data has been erased. The simulation has been reset.\n" COLOR_RESET);
    } else {
        printf(COLOR_INFO "Reset cancelled.\n" COLOR_RESET);
    }
    pressEnterToContinue();
}

// Get the next season year from file, or create the file if it doesn't exist
int getNextSeasonYear() {
    FILE *file;
    int lastCompletedYear;
    const char *filename = "data/season_year.txt";

    // Ensure the 'data' directory exists.
    #ifdef _WIN32
        _mkdir("data");
    #else
        mkdir("data", 0777);
    #endif

    file = fopen(filename, "r");
    if (file == NULL) {
        // File doesn't exist, this is the first ever run.
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        lastCompletedYear = tm->tm_year + 1900 - 1; // e.g., 2024

        // Create the file so it's there for next time.
        file = fopen(filename, "w");
        if (file) {
            fprintf(file, "%d", lastCompletedYear);
            fclose(file);
        }
        // The new season will be the current year.
        return lastCompletedYear + 1;
    }
        // File exists, read the last completed year.
        if (fscanf(file, "%d", &lastCompletedYear) != 1) {
            fprintf(stderr, "Error reading from season year file. Using default year.\n");
            fclose(file);
            return 2025; // Fallback
        }
        fclose(file);
        // The new season is the year after the last completed one.
        return lastCompletedYear + 1;
}

