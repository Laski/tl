%{
	#include "node.h"
        #include <cstdio>
        #include <cstdlib>
	#include <string>
	NPrograma *programBlock; /* the top level root Nodo of our final AST */

	extern int yylex();
	void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }

	#define DEBUG 1
	#define DEBUG_OUT(s) (DEBUG == 1) ? std::cerr << s << std::endl : std::cerr << ""
	#define ERROR_OUT(s) std::cout << s << std::endl
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
%type <expr> expresion
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


/* Operator precedence for mathematical operators */
%left TMAS TMENOS
%left TMUL TDIV
%left NEG


%start programa

%%

programa : funciones ploteo								{DEBUG_OUT("programa -> funciones ploteo"); $$ = new NPrograma(*$1, *$2); programBlock = $$; } 
		;
	
// Acá usamos el diccionario. Por alguna razon no compila si usamos el operator[], por lo que tuve que usar insert y quedo más feo.	
funciones : funciones funcion								{DEBUG_OUT("funciones -> funciones funcion"); 
																if($1->find($2->id.nombre) != $1->end()){
																	ERROR_OUT("ERROR: funcion '" << $2->id.nombre << "' declarada dos veces.");
																	YYABORT;
																}else 
																	$1->insert(std::pair<std::string, NFuncion*>($2->id.nombre, $2)); }
	  | funcion 											{ DEBUG_OUT("funciones -> funcion" ); $$ = new DiccFunciones(); 
	  															$$->insert(std::pair<std::string, NFuncion*>($1->id.nombre, $1) ); }
	  ;

funcion : TFUNC nombre TPARENL argumentos TPARENR bloque	{ DEBUG_OUT("funcion -> nombre argumentos bloque" ); $$ = new NFuncion(*$2, *$4, *$6); } 
		;

nombre : TIDENTIFICADOR					{ DEBUG_OUT("nombre -> '" << *$1 << "'"); $$ = new NIdentificador(*$1); delete $1; }
	   ;
		
argumentos : /* lambda */				{ DEBUG_OUT("argumentos -> lambda"); $$ = new ListaVariables(); }
		| nombre						{ DEBUG_OUT("argumentos -> nombre"); $$ = new ListaVariables(); $$->push_back($1); }
		| argumentos TCOMA nombre		{ DEBUG_OUT("argumentos -> argumentos nombre"); $1->push_back($3); }
		;

bloque : sentencia						{ DEBUG_OUT("bloque -> sentencia"); ListaSentencias *aux = new ListaSentencias; aux->push_back($1); $$ = new NBloque(*aux); } 
	| TLLAVEL sentencias TLLAVER		{ DEBUG_OUT("bloque -> sentencias"); $$ = new NBloque(*$2); }
	| TLLAVEL TLLAVER					{ DEBUG_OUT("bloque -> vacio"); $$ = new NBloque(); }		
	;

sentencias : sentencia					{ DEBUG_OUT("sentencias -> sentencia"); $$ = new ListaSentencias(); $$->push_back($1); }
	| sentencias sentencia				{ DEBUG_OUT("sentencias -> sentencias sentencia"); $1->push_back($2); }
	;
	
sentencia : asignacion					{ DEBUG_OUT("sentencia -> asignacion"); $$ = $1; }
	| ifthenelse						{ DEBUG_OUT("sentencia -> ifthenelse"); $$ = $1; }
	| while								{ DEBUG_OUT("sentencia -> while"); $$ = $1; }
	| return							{ DEBUG_OUT("sentencia -> return"); $$ = $1; }
	;
	
asignacion : nombre TIGUAL expresion	{ DEBUG_OUT("asignacion -> nombre expresion"); $$ = new NAsignacion(*$1, *$3); }
		   ;
		   
ifthenelse : TIF condicion TTHEN bloque	{ DEBUG_OUT("ifthenelse -> IFTHEN condicion bloque"); $$ = new NIfThen(*$2, *$4); }
	| TIF condicion TTHEN bloque TELSE bloque { DEBUG_OUT("ifthenelse -> IFTHENELSE condicion bloque bloque"); $$ = new NIfThenElse(*$2, *$4, *$6); }
	;
	
while : TWHILE condicion bloque{ DEBUG_OUT("while -> condicion bloque"); $$ = new NWhile(*$2, *$3); }
	   ;
	   
return : TRETURN expresion		{ DEBUG_OUT("return -> expresion"); $$ = new NReturn(*$2); }
		;


