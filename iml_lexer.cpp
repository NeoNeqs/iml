#include "iml_lexer.hpp"
#include "iml_lex_utils.hpp"

#include <iostream>

namespace iml {
    std::vector<Lexer::Token> Lexer::tokenize() {
        std::vector<Token> tokens;

        // predict the amount of tokens, might need to shrink_to_fit before returning
        tokens.reserve(static_cast<std::size_t>(static_cast<double>(data.size()) / CharactersToTokensRation));

        while (true) {
            const Token &token = next();
            advance();

            if (token.kind == Token::Kind::EoF) {
                tokens.emplace_back(token);
                break;
            }

            if (token.kind != Token::Kind::Empty) {
                tokens.emplace_back(token);
            }
        }

        return tokens;
    }

    Lexer::Token Lexer::next() {
        const char current_char = peek(0);
        if (current_char == '\0') {
            return Token(Token::Kind::EoF);
        }

        if (current_char == ';') {
            return Token(Token::Kind::EoS);
        }

        if (current_char == ',') {
            return Token(Token::Kind::Comma);
        }

        if (current_char == '<') {
            if (peek(1) == '=') {
                advance();
                return Token(Token::Kind::OperatorLessEqual);
            }
            return Token(Token::Kind::OperatorLess);
        }

        if (current_char == '>') {
            if (peek(1) == '=') {
                advance();
                return Token(Token::Kind::OperatorGreaterEqual);
            }
            return Token(Token::Kind::OperatorGreater);
        }

        if (is_letter(current_char) || current_char == '_') {
            return make_special_token();
        }

        if (current_char == '=') {
            if (peek(1) == '=') {
                advance();
                return Token(Token::Kind::OperatorEqual);
            }
            return Token(Token::Kind::OperatorAssignment);
        }

        if (is_digit(current_char)) {
            return make_numeric_literal();
        }

        if (current_char == '(') {
            return Token(Token::Kind::ParenthesisOpen);
        }

        if (current_char == ')') {
            return Token(Token::Kind::ParenthesisClose);
        }

        if (current_char == '{') {
            return Token(Token::Kind::BraceOpen);
        }

        if (current_char == '}') {
            return Token(Token::Kind::BraceClose);
        }

        if (current_char == '[') {
            return Token(Token::Kind::BracketOpen);
        }

        if (current_char == ']') {
            return Token(Token::Kind::BracketClose);
        }

        if (current_char == '+') {
            if (peek(1) == '+') {
                advance();
                return Token(Token::Kind::OperatorIncrement);
            }
            return Token(Token::Kind::OperatorPlus);
        }

        if (current_char == '-') {
            if (peek(1) == '-') {
                advance();
                return Token(Token::Kind::OperatorDecrement);
            }
            return Token(Token::Kind::OperatorMinus);
        }

        if (current_char == '*') {
            return Token(Token::Kind::OperatorStar);
        }

        if (current_char == '/') {
            return Token(Token::Kind::OperatorSlash);
        }

        if (current_char != '\n' && current_char != ' ') {
            std::cout << "Unknown (code, char): (" << static_cast<int>(current_char) << ", " << current_char << ")"
                    << std::endl;
        }
        return EmptyToken;
    }

    Lexer::Token Lexer::make_special_token() {
        const int64_t begin_pos = position;

        skip_until(is_valid_identifier);

        const std::string_view token_name = make_token(begin_pos);
        if (token_name == "if") {
            return Token(Token::Kind::If);
        }
        if (token_name == "for") {
            return Token(Token::Kind::For);
        }
        if (token_name == "func") {
            return Token(Token::Kind::Function);
        }
        if (token_name == "false" || token_name == "true") {
            return Token(Token::Kind::Literal, token_name);
        }
        return Token(Token::Kind::Identifier, token_name);
    }

    Lexer::Token Lexer::make_numeric_literal() {
        const int64_t token_begin = position;

        const char next_char = peek(1);
        if (peek(0) == '0') {
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

        return Token(Token::Kind::Literal, make_token(token_begin));
    }

    void Lexer::advance() {
        if (position >= data.size()) {
            return;
        }

        ++position;
    }
}
