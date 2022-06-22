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
    tok_dot =           -114,
    tok_colon =         -115,
    tok_comma =         -116,

    // numbers and identifiers
    tok_identifier =    -2,
    tok_intLiteral =    -3,
    tok_floatLiteral =  -100,
    tok_stringLiteral = -117,

    // keywords
    tok_program =       -13,

    tok_procedure =     -7,
    tok_function =      -9,
    tok_forward =       -8,
    tok_exit =          -15,

    tok_begin =         -4,
    tok_end =           -5,

    tok_const =         -6,
    tok_var =           -16,

    tok_integer =       -17,
    tok_float =         -101,

    tok_if =            -10,
    tok_then =          -11,
    tok_else =          -12,

    tok_while =         -14,
    tok_for =           -18,
    tok_do =            -19,

    tok_readln =        -102,
    tok_write =         -103,
    tok_writeln =       -104,

    // unary operators
    tok_not =           -27,

    //binary operators
    tok_assign =        -23,
    tok_equal =         -113,

    tok_plus =          -105,
    tok_minus =         -106,
    tok_multiply =      -107,
    tok_div =           -26,
    tok_mod =           -25,

    tok_lesser =        -108,
    tok_greater =       -109,
    tok_lessequal =     -21,
    tok_greaterequal =  -22,
    tok_notequal =      -20,

    tok_or =            -24,
    tok_and =           -28,
    tok_xor =           -29,

    // keywords in for loop
    tok_to =            -30,
    tok_downto =        -31,

    // keywords for array
    tok_array =         -32,

    // blocks
    tok_parenth_left =  -110,
    tok_parenth_right = -111,
    tok_semicolon =     -112
};


class Lexer {
public:
    Lexer() = default;
    ~Lexer() = default;

    int nextToken();

    const std::string& identifierStr() const { return this->m_IdentifierStr; }
    int intVal() { return this->m_IntVal; }
    float floatVal() { return this->m_FloatVal; }
    std::string stringVal() { return this->m_stringVal; }

    void nextSymbol();

private:
    std::string m_IdentifierStr;
    int m_IntVal;
    float m_FloatVal;
    std::string m_stringVal;

    static std::map<std::string, Token> keywords;

    typedef enum {LETTER, NUMBER, END, WHITE_SPACE, NO_TYPE} InputCharType;
    int symbol; // input symbol
    InputCharType symbol_type; // input symbol type
};

#endif //PJPPROJECT_LEXER_HPP