expresiones : /* lambda */			{ DEBUG_OUT("expresiones -> lambda"); $$ = new ListaExpresiones(); }
	| expresiones TCOMA expresion 	{ DEBUG_OUT("expresiones -> expresiones expresion"); $1->push_back($3); }
	| expresion						{ DEBUG_OUT("expresiones -> expresion"); $$ = new ListaExpresiones(); $$->push_back($1); }
	;

/*http://www-h.eng.cam.ac.uk/help/tpl/languages/flexbison/*/

expresion:
		 numero							{ DEBUG_OUT("expresion -> numero"); $$ = $1; }		
		| nombre						{ DEBUG_OUT("expresion -> nombre"); $$ = new NIdentificador(*$1); delete $1; }
		| llamada_funcion				{ DEBUG_OUT("expresion -> llamada_funcion"); $$ = $1;}
		| expresion TMAS expresion 		{ DEBUG_OUT("expresion -> expresion + expresion"); $$ = new NOperacionAritmetica(MAS, *$1, *$3); }
		| expresion TMENOS expresion 	{ DEBUG_OUT("expresion -> expresion - expresion"); $$ = new NOperacionAritmetica(MENOS, *$1, *$3); }
		| expresion TMUL expresion 		{ DEBUG_OUT("expresion -> expresion * expresion"); $$ = new NOperacionAritmetica(MUL, *$1, *$3); }
		| expresion TDIV expresion 		{ DEBUG_OUT("expresion -> expresion / expresion"); $$ = new NOperacionAritmetica(DIV, *$1, *$3); }
		| TMENOS expresion %prec NEG 	{ DEBUG_OUT("expresion -> -expresion"); $$ = new NOperacionAritmetica(MENOS, *(new NEntero(0)), *$2); } // -x es 0-x
		| TPARENL expresion TPARENR 	{ DEBUG_OUT("expresion -> (expresion)"); $$ = $2; }
		;

numero : TENTERO				{ DEBUG_OUT("numero -> TENTERO: " << atol($1->c_str())); $$ = new NEntero(); $$->value = atol($1->c_str()); delete $1; }
	| TDOUBLE					{ DEBUG_OUT("numero -> TDOUBLE: " << atof($1->c_str())); $$ = new NDouble(); $$-> value = atof($1->c_str()); delete $1; }
	| TPI						{ DEBUG_OUT("numero -> TPI"); $$ = new NDouble(); $$->value = 3.141592; }
	;

llamada_funcion : nombre TPARENL expresiones TPARENR	{ DEBUG_OUT("llamada_funcion -> nombre expresiones"); $$ = new NLlamadaFuncion(*$1, *$3); delete $3; }

condicion : 
	 TNOT condicion										{ DEBUG_OUT("condicion -> NOT condicion"); $$ = new NOperacionBooleana(*$2); } 
	| condicion TAND condicion							{ DEBUG_OUT("condicion -> condicion AND condicion"); $$ = new NOperacionBooleana(AND, *$1, *$3); } 
	| condicion TOR condicion							{ DEBUG_OUT("condicion -> condicion OR condicion"); $$ = new NOperacionBooleana(OR, *$1, *$3); } 
	| expresion TIGUALIGUAL expresion					{ DEBUG_OUT("condicion -> expresion == expresion"); $$ = new NComparacion(IGUAL, *$1, *$3); } 
	| expresion TDISTINTO expresion						{ DEBUG_OUT("condicion -> expresion != expresion"); $$ = new NComparacion(DISTINTO, *$1, *$3); } 
	| expresion TMENOR expresion						{ DEBUG_OUT("condicion -> expresion < expresion"); $$ = new NComparacion(MENOR, *$1, *$3); } 
	| expresion TMENORIGUAL expresion					{ DEBUG_OUT("condicion -> expresion <= expresion"); $$ = new NComparacion(MENORIGUAL, *$1, *$3); } 
	| expresion TMAYOR expresion						{ DEBUG_OUT("condicion -> expresion > expresion"); $$ = new NComparacion(MAYOR, *$1, *$3); } 
	| expresion TMAYORIGUAL expresion					{ DEBUG_OUT("condicion -> expresion >= expresion"); $$ = new NComparacion(MAYORIGUAL, *$1, *$3); } 
	;
		
ploteo : 
	TPLOT TPARENL llamada_funcion TCOMA llamada_funcion TPARENR TFOR TIDENTIFICADOR TIGUAL expresion TPUNTOS expresion TPUNTOS expresion 
	{ DEBUG_OUT("ploteo -> llamada_funcion llamada_funcion numero numero numero "); $$ = new NPloteo(*$3, *$5, *$10, *$12, *$14); }
	   ; 
%%
