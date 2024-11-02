cmake_minimum_required(VERSION 3.15...3.30)
project(GLAD)
add_library(GLAD glad.c)

target_include_directories(GLAD PUBLIC ../include)