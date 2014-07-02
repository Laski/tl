%{
	#include "node.h"
        #include <cstdio>
        #include <cstdlib>
	#include <string>
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
	NNumero *num;
	NCondicion *cond;
	NSentencia *sent;
	NIdentificador *ident;
	NLlamadaFuncion *llamfunc;
	std::vector<NIdentificador*> *varvec;
	std::map<std::string,NFuncion*> *funcdicc;
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
%type <expr> expresion termino factor expr_aritmetica
%type <num> numero
%type <varvec> argumentos
%type <exprvec> expresiones
%type <sentvec> sentencias
%type <funcdicc> funciones
%type <programa> programa
%type <func> funcion
%type <llamfunc> llamada_funcion
%type <block> bloque
%type <sent> sentencia asignacion ifthenelse return while
%type <cond> condicion
%type <plot> ploteo


/* Operator precedence for mathematical operators 
%left TPLUS TMINUS
%left TMUL TDIV
*/

%start programa

%%

programa : funciones ploteo								{std::cerr << "programa" << std::endl; $$ = new NPrograma(*$1, *$2); programBlock = $$; } 
		;
	
//Acá usamos el diccionario. Por alguna razon no compila si usamos el operator[], por lo que tuve que usar insert y quedo más feo.	
funciones : funciones funcion								{std::cerr << "funciones" << std::endl; 
																if($1->find($2->id.nombre) != $1->end()){
																	std::cerr << "ERROR: funcion '" << $2->id.nombre << "' declarada dos veces." << std::endl;
																	YYABORT;
																}else 
																	$1->insert(std::pair<std::string, NFuncion*>($2->id.nombre, $2)); }
	  | funcion 											{ std::cerr << "funcion" << std::endl; $$ = new DiccFunciones(); 
	  															$$->insert(std::pair<std::string, NFuncion*>($1->id.nombre, $1) ); }
	  ;

funcion : TFUNC nombre TPARENL argumentos TPARENR bloque	{ std::cerr << "nueva funcion" << std::endl; $$ = new NFuncion(*$2, *$4, *$6); } 
		;

nombre : TIDENTIFICADOR					{ std::cerr << "nombre" << std::endl; $$ = new NIdentificador(*$1); delete $1; }
	   ;
		
argumentos : /* lambda */				{ std::cerr << "argumentos vacio" << std::endl; $$ = new ListaVariables(); }
		| nombre						{ std::cerr << "argumento uno" << std::endl; $$ = new ListaVariables(); $$->push_back($1); }
		| argumentos TCOMA nombre		{ std::cerr << "argumentos muchos" << std::endl; $1->push_back($3); }
		;

bloque : sentencia						{ std::cerr << "bloque sentencia" << std::endl; ListaSentencias *aux = new ListaSentencias; aux->push_back($1); $$ = new NBloque(*aux); } 
	| TLLAVEL sentencias TLLAVER		{ std::cerr << "bloque sentencias" << std::endl; $$ = new NBloque(*$2); }
	| TLLAVEL TLLAVER					{ std::cerr << "bloque vacio" << std::endl; $$ = new NBloque(); }		
	;

sentencias : sentencia					{ std::cerr << "sentencias una" << std::endl; $$ = new ListaSentencias(); $$->push_back($1); }
	| sentencias sentencia				{ std::cerr << "sentencias muchas" << std::endl; $1->push_back($2); }
	;
	
sentencia : asignacion					{ std::cerr << "sentencia asignacion" << std::endl; $$ = $1; }
	| ifthenelse						{ std::cerr << "sentencia ifthenelse" << std::endl; $$ = $1; }
	| while								{ std::cerr << "sentencia while" << std::endl; $$ = $1; }
	| return							{ std::cerr << "sentencia return" << std::endl; $$ = $1; }
	;
	
asignacion : nombre TIGUAL expresion	{ std::cerr << "asignacion" << std::endl; $$ = new NAsignacion(*$1, *$3); }
		   ;
		   
ifthenelse : TIF condicion TTHEN bloque	{ std::cerr << "ifthen" << std::endl; $$ = new NIfThen(*$2, *$4); }
	| TIF condicion TTHEN bloque TELSE bloque { std::cerr << "ifthenelse" << std::endl; $$ = new NIfThenElse(*$2, *$4, *$6); }
	;
	
while : TWHILE condicion bloque{ std::cerr << "while" << std::endl; $$ = new NWhile(*$2, *$3); }
	   ;
	   
return : TRETURN expresion		{ std::cerr << "return" << std::endl; $$ = new NReturn(*$2); }
		;

/*http://www-h.eng.cam.ac.uk/help/tpl/languages/flexbison/*/

expresiones : /* lambda */		{ std::cerr << "expresiones vacia" << std::endl; $$ = new ListaExpresiones(); }
	| expresiones TCOMA expresion{ std::cerr << "expresiones varias" << std::endl; $1->push_back($3); }
	| expresion					{ std::cerr << "expresiones una" << std::endl; $$ = new ListaExpresiones(); $$->push_back($1); }
	;
	
expresion : expr_aritmetica		{ std::cerr << "expresion aritmetica" << std::endl; $$ = $1; }
	| TMENOS expr_aritmetica	{ std::cerr << "expresion: menos aritmetica" << std::endl; $$ = new NOperacionAritmetica(MENOS, *(new NEntero(0)), *$2); } // -x es 0-x
	;

expr_aritmetica : factor		{ std::cerr << "expr_aritmetica factor" << std::endl; $$ = $1; }
	| factor TMUL factor		{ std::cerr << "expr_aritmetica mul" << std::endl; $$ = new NOperacionAritmetica(MUL, *$1, *$3); }
	| factor TDIV factor		{ std::cerr << "expr_aritmetica div" << std::endl; $$ = new NOperacionAritmetica(DIV, *$1, *$3); }
	;

factor : termino				{ std::cerr << "factor termino" << std::endl; $$ = $1; }
	| termino TMAS termino		{ std::cerr << "factor mas" << std::endl; $$ = new NOperacionAritmetica(MAS, *$1, *$3); }
	| termino TMENOS termino	{ std::cerr << "factor menos" << std::endl; $$ = new NOperacionAritmetica(MENOS, *$1, *$3); }
	;
	
termino : numero				{ std::cerr << "termino numero" << std::endl; $$ = $1; }
	| nombre					{ std::cerr << "termino nombre" << std::endl; $$ = new NIdentificador(*$1); delete $1; }
	| llamada_funcion			{ std::cerr << "termino llamada_funcion" << std::endl; $$ = $1;}
	| TPARENL expresion TPARENR	{ std::cerr << "termino expresion" << std::endl; $$ = $2; }
	;

numero : TENTERO				{ std::cerr << "num entero: " << atol($1->c_str()) << std::endl; $$ = new NEntero(); $$->value = atol($1->c_str()); delete $1; }
	| TDOUBLE					{ std::cerr << "num doble: " << atof($1->c_str()) << std::endl; $$ = new NDouble(); $$-> value = atof($1->c_str()); delete $1; }
	| TPI						{ std::cerr << "num pi" << std::endl; $$ = new NDouble(); $$->value = 3.141592; }
	;

llamada_funcion : nombre TPARENL expresiones TPARENR	{ std::cerr << "llamada_funcion" << std::endl; $$ = new NLlamadaFuncion(*$1, *$3); delete $3; }

condicion : 
	 TNOT condicion										{ std::cerr << "condicion not" << std::endl; $$ = new NOperacionBooleana(*$2); } 
	| condicion TAND condicion							{ std::cerr << "condicion and" << std::endl; $$ = new NOperacionBooleana(AND, *$1, *$3); } 
	| condicion TOR condicion							{ std::cerr << "condicion or" << std::endl; $$ = new NOperacionBooleana(OR, *$1, *$3); } 
	| expresion TIGUALIGUAL expresion					{ std::cerr << "condicion igual" << std::endl; $$ = new NComparacion(IGUAL, *$1, *$3); } 
	| expresion TDISTINTO expresion						{ std::cerr << "condicion distinto" << std::endl; $$ = new NComparacion(DISTINTO, *$1, *$3); } 
	| expresion TMENOR expresion						{ std::cerr << "condicion menor" << std::endl; $$ = new NComparacion(MENOR, *$1, *$3); } 
	| expresion TMENORIGUAL expresion					{ std::cerr << "condicion menorigual" << std::endl; $$ = new NComparacion(MENORIGUAL, *$1, *$3); } 
	| expresion TMAYOR expresion						{ std::cerr << "condicion mayor" << std::endl; $$ = new NComparacion(MAYOR, *$1, *$3); } 
	| expresion TMAYORIGUAL expresion					{ std::cerr << "condicion mayorigual" << std::endl; $$ = new NComparacion(MAYORIGUAL, *$1, *$3); } 
	;
		
ploteo : 
	TPLOT TPARENL llamada_funcion TCOMA llamada_funcion TPARENR TFOR TIDENTIFICADOR TIGUAL numero TPUNTOS numero TPUNTOS numero 
	{ std::cerr << "ploteo " << $10->value << std::endl; $$ = new NPloteo(*$3, *$5, *$10, *$12, *$14); }
	   ; 
%%
