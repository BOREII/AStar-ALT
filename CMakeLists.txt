cmake_minimum_required(VERSION 3.13)
project(AStar)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++z -Wall -Werror -Wextra -Wpedantic -O2 -g -fsanitize=address,undefined")

add_executable(AStar main.cpp test_runner.h profile.h map_building.h graph.h
        priority_queue.h dijkstra.h tests.h alt.h landmarks.h)