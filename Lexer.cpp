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

// INITIAL STATE
init:
    switch (symbol) {
        case '{':
            nextSymbol();
            goto comment;
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
            goto lesser_eq;
        case '>':
            nextSymbol();
            goto greater_eq;
        case ':':
            nextSymbol();
            goto assign;
        case '.':
            m_FloatVal = 0;
            type = tok_numberFloat;
            nextSymbol();
            goto pre_float;
        default:;
    }
    switch (symbol_type) {
        case WHITE_SPACE:
            nextSymbol();
            goto init;
        case END:
            return tok_eof;
        case LETTER:
            m_IdentifierStr += symbol;
            nextSymbol();
            goto ident;
        case NUMBER:
            m_IntVal = symbol - '0';
            if (m_IntVal == 0)
                base = 8;
            type = tok_numberInt;
            nextSymbol();
            goto pre_num;
        default:
            error("Invalid symbol.");
    }

// COMMENTS
comment:
    switch(symbol) {
        case '}':
            nextSymbol();
            goto init;
        case '\\':
            nextSymbol();
            goto comment_esc;
        default:;
    }
    switch(symbol_type) {
        case END:
            error("Uneexpected end of file in a comment.");
        default:
            nextSymbol();
            goto comment;
    }

comment_esc:
    switch(symbol) {
        case '}':
        case '\\':
            nextSymbol();
            goto comment;
        default:;
            error("Unexpected escape symbol \\.");
    }

// LESSER, GREATER
lesser_eq:
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

greater_eq:
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

// ASSIGN
assign:
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

// IDENTIFIERS
ident:
    switch(symbol_type) {
        case LETTER:
        case NUMBER:
            m_IdentifierStr += symbol;
            nextSymbol();
            goto ident;
        default:
            if (keywords.find(m_IdentifierStr) == keywords.end())
                return tok_identifier;
            return keywords[m_IdentifierStr];
    }

// NUMBERS
pre_num:
    switch (symbol) {
        case '.':
            m_FloatVal = m_IntVal;
            type = tok_numberFloat;
            nextSymbol();
            goto num_float;
        case 'x':
        case 'X':
            base = 16;
            nextSymbol();
    }
    goto num;

num:
    switch(symbol) {
        case '.':
            if (base != 10)
                error("Decimal numbers only supported in base 10!");

            m_FloatVal = (float)m_IntVal;
            type = tok_numberInt;
            nextSymbol();
            goto num_float;
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
            goto num;
        default:
            return type;
    }

pre_float:
    switch(symbol_type) {
        case NUMBER:
            goto num_float;
        default:
            error("Decimal point is not a number.");
    }

num_float:
    switch(symbol_type) {
        case NUMBER:
            digit = (symbol - '0');
            m_FloatVal += ((float)digit)/divider;
            divider *= 10;
            nextSymbol();
            goto num_float;
        default:
            return tok_numberFloat;
    }
}