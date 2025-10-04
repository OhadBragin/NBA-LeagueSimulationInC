cmake_minimum_required(VERSION 3.16)
project(NBA_SIM C)

set(CMAKE_C_STANDARD 11)
configure_file(${CMAKE_SOURCE_DIR}/nba_players.txt
        ${CMAKE_BINARY_DIR}/nba_players.txt COPYONLY)
add_executable(NBA_SIM
        main.c
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
        playoffs.c
        season_manager.c)
add_custom_command(TARGET NBA_SIM POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/nba_players.txt"
        "$<TARGET_FILE_DIR:NBA_SIM>"
)
