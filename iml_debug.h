#ifndef IML_DEBUG_H
#define IML_DEBUG_H

#include <format>
#include <iostream>
#include <vector>

namespace iml {
    static constexpr std::string token_names[] = {
        "Empty",
        "Identifier",
        "Literal",

        "<",
        "<=",
        ">",
        ">=",
        "==",
        "!=",

        "&",
        "|",
        "^",
        "~",
        "<<",
        ">>",

        "&&",
        "||",
        "!",

        "++",
        "--",
        "+",
        "-",
        "*",
        "**",
        "/",
        "%",

        "=",

        "if",
        "for",
        "func",

        "{",
        "}",
        "[",
        "]",
        "(",
        ")",
        ",",
        ".",

        "EoS",
        "EoF",
    };

    template<class... Args>
    void print_fmt(std::format_string<Args...> fmt, Args &&... args) {
        std::cout << std::format(fmt, std::forward<Args>(args)...);
    }

    inline void print(const std::vector<Lexer::Token> &tokens) {
        for (const auto &[kind, name]: tokens) {
            if (!name.empty()) {
                print_fmt("[{}: \"{}\"]", token_names[static_cast<int>(kind)], name);
            } else {
                print_fmt("[\"{}\"]", token_names[static_cast<int>(kind)]);
            }
            if (kind == Lexer::Token::Kind::EoS) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

#endif //IML_DEBUG_H
