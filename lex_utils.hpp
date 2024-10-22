#ifndef LEX_UTILS_H
#define LEX_UTILS_H

namespace iml {
    bool is_letter(char c);
    bool is_digit(char c);
    bool is_hex_digit(char c);
    bool is_oct_digit(char c);
    bool is_bin_digit(char c);
    bool is_decimal(char c);
    bool is_valid_identifier(char c);
}

#endif //LEX_UTILS_H
