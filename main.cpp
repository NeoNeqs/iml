#include <fstream>
#include <ranges>
#include <sstream>

#include "iml_lexer.hpp"

#include "iml_debug.h"

int main() {
    std::ifstream iml_file;
    iml_file.open("../main.iml", std::ifstream::in);
    std::stringstream buffer;
    buffer << iml_file.rdbuf();
    buffer << '\n';
    iml_file.close();

    std::string d = buffer.str();

    iml::Lexer lexer(d);

    const std::vector<iml::Lexer::Token> &tokens = lexer.tokenize();

    iml::print(tokens);
    iml::print_fmt("Tokens count: {}\n", tokens.size());
    return 0;
}
