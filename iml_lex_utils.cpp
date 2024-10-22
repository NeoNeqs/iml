#include "iml_lex_utils.hpp"

namespace iml {
    bool is_letter(const char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    bool is_digit(const char c) {
        return c >= '0' && c <= '9';
    }

    bool is_hex_digit(const char c) {
        return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    bool is_oct_digit(const char c) {
        return c >= '0' && c <= '7';
    }

    bool is_bin_digit(const char c) {
        return c == '0' || c == '1';
    }

    bool is_decimal(const char c) {
        return is_digit(c) || c == '.';
    }

    bool is_valid_identifier(const char c) {
        return is_letter(c) || is_digit(c) || c == '_';
    }
}
