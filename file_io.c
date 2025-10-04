#include "basketball_sim.h"
#include "colors.h"

// Headers needed for cross-platform directory creation
#ifdef _WIN32
    #include <direct.h> // For _mkdir on Windows
    #include <windows.h> // For FindFirstFile, FindNextFile, FindClose
#else
    #include <sys/stat.h> // For mkdir on Linux/macOS
    #include <dirent.h>   // For opendir, readdir, closedir
#endif




void saveSimulation(const Simulation *sim) {
    #ifdef _WIN32
        _mkdir("saves");
    #else
        // On Linux/macOS, mkdir requires permissions. 0777 gives full access.
        mkdir("saves", 0777);
    #endif

    char filename[100];
    sprintf(filename, "saves/nba_season_%d-%d.sav", sim->year, sim->year + 1);

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error: Could not open file for saving. Please check permissions");
        return;
    }

    // 1. Write the season year first for verification
    fwrite(&sim->year, sizeof(int), 1, file);

    // 2. Write the entire block of 30 teams
    for (int i = 0; i < TEAMS_COUNT; i++) {
        const Team *t = &sim->teams[i];

        // Write the team's name (length first, then the string)
        size_t nameLen = strlen(t->name) + 1;
        fwrite(&nameLen, sizeof(size_t), 1, file);
        fwrite(t->name, 1, nameLen, file);

        // Write the simple integer fields
        fwrite(&t->PR, sizeof(int), 1, file);
        fwrite(&t->wins, sizeof(int), 1, file);
        fwrite(&t->losses, sizeof(int), 1, file);
        fwrite(&t->conference, sizeof(int), 1, file);
        fwrite(&t->seasonPoints, sizeof(int), 1, file);

        // Write the entire roster (this is safe as Player struct has no pointers)
        fwrite(t->roster, sizeof(Player), MAX_PLAYERS, file);
    }



    // 4. Write the index of the champion team
    int championIndex = -1;
    if (sim->champion) {
        championIndex = sim->champion - sim->teams;
    }
    fwrite(&championIndex, sizeof(int), 1, file);

    fclose(file);
    printf(COLOR_SUCCESS "Season %d-%d results saved to '%s'\n" COLOR_RESET, sim->year, sim->year + 1, filename);
}



int loadSimulation(Simulation *sim, const char *filename) {
    cleanupSimulation(sim); // This is the correct first step.

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file for loading");
        return 0;
    }


    // Read the season year
    if (fread(&sim->year, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to read season year.\n");
        fclose(file); return 0;
    }

    // Iterate and read each team's data field by field
    for (int i = 0; i < TEAMS_COUNT; i++) {
        Team *t = &sim->teams[i];

        // Read the team's name
        size_t nameLen;
        if (fread(&nameLen, sizeof(size_t), 1, file) != 1) {
            fprintf(stderr, "Error: Failed to read name length for team %d.\n", i);
            fclose(file); return 0;
        }
        t->name = malloc(nameLen);
        if (!t->name) {
            fprintf(stderr, "Error: Malloc failed for team name.\n");
            fclose(file); return 0;
        }
        if (fread(t->name, 1, nameLen, file) != nameLen) {
            fprintf(stderr, "Error: Failed to read name for team %d.\n", i);
            fclose(file); return 0;
        }

        // Read the simple integer fields
        if (fread(&t->PR, sizeof(int), 1, file) != 1) { fprintf(stderr, "Error: Failed to read PR for team %s.\n", t->name); fclose(file); return 0; }
        if (fread(&t->wins, sizeof(int), 1, file) != 1) { fprintf(stderr, "Error: Failed to read wins for team %s.\n", t->name); fclose(file); return 0; }
        if (fread(&t->losses, sizeof(int), 1, file) != 1) { fprintf(stderr, "Error: Failed to read losses for team %s.\n", t->name); fclose(file); return 0; }
        if (fread(&t->conference, sizeof(int), 1, file) != 1) { fprintf(stderr, "Error: Failed to read conference for team %s.\n", t->name); fclose(file); return 0; }
        if (fread(&t->seasonPoints, sizeof(int), 1, file) != 1) { fprintf(stderr, "Error: Failed to read seasonPoints for team %s.\n", t->name); fclose(file); return 0; }

        // Read the entire roster
        if (fread(t->roster, sizeof(Player), MAX_PLAYERS, file) != MAX_PLAYERS) {
            fprintf(stderr, "Error: Failed to read roster for team %s.\n", t->name);
            fclose(file); return 0;
        }
    }

    // 3. Read the champion's index and restore the pointer
    int championIndex;
    if (fread(&championIndex, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to read champion index.\n");
        fclose(file); return 0;
    }
    if (championIndex >= 0 && championIndex < TEAMS_COUNT) {
        sim->champion = &sim->teams[championIndex];
    } else {
        sim->champion = NULL;
    }

    sim->seasonComplete = 1;
    sim->playoffsComplete = 1;
    sim->matchSchedule = NULL;

    fclose(file);
    printf(COLOR_SUCCESS "Loaded season %d-%d successfully!\n" COLOR_RESET, sim->year, sim->year + 1);
    return 1;
}


char* selectSaveFile() {
    char *filenames[256]; // Max 256 save files
    int count = 0;

    printf(COLOR_HEADER "\n=== Load a Past Season ===\n" COLOR_RESET);

    #ifdef _WIN32
        WIN32_FIND_DATA fd;
        HANDLE hFind = FindFirstFile("saves\\*.sav", &fd);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (count < 256) {
                    filenames[count++] = _strdup(fd.cFileName);
                }
            } while (FindNextFile(hFind, &fd) != 0);
            FindClose(hFind);
        }
    #else
        DIR *d;
        struct dirent *dir;
        d = opendir("saves");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strstr(dir->d_name, ".sav") && count < 256) {
                    filenames[count++] = strdup(dir->d_name);
                }
            }
            closedir(d);
        }
    #endif

    if (count == 0) {
        printf(COLOR_WARNING "No saved seasons found in the 'saves' directory.\n" COLOR_RESET);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        printf("%2d. %s\n", i + 1, filenames[i]);
    }
    printf(" 0. Cancel\n");

    int choice = getUserChoice(0, count);

    if (choice == 0) {
        for (int i = 0; i < count; i++) free(filenames[i]);
        return NULL;
    }

    char *selectedFile = malloc(100);
    sprintf(selectedFile, "saves/%s", filenames[choice - 1]);

    // Free all allocated names
    for (int i = 0; i < count; i++) {
        free(filenames[i]);
    }

    return selectedFile;
}

