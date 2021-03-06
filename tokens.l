%{
#include <string>
#include "node.h"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
int linea = 1;
%}
%option noyywrap

%%
[ \t]					;
[\n]					DEBUG_OUT("Linea: " << linea); linea++;
"/*"([^*]|[\r\n]|("*"+([^*/]|[\r\n])))*"*"+"/" ;
"//".*					;
"return"				return TOKEN(TRETURN);
"if"					return TOKEN(TIF);
"then"					return TOKEN(TTHEN);
"else"					return TOKEN(TELSE); 
"while"					return TOKEN(TWHILE);
"for"					return TOKEN(TFOR);
"function"				return TOKEN(TFUNC);
"plot"					return TOKEN(TPLOT);
"="						return TOKEN(TIGUAL);
"=="					return TOKEN(TIGUALIGUAL);
"!="					return TOKEN(TDISTINTO);
"<"						return TOKEN(TMENOR);
"<="					return TOKEN(TMENORIGUAL);
">"						return TOKEN(TMAYOR);
">="					return TOKEN(TMAYORIGUAL);
"("						return TOKEN(TPARENL);
")"						return TOKEN(TPARENR);
"{"						return TOKEN(TLLAVEL);
"}"						return TOKEN(TLLAVER);
".."					return TOKEN(TPUNTOS);
","						return TOKEN(TCOMA);
"+"						return TOKEN(TMAS);
"-"						return TOKEN(TMENOS);
"**"					return TOKEN(TPOT);
"*"						return TOKEN(TMUL);
"/"						return TOKEN(TDIV);
"&&"					return TOKEN(TAND);
"||"					return TOKEN(TOR);
"!"						return TOKEN(TNOT);
"pi"					return TOKEN(TPI);
[a-zA-Z_][a-zA-Z0-9_]* 	SAVE_TOKEN; return TIDENTIFICADOR;
[0-9]+\.[0-9]+ 			SAVE_TOKEN; return TDOUBLE;
[0-9]+					SAVE_TOKEN; return TDOUBLE;
.						std::cerr << "Unknown token! " << yytext << std::endl; yyterminate();

%%
