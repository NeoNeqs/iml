#ifndef IML_LEXER_H
#define IML_LEXER_H

#include <functional>
#include <string>
#include <vector>
#include <string_view>

namespace iml {
    class Lexer {

    public:
        struct Token {
            enum class Kind {
                Empty,
                Identifier,
                Literal,

                OperatorLess,
                OperatorLessEqual,
                OperatorGreater,
                OperatorGreaterEqual,
                OperatorEqual,
                OperatorNotEqual,

                OperatorBitwiseAnd,
                OperatorBitwiseOr,
                OperatorBitwiseXor,
                OperatorBitwiseNot,
                OperatorBitwiseShiftLeft,
                OperatorBitwiseShiftRight,

                OperatorAnd,
                OperatorOr,
                OperatorNot,

                OperatorIncrement,
                OperatorDecrement,
                OperatorPlus,
                OperatorMinus,
                OperatorStar,
                OperatorDoubleStar,
                OperatorSlash,
                OperatorPercent,

                OperatorAssignment,

                If,
                For,
                Function,

                BraceOpen,
                BraceClose,
                BracketOpen,
                BracketClose,
                ParenthesisOpen,
                ParenthesisClose,
                Comma,
                Period,
                EoS,
                EoF,
            };

            Kind kind;
            std::string_view name;

            explicit Token(const Kind kind,
                           const std::string_view p_name = std::string_view("")) : kind(kind) {
                name = p_name;
            }
        };

        std::vector<Token> tokenize();

        // Constructor converts p_data to `std::string_view`. Those 2 are deleted to ensure the data pointed to by
        // `std::string_view` doesn't go out of scope when p_data goes out of scope (which would be the case if a
        // temporary is passed)
        explicit Lexer(std::string &&p_data) = delete;
        explicit Lexer(const std::string &&p_data) = delete;

        explicit Lexer(const std::string &p_data) noexcept : data(p_data),
                                                             position(0),
                                                             EmptyToken(Token::Kind::Empty) {}

    private:
        // 2.3 is good for the start, needs to recalibrated with more data
        static constexpr double CharactersToTokensRation = 2.3;

        std::string_view data;
        int64_t position;

        Token EmptyToken;

        Token next();
        Token make_special_token();
        Token make_numeric_literal();

        void advance() {
            if (position >= data.size()) {
                return;
            }

            ++position;
        }

        [[nodiscard]] std::string_view make_token(const int64_t begin_pos) const {
            return data.substr(begin_pos, position - begin_pos + 1);
        }

        void skip_until(const std::function<bool(char)> &predicate) {
            char next_char;
            while ((next_char = peek(1)) != '\0' && predicate(next_char)) {
                advance();
            }
        }

        void skip_until_not(const std::function<bool(char)> &predicate) {
            char next_char;
            while ((next_char = peek(1)) != '\0' && !predicate(next_char)) {
                advance();
            }
        }

        [[nodiscard]] char peek(const int64_t p_offset) const {
            if (position + p_offset < 0 || position + p_offset >= data.size()) {
                return '\0';
            }

            return data[position + p_offset];
        }
    };
}
#endif // IML_LEXER_H
