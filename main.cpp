#include "Parser.hpp"

// Use tutorials in: https://llvm.org/docs/tutorial/

int main (int argc, char *argv[])
{
    Parser parser;

    if (!parser.Parse()) {
        return 1;
    }

    return 0;
}
