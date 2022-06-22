#include "../Lexer.hpp"
#include <map>


std::map<int, std::string> symbTable = {
    {tok_eof, "EOF"},
    {tok_dot, "DOT"},
    {tok_colon, "COLON"},
    {tok_comma, "COMMA"},
    {tok_identifier, "IDENT"},
    {tok_intLiteral, "INT_LITERAL"},
    {tok_floatLiteral, "FLOAT_LITERAL"},
    {tok_stringLiteral, "STRING_LITERAL"},
    {tok_begin, "kwBEGIN"},
    {tok_end, "kwEND"},
    {tok_const, "kwCONST"},
    {tok_procedure, "kwPROCEDURE"},
    {tok_forward, "kwFORWARD"},
    {tok_function, "kwFUNC"},
    {tok_if, "kwIF"},
    {tok_then, "kwTHEN"},
    {tok_else, "kwELSE"},
    {tok_program, "kwPROGRAM"},
    {tok_while, "kwWHILE"},
    {tok_exit, "kwEXIT"},
    {tok_var, "kwVAR"},
    {tok_integer, "kwINTEGER"},
    {tok_float, "kwFLOAT"},
    {tok_for, "kwFOR"},
    {tok_do, "kwDO"},
    {tok_readln, "kwREADLN"},
    {tok_write, "kwWRITE"},
    {tok_writeln, "kwWRITELN"},
    {tok_plus, "opPLUS"},
    {tok_minus, "opMINUS"},
    {tok_multiply, "opMULTIPLY"},
    {tok_lesser, "opLESSER"},
    {tok_greater, "opGREATER"},
    {tok_notequal, "opNOT_EQUAL"},
    {tok_lessequal, "opLESS_EQUAL"},
    {tok_greaterequal, "opGREAT_EQUAL"},
    {tok_assign, "opASSIGN"},
    {tok_equal, "opASSIGN_CONST"},
    {tok_or, "opOR"},
    {tok_mod, "opMOD"},
    {tok_div, "opDIV"},
    {tok_not, "opNOT"},
    {tok_and, "opAND"},
    {tok_xor, "opXOR"},
    {tok_to, "TO"},
    {tok_downto, "DOWNTO"},
    {tok_array, "ARRAY"},
    {tok_parenth_left, "PARENTH_LEFT"},
    {tok_parenth_right, "PARENTH_RIGHT"},
    {tok_semicolon, "SEMICOLON"}
};


void printSymb(Lexer & lexer, int token) {
   std::cout << "<" << symbTable[token];

   switch (token) {
   case tok_identifier:
      std::cout << ", " << lexer.identifierStr();
      break;
   case tok_intLiteral:
      std::cout << ", " << lexer.intVal();
      break;
   case tok_floatLiteral:
      std::cout << ", " << lexer.floatVal();
      break;
   case tok_stringLiteral:
      std::cout << ", '" << lexer.stringVal() << "'";
      break;
   default:
     break;
   }

   std::cout << ">" << std::endl;
}


int main(int argc, char *argv[]) {
    Lexer lexer;
    lexer.nextSymbol();
    int token;
    do {
        token = lexer.nextToken();
        printSymb(lexer, token);
    } while (token != tok_eof);

    return 0;
}
