# Basketball Simulation Project

A comprehensive basketball league simulation written in C.

## Project Structure

```
basketball_sim/
├── main.c              # Entry point and main program flow
├── basketball_sim.h    # Main header with all structs and function declarations
├── simulation.c        # Core simulation logic and main loop
├── team.c             # Team management and statistics
├── player.c           # Player management and statistics  
├── match.c            # Game simulation and scoring
├── schedule.c         # Schedule generation and management
├── standings.c        # League standings and sorting
├── utils.c            # Utility functions and helpers
├── file_io.c          # Save/load functionality
├── Makefile           # Build configuration
└── README.md          # This file
```

## Data Structures

### Player
- `name[30]`: Player name
- `points`: Total points scored this season
- `gamesPlayed`: Total games played this season

### Team  
- `name[50]`: Team name
- `PR`: Power ranking
- `wins/losses`: Team record
- `conference`: East (0) or West (1)
- `seasonPoints`: Total points scored by team
- `roster[MAX_PLAYERS]`: Array of team players

### Match
- `teamA/teamB`: Pointers to competing teams
- `scoreA/scoreB`: Final scores
- `stage`: Game type (0=Regular, 1=Playoffs, 2=Finals)

### Simulation
- `currentDay/totalDays`: Season progress tracking
- `teams[TEAMS_COUNT]`: All league teams
- `schedule[MAX_GAMES_PER_DAY]`: Daily game schedule
- `gamesToday`: Number of games scheduled today

## Building the Project

### Prerequisites
- GCC compiler
- Make utility

### Compile
```bash
make                # Build the project
make debug         # Build with debug symbols
make release       # Optimized release build
```

### Run
```bash
make run           # Compile and run
./basketball_sim   # Run directly
```

### Clean
```bash
make clean         # Remove build files
make rebuild       # Clean and rebuild
```

## Implementation Guide

Each `.c` file contains skeleton functions that need to be implemented:

1. **simulation.c**: Main game loop, initialization, day advancement
2. **team.c**: Team creation, stat updates, power rankings
3. **player.c**: Player stat tracking and management
4. **match.c**: Game simulation algorithm, score calculation
5. **schedule.c**: Season and playoff schedule generation
6. **standings.c**: Sorting and displaying league standings
7. **utils.c**: Helper functions, random numbers, user input
8. **file_io.c**: Save/load game state, export results

## Constants

- `MAX_PLAYERS`: 15 players per team
- `TEAMS_COUNT`: 30 teams in league  
- `MAX_GAMES_PER_DAY`: 16 maximum daily games
- `REGULAR_SEASON_DAYS`: 82 game regular season
- `PLAYOFFS_DAYS`: 28 playoff games
- `FINALS_DAYS`: 7 game finals series

## Features to Implement

- [ ] Team and player initialization
- [ ] Schedule generation (regular season, playoffs)
- [ ] Game simulation with realistic scoring
- [ ] Power rankings and standings
- [ ] Statistics tracking
- [ ] Save/load functionality
- [ ] User interface and controls
- [ ] Playoff bracket generation
- [ ] Season summary and reports

## Usage Example

```c
// Initialize simulation
Simulation sim;
initializeSimulation(&sim);

// Run season
runSimulation(&sim);

// View results
printStandings(&sim);
```
