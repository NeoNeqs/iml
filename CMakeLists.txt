cmake_minimum_required(VERSION 3.29)
project(iml)

set(CMAKE_CXX_STANDARD 23)
add_compile_definitions(DEBUG)

add_executable(iml main.cpp
        Lexer.cpp
        Lexer.hpp
        lex_utils.cpp
        lex_utils.hpp
)
