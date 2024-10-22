#include <fstream>
#include <ranges>
#include <sstream>

#include "iml_lexer.hpp"

#include "iml_debug.h"

static size_t allocs = 0;

void *operator new(size_t size) {
    ++allocs;
    return malloc(size);
}

void operator delete(void *data) {
    free(data);
}


int main() {
    std::ifstream iml_file;
    iml_file.open("../main.iml", std::ifstream::in);
    std::stringstream buffer;
    buffer << iml_file.rdbuf();
    buffer << '\n';

    const std::string &d = buffer.str();
    iml::Lexer lexer(d);

    const std::vector<iml::Lexer::Token> &tokens = lexer.tokenize();
    iml::print(tokens);
    iml_file.close();
    iml::print_fmt("Allocs: {}\n", allocs);
    iml::print_fmt("Tokens count: {}\n", tokens.size());
    return 0;
}
