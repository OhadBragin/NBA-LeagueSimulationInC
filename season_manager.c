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
    FILE *fptr;
    int year;
    #ifdef _WIN32
        _mkdir("data");
    #else
        // On Linux/macOS, mkdir requires permissions. 0777 gives full access.
        mkdir("data", 0777);
    #endif

    const char *filename = "data/season_year.txt";

    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        // File doesn't exist, so create it with the current year
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        year = tm->tm_year + 1900; // Get current year

        fptr = fopen(filename, "w");
        if (fptr == NULL) {
            // This should now only fail if there are disk space or permission issues.
            perror("FATAL ERROR: Could not create season tracking file");
            pressEnterToContinue();
            exit(1);
        }
        fprintf(fptr, "%d", year);
        fclose(fptr);
        return year;
    }

    // File exists, so read the year, increment it, and write it back
    if (fscanf(fptr, "%d", &year) != 1) {
        fprintf(stderr, "FATAL ERROR: Could not read from season year file. It may be corrupted.\n");
        pressEnterToContinue();
        exit(1);
    }
    fclose(fptr);

    year++; // Increment for the next season

    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        perror("FATAL ERROR: Could not update season tracking file");
        exit(1);
    }
    fprintf(fptr, "%d", year);
    fclose(fptr);
    return year;
}

