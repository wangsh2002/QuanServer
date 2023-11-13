cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)
aux_source_directory(. ALL_SRCS)
add_executable(add1 ${ALL_SRCS})