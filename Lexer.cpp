#include "Lexer.hpp"
#include <map>


std::map<std::string, Token> keywords = {
    {"begin", tok_begin},
    {"end", tok_end},
    {"const", tok_const},
    {"procedure", tok_procedure},
    {"fborward", tok_forward},
    {"function", tok_function},
    {"if", tok_if},
    {"then", tok_then},
    {"else", tok_else},
    {"program", tok_program},
    {"while", tok_while},
    {"exit", tok_exit},
    {"var", tok_var},
    {"integer", tok_integer},
    {"float", tok_float},
    {"for", tok_for},
    {"do", tok_do}
};


void nextSymbol() {
    symbol = getchar();

    // Set type
    if (isalpha(symbol))
        symbol_type = LETTER;
    else if (isdigit(symbol))
        symbol_type = NUMBER;
    else if (symbol == EOF)
        symbol_type = END;
    else if (symbol <= ' ')
        symbol_type = WHITE_SPACE;
    else
        symbol_type = NO_TYPE;
}


void error(std::string text) {
   std::cout << "\n" << text << std::endl;
   exit(1);
}


/**
 * @brief Function to return the next token from standard input
 *
 * the variable 'm_IdentifierStr' is set there in case of an identifier,
 * the variable 'm_IntVal' is set there in case of a number.
 */
int Lexer::gettok() {
    int type;
    int base = 10;
    int divider = 10;
    int digit;

    m_IdentifierStr = "";
    m_IntVal = m_FloatVal = 0;

q_init:
    switch (symbol) {
        case '{':
            nextSymbol();
            goto q1;
        case '+':
            nextSymbol();
            return tok_plus;
        case '-':
            nextSymbol();
            return tok_minus;
        case '*':
            nextSymbol();
            return tok_multiply;
        case '/':
            nextSymbol();
            return tok_div;
        case '<':
            nextSymbol();
            goto q4;
        case '>':
            nextSymbol();
            goto q6;
        case ':':
            nextSymbol();
            goto q5;
        case '.':
            m_FloatVal = 0;
            type = tok_numberFloat;
            nextSymbol();
            goto q3rr;
        default:;
    }
    switch (symbol_type) {
        case WHITE_SPACE:
            nextSymbol();
            goto q_init;
        case END:
            return tok_eof;
        case LETTER:
            m_IdentifierStr += symbol;
            nextSymbol();
            goto q2;
        case NUMBER:
            m_IntVal = symbol - '0';
            if (m_IntVal == 0)
                base = 8;
            type = tok_numberInt;
            nextSymbol();
            goto q3dx;
        default:
            error("Invalid symbol.");
    }

q3dx:
    switch (symbol) {
        case '.':
            m_FloatVal = m_IntVal;
            type = tok_numberFloat;
            nextSymbol();
            goto q3r;
        case 'x':
        case 'X':
            base = 16;
            nextSymbol();
    }
    goto q3;

q1:
    switch(symbol) {
        case '}':
            nextSymbol();
            goto q_init;
        case '\\':
            nextSymbol();
            goto q11;
        default:;
    }
    switch(symbol_type) {
        case END:
            error("Uneexpected end of file in a comment.");
        default:
            nextSymbol();
            goto q1;
    }

q11:
    switch(symbol) {
        case '}':
            nextSymbol();
            goto q1;
        case '\\':
            nextSymbol();
            goto q1;
        default:;
            error("Unexpected escape symbol \\.");
    }

q2:
    switch(symbol_type) {
        case LETTER:
        case NUMBER:
            m_IdentifierStr += symbol;
            nextSymbol();
            goto q2;
        default:
            if (keywords.find(m_IdentifierStr) == keywords.end())
                return tok_identifier;
            return keywords[m_IdentifierStr];
    }

q3:
    switch(symbol) {
        case '.':
            if (base != 10)
                error("Decimal numbers only supported in base 10!");

            m_FloatVal = (float)m_IntVal;
            type = tok_numberInt;
            nextSymbol();
            goto q3r;
    }
    switch(symbol_type) {
        case NUMBER:
        case LETTER:
            if (symbol >= 'a')
                digit = (symbol - 'a') + 10;
            else if (symbol >= 'A')
                digit = (symbol - 'A') + 10;
            else
                digit = (symbol - '0');

            if (digit >= base)
                error("Digit not allowed in this base!");

            m_IntVal = base * m_IntVal + digit; 
            nextSymbol();
            goto q3;
        default:
            return type;
    }

q3rr:
    switch(symbol_type) {
        case NUMBER:
            goto q3r;
        default:
            error("Decimal point is not a number.");
    }

q3r:
    switch(symbol_type) {
        case NUMBER:
            digit = (symbol - '0');
            m_FloatVal += ((float)digit)/divider;
            divider *= 10;
            nextSymbol();
            goto q3r;
        default:
            return tok_numberFloat;
    }

q4:
    switch(symbol) {
        case '=':
            nextSymbol();
            return tok_lessequal;
        default:;
    }
    switch(symbol_type) {
        default:
            return tok_lesser;
    }

q5:
    switch(symbol) {
        case '=':
            nextSymbol();
            return tok_assign;
        default:;
    }
    switch(symbol_type) {
        default:
            error("Expected \'=\'.");
    }

q6:
    switch(symbol) {
        case '=':
            nextSymbol();
            return tok_greaterequal;
        default:;
    }
    switch(symbol_type) {
        default:
            return tok_greater;
    }
}
