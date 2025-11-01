# 🏀 C-Sim Basketball League

A comprehensive and lightweight basketball league simulator built entirely in C. This project simulates a full NBA-style season, from schedule generation to the finals, tracking detailed team and player statistics along the way.

## ✨ Features

- **Dynamic Season Simulation**: Progress through a full regular season, playoffs, and finals day-by-day
- **Realistic Scheduling**: Automatically generates a valid league schedule
- **Detailed Stat Tracking**: Monitors wins, losses, points, and power rankings for 30 teams
- **Game Logic**: Simulates individual game outcomes based on team power rankings
- **Persistent State**: Save your league's progress to a file and load it back later
- **Clean Architecture**: Modular code organization separates concerns for easy maintenance and expansion

## 🚀 Getting Started

Follow these instructions to get a copy of the project up and running on your local machine.

### Prerequisites

You'll need a C compiler and the `make` build tool:

- **GCC** (GNU Compiler Collection)
- **Make**

### Installation & Compilation

1. Clone the repository to your local machine:
```bash
git clone https://your-repository-url/basketball-sim.git
cd basketball-sim
```

2. Build the project using the provided Makefile:
```bash
# Build the standard executable (optimized)
make release

# Build with debugging symbols
make debug

# The default 'make' command also builds the debug version
make
```

3. Run the simulation:
```bash
./basketball_sim

# Or compile and run with one command
make run
```

4. Clean up build files:
```bash
make clean
```

## 📁 Project Structure

The project is organized into modules, each with a specific responsibility.

```
basketball_sim/
├── main.c              # Entry point and main program flow
├── basketball_sim.h    # Main header with all structs and function declarations
├── simulation.c        # Core simulation logic and main loop
├── team.c              # Team management and statistics
├── player.c            # Player management and statistics
├── match.c             # Game simulation and scoring
├── schedule.c          # Schedule generation and management
├── standings.c         # League standings and sorting
├── utils.c             # Utility functions and helpers
├── file_io.c           # Save/load functionality
├── Makefile            # Build configuration
└── README.md           # This file
```

## 🧱 Core Data Models

The simulation is built around four main data structures defined in `basketball_sim.h`.

### Player

Represents an individual player in the league.

| Field | Type | Description |
|-------|------|-------------|
| `name` | `char[30]` | The player's full name |
| `points` | `int` | Total points scored by the player during the season |
| `gamesPlayed` | `int` | Total number of games the player has participated in |

### Team

Represents one of the 30 teams in the league.

| Field | Type | Description |
|-------|------|-------------|
| `name` | `char[50]` | The team's official name |
| `PR` | `int` | Power Ranking: a score representing the team's overall strength |
| `wins` / `losses` | `int` | The team's current win-loss record |
| `conference` | `int` | 0 for Eastern Conference, 1 for Western Conference |
| `seasonPoints` | `int` | Total points scored by the team across all games |
| `roster` | `Player[]` | An array of Player structs (MAX_PLAYERS) |

### Match

Represents a single game between two teams.

| Field | Type | Description |
|-------|------|-------------|
| `teamA` / `teamB` | `Team*` | Pointers to the two competing teams |
| `scoreA` / `scoreB` | `int` | The final score for each team |
| `stage` | `int` | 0=Regular, 1=Playoffs, 2=Finals |

### Simulation

The main struct that holds the entire state of the league.

| Field | Type | Description |
|-------|------|-------------|
| `currentDay` | `int` | The current day of the season (1-indexed) |
| `totalDays` | `int` | The total number of days in the current season stage |
| `teams` | `Team[]` | An array of all TEAMS_COUNT teams in the league |
| `schedule` | `Match[]` | An array of games scheduled for the current day |
| `gamesToday` | `int` | The number of games scheduled for the current day |



## 🗺️ Implementation Guide

This project is set up with skeleton functions in each module. The core logic in the following files needs to be completed:

- **`simulation.c`**: Implement the main game loop, initialization, and functions to advance the simulation day by day
- **`team.c`**: Write logic for team creation, updating stats after a game, and calculating power rankings
- **`player.c`**: Implement functions for tracking and updating individual player stats
- **`match.c`**: Develop the core game simulation algorithm to determine the winner and calculate scores
- **`schedule.c`**: Create functions to generate the regular season and playoff schedules
- **`standings.c`**: Implement sorting algorithms to display league standings by conference
- **`file_io.c`**: Build the functions to save the Simulation state to a binary file and load it back
- **`utils.c`**: Add any necessary helper functions for tasks like random number generation or user input

### Example Usage (in `main.c`)

```c
// Initialize simulation
Simulation sim;
initializeSimulation(&sim);

// Run season
runSimulation(&sim);

// View results
printStandings(&sim);
```


