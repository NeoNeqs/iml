#include <fstream>
#include <iostream>
#include <sstream>

#include "Lexer.hpp"


int main() {
    std::ifstream iml_file;
    iml_file.open("../main.iml", std::ifstream::in);
    std::stringstream buffer;
    buffer << iml_file.rdbuf();
    buffer << '\n';
    iml::Lexer lexer(buffer.str());
    lexer.tokenize();
    iml_file.close();

    lexer.print();
    return 0;
}
