#include "iml_lexer.hpp"
#include "iml_lex_utils.hpp"

#include <iostream>

namespace iml {
    std::vector<Lexer::Token> Lexer::tokenize() {
        std::vector<Token> tokens;

        // predict the amount of tokens, might need to shrink_to_fit before returning
        tokens.reserve(static_cast<std::size_t>(static_cast<double>(data.size()) / CharactersToTokensRation));

        while (true) {
            const Token &token = scan();

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

    Lexer::Token Lexer::scan() {
        Token token;

        const char current_char = peek(0);
        if (current_char == '\0') {
            token = Token(Token::Kind::EoF);
        } else if (current_char == ';') {
            token = Token(Token::Kind::EoS);
        } else if (current_char == ',') {
            token = Token(Token::Kind::Comma);
        } else if (current_char == '<') {
            if (peek(1) == '=') {
                advance();
                token = Token(Token::Kind::OperatorLessEqual);
            } else {
                token = Token(Token::Kind::OperatorLess);
            }
        } else if (current_char == '>') {
            if (peek(1) == '=') {
                advance();
                token = Token(Token::Kind::OperatorGreaterEqual);
            } else {
                token = Token(Token::Kind::OperatorGreater);
            }
        } else if (is_letter(current_char) || current_char == '_') {
            token = make_special_token();
        } else if (current_char == '=') {
            if (peek(1) == '=') {
                advance();
                token = Token(Token::Kind::OperatorEqual);
            } else {
                token = Token(Token::Kind::OperatorAssignment);
            }
        } else if (is_digit(current_char)) {
            token = make_numeric_literal();
        } else if (current_char == '(') {
            token = Token(Token::Kind::ParenthesisOpen);
        } else if (current_char == ')') {
            token = Token(Token::Kind::ParenthesisClose);
        } else if (current_char == '{') {
            token = Token(Token::Kind::BraceOpen);
        } else if (current_char == '}') {
            token = Token(Token::Kind::BraceClose);
        } else if (current_char == '[') {
            token = Token(Token::Kind::BracketOpen);
        } else if (current_char == ']') {
            token = Token(Token::Kind::BracketClose);
        } else if (current_char == '+') {
            if (peek(1) == '+') {
                advance();
                token = Token(Token::Kind::OperatorIncrement);
            } else {
                token = Token(Token::Kind::OperatorPlus);
            }
        } else if (current_char == '-') {
            if (peek(1) == '-') {
                advance();
                token = Token(Token::Kind::OperatorDecrement);
            } else {
                token = Token(Token::Kind::OperatorMinus);
            }
        } else if (current_char == '*') {
            token = Token(Token::Kind::OperatorStar);
        } else if (current_char == '/') {
            token = Token(Token::Kind::OperatorSlash);
        } else {
            if (current_char != '\n' && current_char != ' ') {
                std::cout << "Unknown (code, char): (" << static_cast<int>(current_char) << ", " << current_char << ")"
                        << std::endl;
            }
            token = Token(Token::Kind::Empty);
        }
        advance();

        return token;
    }

    Lexer::Token Lexer::make_special_token() {
        Token token;

        const int64_t begin_pos = position;

        skip_until(is_valid_identifier);

        const std::string_view token_name = make_token(data, begin_pos);
        if (token_name == "if") {
            token = Token(Token::Kind::If);
        } else if (token_name == "for") {
            token = Token(Token::Kind::For);
        } else if (token_name == "func") {
            token = Token(Token::Kind::Function);
        } else if (token_name == "false" || token_name == "true") {
            token = Token(Token::Kind::Literal, token_name);
        } else {
            token = Token(Token::Kind::Identifier, token_name);
        }

        return token;
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

        return Token(Token::Kind::Literal, make_token(data, token_begin));
    }

    void Lexer::advance() {
        if (position >= data.size()) {
            return;
        }

        ++position;
    }
}
