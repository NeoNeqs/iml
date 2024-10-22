#ifndef LEXER_H
#define LEXER_H

#include <functional>
#include <string>
#include <vector>

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
            std::string name;

            constexpr bool operator==(const Kind p_kind) const {
                return kind == p_kind;
            }
        };

    private:
        // 2.3 is good for the start, needs to recalibrated with more data
        static constexpr float CharactersToTokensRation = 2.3;

        std::string data;
        std::vector<Token> tokens;
        int64_t position;

    public:
        void tokenize();
        void print();

        template<typename T>
            requires std::convertible_to<T, std::string>
        explicit Lexer(T &&p_data) : data{std::forward<T>(p_data)}, position{0} {
            data.reserve(p_data.size() / CharactersToTokensRation);
        }

    private:
        void scan();
        void advance();

        [[nodiscard]] std::string make_token(const int64_t begin_pos) const {
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

        [[nodiscard]] Token get_last_token() const {
            if (tokens.empty()) {
                return {Token::Kind::Empty};
            }
            return tokens.back();
        }
    };
}
#endif // LEXER_H
