%{
#include <string>
#include "node.h"
#include "parser.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
%}
%option noyywrap

%%
[ \t\n]					;
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
"*"						return TOKEN(TMUL);
"/"						return TOKEN(TDIV);
"&&"					return TOKEN(TAND);
"||"					return TOKEN(TOR);
"!"						return TOKEN(TNOT);
[a-zA-Z_][a-zA-Z0-9_]* 	SAVE_TOKEN; return TIDENTIFICADOR;
[0-9]+\.[0-9]+ 			SAVE_TOKEN; return TDOUBLE;
[0-9]+					SAVE_TOKEN; return TENTERO;
.						std::cout << "Unknown token! " << yytext << std::endl; yyterminate();

%%
