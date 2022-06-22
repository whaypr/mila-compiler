#ifndef PJPPROJECT_LEXER_HPP
#define PJPPROJECT_LEXER_HPP

#include <iostream>
#include <string>
#include <map>


/*
 * Lexer returns tokens [0-255] if it is an unknown character, otherwise one of these for known things.
 * Here are all valid tokens:
 */
enum Token {
    tok_eof =           -1,
    tok_dot =           -2,
    tok_colon =         -3,
    tok_comma =         -4,

    // numbers and identifiers
    tok_identifier =    -5,
    tok_intLiteral =    -6,
    tok_realLiteral =   -7,
    tok_stringLiteral = -8,

    // keywords
    tok_program =       -9,

    tok_procedure =     -10,
    tok_function =      -11,
    tok_forward =       -12,
    tok_exit =          -13,

    tok_begin =         -14,
    tok_end =           -15,

    tok_const =         -16,
    tok_var =           -17,

    tok_integer =       -18,
    tok_real =          -19,
    tok_string =        -20,

    tok_if =            -21,
    tok_then =          -22,
    tok_else =          -23,

    tok_while =         -24,
    tok_for =           -25,
    tok_do =            -26,

    tok_readln =        -27,
    tok_write =         -28,
    tok_writeln =       -29,

    // unary operators
    tok_not =           -30,

    //binary operators
    tok_assign =        -31,
    tok_equal =         -32,

    tok_plus =          -33,
    tok_minus =         -34,
    tok_star =          -35,
    tok_div =           -36,
    tok_mod =           -37,

    tok_lt =            -38,
    tok_gt =            -39,
    tok_le =            -40,
    tok_ge =            -41,
    tok_notequal =      -42,

    tok_or =            -43,
    tok_and =           -44,
    tok_xor =           -45,

    // keywords in for loop
    tok_to =            -46,
    tok_downto =        -47,

    // keywords for array
    tok_array =         -48,

    // blocks
    tok_lparen =        -49,
    tok_rparen =        -50,
    tok_semicolon =     -51
};


class Lexer {
public:
    Lexer() = default;
    ~Lexer() = default;

    Token nextToken();

    const std::string& identifierStr() const { return this->m_identifierStr; }
    int intVal() { return this->m_intVal; }
    float realVal() { return this->m_realVal; }
    std::string stringVal() { return this->m_stringVal; }

    void nextSymbol();

private:
    std::string m_identifierStr;
    int m_intVal;
    float m_realVal;
    std::string m_stringVal;

    static std::map<std::string, Token> keywords;

    typedef enum {LETTER, NUMBER, UNDERSCORE, END, WHITE_SPACE, NO_TYPE} InputCharType;
    int symbol; // input symbol
    InputCharType symbol_type; // input symbol type
};

#endif //PJPPROJECT_LEXER_HPP
