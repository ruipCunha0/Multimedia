cmake_minimum_required(VERSION 3.25)
project(Socket C)

set(CMAKE_C_STANDARD 11)

add_executable(Socket server.c client.c)
