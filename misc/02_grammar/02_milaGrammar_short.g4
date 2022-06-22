grammar mila;

program
   : PROGRAM IDENT SEMI
     block
     DOT
   ;

block
   : (constantDefinition | variableDeclarationPart | procedureAndFunctionDeclarationPart)*
     compoundStatement
   ;

/* constantDefinitionPart */
constantDefinition
   : CONST (IDENT EQUAL constant SEMI)+
   ;

constant
   : (sign)? unsignedNumber
   | (sign)? IDENT
   | STRING_LITERAL
   ;

unsignedNumber
   : NUM_INT
   | NUM_REAL
   ;

sign
   : PLUS
   | MINUS
   ;

/* variableDeclarationPart */
variableDeclarationPart
   : VAR variableDeclaration (SEMI variableDeclaration)* SEMI
   ;

variableDeclaration
   : identifierList COLON typeIdentifier
   ;

identifierList
   : IDENT (COMMA IDENT)*
   ;

typeIdentifier
   : IDENT
   | (INTEGER | REAL | STRING)
   ;

/* procedureAndFunctionDeclarationPart */
procedureAndFunctionDeclarationPart
   : procedureOrFunctionDeclaration SEMI
   ;

procedureOrFunctionDeclaration
   : procedureDeclaration
   | functionDeclaration
   ;

procedureDeclaration
   : PROCEDURE IDENT (formalParameterList)? SEMI block
   ;

formalParameterList
   : LPAREN parameterGroup (SEMI parameterGroup)* RPAREN
   ;

parameterGroup
   : identifierList COLON typeIdentifier
   ;

functionDeclaration
   : FUNCTION IDENT (formalParameterList)? COLON typeIdentifier SEMI block
   ;

/* compoundStatement */
statement
   : simpleStatement
   | structuredStatement
   ;

simpleStatement
   : assignmentStatement
   | procedureStatement
   | emptyStatement_
   ;

assignmentStatement
   : IDENT ASSIGN expression
   ;

procedureStatement
   : IDENT (LPAREN parameterList RPAREN)?
   ;

structuredStatement
   : compoundStatement
   | ifStatement
   | repetetiveStatement
   ;

compoundStatement
   : BEGIN  statement (SEMI statement)* END
   ;

ifStatement
   : IF expression THEN statement (: ELSE statement)?
   ;

repetetiveStatement
   : whileStatement
   | forStatement
   ;

whileStatement
   : WHILE expression DO statement
   ;

forStatement
   : FOR IDENT ASSIGN expression TO expression DO statement
   ;

emptyStatement_
   :
   ;

/* expression */
expression
   : simpleExpression (relationaloperator expression)?
   ;

relationaloperator
   : EQUAL
   | NOT_EQUAL
   | LT
   | LE
   | GE
   | GT
   ;

simpleExpression
   : term (additiveoperator simpleExpression)?
   ;

additiveoperator
   : PLUS
   | MINUS
   | OR
   ;

term
   : signedFactor (multiplicativeoperator term)?
   ;

multiplicativeoperator
   : STAR
   | DIV
   | MOD
   | AND
   ;

signedFactor
   : (PLUS | MINUS)? factor
   ;

factor
   : IDENT
   | LPAREN expression RPAREN
   | unsignedConstant
   | NOT factor
   ;

unsignedConstant
   : unsignedNumber
   | STRING_LITERAL
   ;

parameterList
   : expression (COMMA expression)*
   ;

/* --------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

AND
   : 'AND'
   ;

BEGIN
   : 'BEGIN'
   ;

CONST
   : 'CONST'
   ;

DIV
   : 'DIV'
   ;

DO
   : 'DO'
   ;

ELSE
   : 'ELSE'
   ;

END
   : 'END'
   ;

FOR
   : 'FOR'
   ;

FUNCTION
   : 'FUNCTION'
   ;

IF
   : 'IF'
   ;

INTEGER
   : 'INTEGER'
   ;

MOD
   : 'MOD'
   ;

NOT
   : 'NOT'
   ;

OR
   : 'OR'
   ;

PROCEDURE
   : 'PROCEDURE'
   ;

PROGRAM
   : 'PROGRAM'
   ;

REAL
   : 'REAL'
   ;

THEN
   : 'THEN'
   ;

TO
   : 'TO'
   ;

VAR
   : 'VAR'
   ;

WHILE
   : 'WHILE'
   ;

PLUS
   : '+'
   ;

MINUS
   : '-'
   ;

STAR
   : '*'
   ;

ASSIGN
   : ':='
   ;

COMMA
   : ','
   ;

SEMI
   : ';'
   ;

COLON
   : ':'
   ;

EQUAL
   : '='
   ;

NOT_EQUAL
   : '<>'
   ;

LT
   : '<'
   ;

LE
   : '<='
   ;

GE
   : '>='
   ;

GT
   : '>'
   ;

LPAREN
   : '('
   ;

RPAREN
   : ')'
   ;

DOT
   : '.'
   ;

STRING
   : 'STRING'
   ;

TRUE
   : 'TRUE'
   ;

FALSE
   : 'FALSE'
   ;

WS
   : [ \t\r\n] -> skip
   ;

IDENT
   : ('A' .. 'Z') ('A' .. 'Z' | '0' .. '9' | '_')*
   ;

STRING_LITERAL
   : '\'' ('\'\'' | ~ ('\''))* '\''
   ;

NUM_INT
   : ('0' .. '9') +
   ;

NUM_REAL
   : ('0' .. '9') + (('.' ('0' .. '9') + (EXPONENT)?)? | EXPONENT)
   ;

fragment EXPONENT
   : ('E') ('+' | '-')? ('0' .. '9') +
   ;
