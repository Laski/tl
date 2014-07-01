%{
	#include "node.h"
        #include <cstdio>
        #include <cstdlib>
	NPrograma *programBlock; /* the top level root Nodo of our final AST */

	extern int yylex();
	void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }
%}

/* Represents the many different ways we can access our data */
%union {
	Nodo *nodo;
	NPrograma *programa;
	NPloteo *plot;
	NBloque *block;
	NFuncion *func;
	NExpresion *expr;
	NCondicion *cond;
	NSentencia *sent;
	NIdentificador *ident;
	NLlamadaFuncion *llamfunc;
	std::vector<NIdentificador*> *varvec;
	std::vector<NFuncion*> *funcvec;
	std::vector<NExpresion*> *exprvec;
	std::vector<NSentencia*> *sentvec;
	std::string *string;
	int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the Nodo type
   they represent.
 */
 
%token <string> TIDENTIFICADOR TENTERO TDOUBLE
%token <token> TIGUALIGUAL TDISTINTO TMENOR TMENORIGUAL TMAYOR TMAYORIGUAL
%token <token> TPARENL TPARENR TLLAVEL TLLAVER TCOMA TPUNTOS
%token <token> TMAS TMENOS TMUL TDIV
%token <token> TAND TOR TNOT
%token <token> TRETURN TIF TTHEN TELSE TWHILE TFOR TFUNC TPLOT 
%token <token> TPI TIGUAL

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentificador*). It makes the compiler happy.
*/
 
%type <ident> nombre 
%type <expr> numero expresion termino factor expr_aritmetica
%type <varvec> argumentos
%type <exprvec> expresiones
%type <sentvec> sentencias
%type <funcvec> funciones
%type <programa> programa
%type <func> funcion
%type <llamfunc> llamada_funcion
%type <block> bloque
%type <sent> sentencia asignacion ifthenelse return while
%type <cond> condicion subcondicion
%type <plot> ploteo


/* Operator precedence for mathematical operators 
%left TPLUS TMINUS
%left TMUL TDIV
*/

%start programa

%%

programa : funciones ploteo								{ $$ = new NPrograma(*$1, *$2); } 
		;
		
funciones : funcion funciones								{ $2->push_back($1); }
	  | funcion 											{ $$ = new ListaFunciones(); $$->push_back($1); }
	  ;

funcion : TFUNC nombre TPARENL argumentos TPARENR bloque	{ $$ = new NFuncion(*$2, *$4, *$6); } 
		;

nombre : TIDENTIFICADOR					{ $$ = new NIdentificador(*$1); delete $1; }
	   ;
		
argumentos : /* lambda */				{ $$ = new ListaVariables(); }
		| nombre						{ $$ = new ListaVariables(); $$->push_back($1); }
		| argumentos TCOMA nombre		{ $1->push_back($3); }
		;

bloque : sentencia						{ ListaSentencias *aux = new ListaSentencias; aux->push_back($1); $$ = new NBloque(*aux); } 
	| TLLAVEL sentencias TLLAVER		{ $$ = new NBloque(*$2); }
	| TLLAVEL TLLAVER					{ $$ = new NBloque(); }		
	;

sentencias : sentencia					{ $$ = new ListaSentencias(); $$->push_back($1); }
	| sentencias sentencia				{ $1->push_back($2); }
	;
	
sentencia : asignacion					{ $$ = $1; }
	| ifthenelse						{ $$ = $1; }
	| while								{ $$ = $1; }
	| return							{ $$ = $1; }
	;
	
asignacion : nombre TIGUAL expresion	{ $$ = new NAsignacion(*$1, *$3); }
		   ;
		   
ifthenelse : TIF condicion TTHEN bloque	{ $$ = new NIfThen(*$2, *$4); }
	| TIF condicion TTHEN bloque TELSE bloque { $$ = new NIfThenElse(*$2, *$4, *$6); }
	;
	
while : TWHILE condicion bloque{ $$ = new NWhile(*$2, *$3); }
	   ;
	   
return : TRETURN expresion		{ $$ = new NReturn(*$2); }
		;
		
expresiones : /* lambda */		{ $$ = new ListaExpresiones(); }
	| expresiones TCOMA expresion{ $1->push_back($3); }
	| expresion					{ $$ = new ListaExpresiones(); $$->push_back($1); }
	;
	
expresion : expr_aritmetica		{ $$ = $1; }
	| TMENOS expr_aritmetica	{ $$ = new NOperacionAritmetica(MENOS, *(new NEntero(0)), *$2); } // -x es 0-x
	| llamada_funcion			{ $$ = $1;}
	;

expr_aritmetica : factor		{ $$ = $1; }
	| factor TMUL factor		{ $$ = new NOperacionAritmetica(MUL, *$1, *$3); }
	| factor TDIV factor		{ $$ = new NOperacionAritmetica(DIV, *$1, *$3); }
	;

factor : termino				{ $$ = $1; }
	| termino TMAS termino		{ $$ = new NOperacionAritmetica(MAS, *$1, *$3); }
	| termino TMENOS termino	{ $$ = new NOperacionAritmetica(MENOS, *$1, *$3); }
	;
	
termino : numero				{ $$ = $1; }
	| TPARENL expresion TPARENR	{ $$ = $2; }
	;

numero : TENTERO				{ $$ = new NEntero(atol($1->c_str())); delete $1; }
	| TDOUBLE					{ $$ = new NDouble(atof($1->c_str())); delete $1; }
	| TPI						{ $$ = new NDouble(3.141592); }
	| nombre					{ $$ = new NIdentificador(*$1); delete $1; }
	;

llamada_funcion : nombre TPARENL expresiones TPARENR	{ $$ = new NLlamadaFuncion(*$1, *$3); delete $3; }

condicion : 
	 TNOT subcondicion									{ $$ = new NOperacionBooleana(*$2); } 
	| condicion TAND subcondicion						{ $$ = new NOperacionBooleana(AND, *$1, *$3); } 
	| condicion TOR subcondicion						{ $$ = new NOperacionBooleana(OR, *$1, *$3); } 
	| expresion TIGUAL expresion						{ $$ = new NComparacion(IGUAL, *$1, *$3); } 
	| expresion TDISTINTO expresion						{ $$ = new NComparacion(DISTINTO, *$1, *$3); } 
	| expresion TMENOR expresion						{ $$ = new NComparacion(MENOR, *$1, *$3); } 
	| expresion TMENORIGUAL expresion					{ $$ = new NComparacion(MENORIGUAL, *$1, *$3); } 
	| expresion TMAYOR expresion						{ $$ = new NComparacion(MAYOR, *$1, *$3); } 
	| expresion TMAYORIGUAL expresion					{ $$ = new NComparacion(MAYORIGUAL, *$1, *$3); } 
	;

subcondicion: TPARENL condicion TPARENR					{ $$ = $2; }
			;
			
ploteo : 
	TPLOT TPARENL llamada_funcion TCOMA llamada_funcion TPARENR TFOR TIDENTIFICADOR TIGUAL expresion TPUNTOS expresion TPUNTOS expresion 
	{ $$ = new NPloteo(*$3, *$5, *$10, *$12, *$14); }
	   ; 
%%
