#include "Lexer.hpp"
#include <map>


void error(std::string text) {
   std::cout << "\n" << text << std::endl;
   exit(1);
}


std::map<std::string, Token> Lexer::keywords = {
    {"program", tok_program},
    {"procedure", tok_procedure},
    {"function", tok_function},
    {"forward", tok_forward},
    {"exit", tok_exit},
    {"begin", tok_begin},
    {"end", tok_end},
    {"const", tok_const},
    {"var", tok_var},
    {"integer", tok_integer},
    {"real", tok_real},
    {"if", tok_if},
    {"then", tok_then},
    {"else", tok_else},
    {"while", tok_while},
    {"for", tok_for},
    {"do", tok_do},
    {"div", tok_div},
    {"mod", tok_mod},
    {"and", tok_and},
    {"or", tok_or},
};


void Lexer::nextSymbol() {
    symbol = getchar();

    // Set type
    if (symbol == '_')
        symbol_type = UNDERSCORE;
    else if (isalpha(symbol))
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


/**
 * @brief Function to return the next token from standard input
 *
 * the variable 'm_identifierStr' is set there in case of an identifier,
 * the variable 'm_intVal' is set there in case of an integer literal.
 * the variable 'm_floatVal' is set there in case of a float literal.
 * the variable 'm_stringVal' is set there in case of a string literal.
 */
Token Lexer::nextToken() {
    Token type;
    int base = 10;
    int divider = 10;
    int digit;

    m_identifierStr = m_stringVal = "";
    m_intVal = m_realVal = 0;

// INITIAL STATE
init:
    switch (symbol) {
        case '{':
            nextSymbol();
            goto comment;
        case '(':
            nextSymbol();
            return tok_lparen;
        case ')':
            nextSymbol();
            return tok_rparen;
        case ';':
            nextSymbol();
            return tok_semicolon;
        case '+':
            nextSymbol();
            return tok_plus;
        case '-':
            nextSymbol();
            return tok_minus;
        case '*':
            nextSymbol();
            return tok_star;
        case '<':
            nextSymbol();
            goto lesser_eq;
        case '>':
            nextSymbol();
            goto greater_eq;
        case '=':
            nextSymbol();
            return tok_equal;
        case ':':
            nextSymbol();
            goto colon;
        case ',':
            nextSymbol();
            return tok_comma;
        case '$':
            base = 16;
            type = tok_intLiteral;
            nextSymbol();
            goto nint;
        case '&':
            base = 8;
            type = tok_intLiteral;
            nextSymbol();
            goto nint;
        case '.':
            nextSymbol();
            goto dot;
        case '\'':
            nextSymbol();
            goto str;
    }
    switch (symbol_type) {
        case WHITE_SPACE:
            nextSymbol();
            goto init;
        case END:
            return tok_eof;
        case LETTER:
            m_identifierStr += symbol;
            nextSymbol();
            goto ident;
        case NUMBER:
            m_intVal = symbol - '0';
            type = tok_intLiteral;
            nextSymbol();
            goto nint;
        default:
            error("Invalid symbol.");
    }

// COMMENTS
comment:
    switch(symbol) {
        case '}':
            nextSymbol();
            goto init;
    }
    switch(symbol_type) {
        case END:
            error("Unexpected end of file in a comment.");
        default:
            nextSymbol();
            goto comment;
    }

// STRINGS
str:
    switch(symbol) {
        case '\'':
            nextSymbol();
            return tok_stringLiteral;
        case '\\':
            nextSymbol();
            goto str_esc;
    }
    switch(symbol_type) {
        case END:
            error("Unexpected end of file in a string literal.");
        default:
            m_stringVal += symbol;
            nextSymbol();
            goto str;
    }

str_esc:
    switch(symbol) {
        case 't':
            m_stringVal += '\t';
            nextSymbol();
            goto str;
        default:
            error("Unexpected escape symbol.");
    }

// LESSER, NOT EQUAL
lesser_eq:
    switch(symbol) {
        case '=':
            nextSymbol();
            return tok_le;
        case '>':
            nextSymbol();
            return tok_notequal;
    }
    switch(symbol_type) {
        default:
            return tok_lt;
    }

// GREATER
greater_eq:
    switch(symbol) {
        case '=':
            nextSymbol();
            return tok_ge;
    }
    switch(symbol_type) {
        default:
            return tok_gt;
    }

// COLON, ASSIGN
colon:
    switch(symbol) {
        case '=':
            nextSymbol();
            return tok_assign;
        default:
            nextSymbol();
            return tok_colon;
    }

// IDENTIFIERS
ident:
    switch(symbol_type) {
        case LETTER:
        case NUMBER:
        case UNDERSCORE:
            m_identifierStr += symbol;
            nextSymbol();
            goto ident;
        default:
            if (keywords.find(m_identifierStr) == keywords.end())
                return tok_identifier;
            return keywords[m_identifierStr];
    }

// NUMBERS
nint:
    switch(symbol) {
        case '.':
            if (base != 10)
                error("Decimal numbers only supported in base 10!");

            m_realVal = (float)m_intVal;
            type = tok_realLiteral;
            nextSymbol();
            goto nfloat;
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
                error("This digit is not allowed in this base!");

            m_intVal = base * m_intVal + digit; 
            nextSymbol();
            goto nint;
        default:
            return type;
    }

dot:
    switch(symbol_type) {
        case NUMBER:
            type = tok_realLiteral;
            goto nfloat;
        default:
            return tok_dot;
    }

nfloat:
    switch(symbol_type) {
        case NUMBER:
            digit = (symbol - '0');
            m_realVal += ( (float)digit ) / divider;
            divider *= 10;
            nextSymbol();
            goto nfloat;
        default:
            return tok_realLiteral;
    }
}
