#include "Lexer.hpp"
#include <map>


void error(std::string text) {
   std::cout << "\n" << text << std::endl;
   exit(1);
}


std::map<std::string, Token> Lexer::keywords = {
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
    {"do", tok_do},
    {"div", tok_div},
    {"mod", tok_mod},
};


void Lexer::nextSymbol() {
    symbol = getchar();

    // Set type
    if (isalpha(symbol) || symbol == '_')
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
 * the variable 'm_IdentifierStr' is set there in case of an identifier,
 * the variable 'm_intVal' is set there in case of an integer literal.
 * the variable 'm_floatVal' is set there in case of a float literal.
 * the variable 'm_stringVal' is set there in case of a string literal.
 */
int Lexer::nextToken() {
    int type;
    int base = 10;
    int divider = 10;
    int digit;

    m_IdentifierStr = m_stringVal = "";
    m_IntVal = m_FloatVal = 0;

// INITIAL STATE
init:
    switch (symbol) {
        case '{':
            nextSymbol();
            goto comment;
        case '(':
            nextSymbol();
            return tok_parenth_left;
        case ')':
            nextSymbol();
            return tok_parenth_right;
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
            return tok_multiply;
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
            return tok_comma;;
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
        default:;
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
        default:
            m_stringVal += symbol;
            nextSymbol();
            goto str;
    }
    switch(symbol_type) {
        case END:
            error("Unexpected end of file in a string literal.");
        default:
            nextSymbol();
            goto str;
    }

str_esc:
    switch(symbol) {
        case '}':
        case '\\':
            nextSymbol();
            goto str;
        default:;
            error("Unexpected escape symbol.");
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
    switch(symbol_type) {
        default:
            error("Expected '='.");
    }

// IDENTIFIERS
ident:
    switch(symbol_type) {
        case LETTER:
        case NUMBER:
        case '_':
            m_IdentifierStr += symbol;
            nextSymbol();
            goto ident;
        default:
            if (keywords.find(m_IdentifierStr) == keywords.end())
                return tok_identifier;
            return keywords[m_IdentifierStr];
    }

// NUMBERS
nint:
    switch(symbol) {
        case '.':
            if (base != 10)
                error("Decimal numbers only supported in base 10!");

            m_FloatVal = (float)m_IntVal;
            type = tok_floatLiteral;
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

            m_IntVal = base * m_IntVal + digit; 
            nextSymbol();
            goto nint;
        default:
            return type;
    }

dot:
    switch(symbol_type) {
        case NUMBER:
            type = tok_floatLiteral;
            goto nfloat;
        default:
            return tok_dot;
    }

nfloat:
    switch(symbol_type) {
        case NUMBER:
            digit = (symbol - '0');
            m_FloatVal += ( (float)digit ) / divider;
            divider *= 10;
            nextSymbol();
            goto nfloat;
        default:
            return tok_floatLiteral;
    }
}
