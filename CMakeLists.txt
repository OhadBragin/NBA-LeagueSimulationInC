cmake_minimum_required(VERSION 3.16)
project(NBA_SIM C)

set(CMAKE_C_STANDARD 11)

add_executable(NBA_SIM main.c
        simulation.c
        team.c
        player.c
        match.c
        schedule.c
        standings.c
        utils.c
        file_io.c
        basketball_sim.h
        ui.c
        playoffs.c
        playoffs.c)
