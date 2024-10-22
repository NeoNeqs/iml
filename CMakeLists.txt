cmake_minimum_required(VERSION 3.29)
project(iml)

set(CMAKE_CXX_STANDARD 23)
add_compile_definitions(DEBUG)

add_executable(iml main.cpp
        iml_lexer.cpp
        iml_lexer.hpp
        iml_lex_utils.cpp
        iml_lex_utils.hpp
        iml_debug.h
)
