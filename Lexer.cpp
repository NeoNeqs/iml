#include "Lexer.hpp"
#include "lex_utils.hpp"

#include <iostream>
#include <random>
#include <string>
#include <format>

template<class... Args>
void printfmt(std::format_string<Args...> fmt, Args &&... args) {
    std::cout << std::format(fmt, std::forward<Args>(args)...);
}

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

    void Lexer::tokenize() {
        do {
            scan();
        } while (get_last_token().kind != Token::Kind::EoF);
    }

    void Lexer::print() {
        for (const auto &[kind, name]: tokens) {
            if (!name.empty()) {
                printfmt("[{}: \"{}\"]", token_names[static_cast<int>(kind)], name);
            } else {
                printfmt("[\"{}\"]", token_names[static_cast<int>(kind)]);
            }
            if (kind == Token::Kind::EoS) {
                std::cout << std::endl;
            }
        }
    }

    void Lexer::scan() {
        const char current_char = peek(0);

        // Double token
        if (current_char == '\0') {
            // Make sure last statement is closed before the end of file
            if (get_last_token().kind != Token::Kind::EoS) {
                tokens.emplace_back(Token::Kind::EoS);
            }
            tokens.emplace_back(Token::Kind::EoF);
        } else if (current_char == ';') {
            // Prevent duplicate EoS next to each other
            if (get_last_token().kind != Token::Kind::EoS) {
                tokens.emplace_back(Token::Kind::EoS);
            }
        } else if (current_char == ',') {
            tokens.emplace_back(Token::Kind::Comma);
        } else if (current_char == '<') {
            if (peek(1) == '=') {
                advance();
                tokens.emplace_back(Token::Kind::OperatorLessEqual);
            } else {
                tokens.emplace_back(Token::Kind::OperatorLess);
            }
        } else if (current_char == '>') {
            if (peek(1) == '=') {
                advance();
                tokens.emplace_back(Token::Kind::OperatorGreaterEqual);
            } else {
                tokens.emplace_back(Token::Kind::OperatorGreater);
            }
        } else if (is_letter(current_char) || current_char == '_') {
            const int64_t begin_pos = position;

            skip_until(is_valid_identifier);

            const std::string token = make_token(begin_pos);
            if (token == "if") {
                tokens.emplace_back(Token::Kind::If);
            } else if (token == "for") {
                tokens.emplace_back(Token::Kind::For);
            } else if (token == "func") {
                tokens.emplace_back(Token::Kind::Function);
            } else if (token == "false" || token == "true") {
                tokens.emplace_back(Token::Kind::Literal, token);
            } else {
                tokens.emplace_back(Token::Kind::Identifier, token);
            }
        } else if (current_char == '=') {
            if (peek(1) == '=') {
                advance();
                tokens.emplace_back(Token::Kind::OperatorEqual);
            } else {
                tokens.emplace_back(Token::Kind::OperatorAssignment);
            }
        } else if (is_digit(current_char)) {
            const int64_t token_begin = position;

            const char next_char = peek(1);
            if (current_char == '0') {
                if (next_char == 'x') {
                    ++position;
                    skip_until(is_hex_digit);
                } else if (next_char == 'b') {
                    ++position;
                    skip_until(is_bin_digit);
                } else if (is_digit(next_char)) {
                    ++position;
                    skip_until(is_oct_digit);
                }
            } else {
                skip_until(is_decimal);
            }

            tokens.emplace_back(Token::Kind::Literal, make_token(token_begin));
        } else if (current_char == '(') {
            tokens.emplace_back(Token::Kind::ParenthesisOpen);
        } else if (current_char == ')') {
            tokens.emplace_back(Token::Kind::ParenthesisClose);
        } else if (current_char == '{') {
            tokens.emplace_back(Token::Kind::BraceOpen);
            tokens.emplace_back(Token::Kind::EoS);
        } else if (current_char == '}') {
            tokens.emplace_back(Token::Kind::BraceClose);
            tokens.emplace_back(Token::Kind::EoS);
        } else if (current_char == '[') {
            tokens.emplace_back(Token::Kind::BracketOpen);
        } else if (current_char == ']') {
            tokens.emplace_back(Token::Kind::BracketClose);
        } else if (current_char == '+') {
            if (peek(1) == '+') {
                advance();
                tokens.emplace_back(Token::Kind::OperatorIncrement);
            } else {
                tokens.emplace_back(Token::Kind::OperatorPlus);
            }
        } else if (current_char == '-') {
            if (peek(1) == '-') {
                advance();
                tokens.emplace_back(Token::Kind::OperatorDecrement);
            } else {
                tokens.emplace_back(Token::Kind::OperatorMinus);
            }
        } else if (current_char == '*') {
            tokens.emplace_back(Token::Kind::OperatorStar);
        } else if (current_char == '/') {
            tokens.emplace_back(Token::Kind::OperatorSlash);
        } else {
            if (current_char != '\n' && current_char != ' ') {
                std::cout << "Unknown (code, char): (" << static_cast<int>(current_char) << ", " << current_char << ")" << std::endl;
            }
        }
        advance();
    }

    void Lexer::advance() {
        if (position >= data.size()) {
            return;
        }

        ++position;
    }

    
}
