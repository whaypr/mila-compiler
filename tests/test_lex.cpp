#include "../compiler/lexer/lexer.hpp"
#include <map>


std::map<int, std::string> symbTable = {
    {tok_eof, "EOF"},
    {tok_dot, "DOT"},
    {tok_colon, "COLON"},
    {tok_comma, "COMMA"},
    {tok_identifier, "IDENT"},
    {tok_intLiteral, "INT_LITERAL"},
    {tok_realLiteral, "REAL_LITERAL"},
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
    {tok_real, "kwFLOAT"},
    {tok_for, "kwFOR"},
    {tok_do, "kwDO"},
    {tok_readln, "kwREADLN"},
    {tok_write, "kwWRITE"},
    {tok_writeln, "kwWRITELN"},
    {tok_plus, "opPLUS"},
    {tok_minus, "opMINUS"},
    {tok_star, "opSTAR"},
    {tok_lt, "opLESSER"},
    {tok_gt, "opGREATER"},
    {tok_notequal, "opNOT_EQUAL"},
    {tok_le, "opLESS_EQUAL"},
    {tok_ge, "opGREAT_EQUAL"},
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
    {tok_lparen, "PARENTH_LEFT"},
    {tok_rparen, "PARENTH_RIGHT"},
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
   case tok_realLiteral:
      std::cout << ", " << lexer.realVal();
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
