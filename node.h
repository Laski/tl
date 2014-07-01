#include <iostream>
#include <vector>
#include <map>
#include <string>

class NSentencia;
class NExpresion;
class NFuncion;
class NIdentificador;
class NPloteo;
class NBloque;
class NLlamadaFuncion;
class NCondicion;
class NDouble;
class NNumero;

typedef std::map<std::string,NFuncion*> DiccFunciones;
typedef std::vector<NSentencia*> ListaSentencias;
typedef std::vector<NExpresion*> ListaExpresiones;
typedef std::vector<NIdentificador*> ListaVariables;

#define NOT 	0
#define AND 	1
#define OR  	2
#define IGUAL  	3
#define MAYOR  	4
#define MENOR  	5
#define MENORIGUAL  	6
#define MAYORIGUAL  	7
#define DISTINTO  		8
#define MAS  	9
#define MENOS  	10
#define MUL  	11
#define DIV  	12

class Nodo {
public:
	virtual ~Nodo() {}
};

class NPrograma : public Nodo { 
public:
	DiccFunciones funciones;
	NPloteo& ploteo;
	NPrograma(DiccFunciones funciones, NPloteo& ploteo) : 
		funciones(funciones), ploteo(ploteo) { }
};

class NFuncion : public Nodo {
public:
	const NIdentificador& id;
	ListaVariables argumentos;
	NBloque& bloque;
	NFuncion(const NIdentificador& id, 
			const ListaVariables& argumentos, NBloque& bloque) :
		id(id), argumentos(argumentos), bloque(bloque) { }
};

class NBloque : public Nodo {
public:
	ListaSentencias& sentencias;
	NBloque() : sentencias(*(new ListaSentencias())) {};
	NBloque(ListaSentencias& sentencias) :
		sentencias(sentencias) { };
};

class NPloteo : public Nodo {
public:
	NLlamadaFuncion& func1;
	NLlamadaFuncion& func2;
	NNumero& desde;
	NNumero& paso;
	NNumero& hasta;
	NPloteo(NLlamadaFuncion& func1, NLlamadaFuncion& func2, NNumero& desde, NNumero& paso, NNumero& hasta) : 
		func1(func1), func2(func2), desde(desde), paso(paso), hasta(hasta) { }
};

/***********************************************************/
/***********************************************************/
/*********************** SENTENCIAS ************************/
/***********************************************************/
/***********************************************************/

class NSentencia : public Nodo {
};

class NReturn : public NSentencia {
public:
	NExpresion& expresion;
	NReturn(NExpresion& expresion) : 
		expresion(expresion) { }
};

class NIf : public NSentencia {
};

class NIfThen : public NIf {
public:
	NCondicion& cond;
	NBloque& bloque;
	NIfThen(NCondicion& cond, NBloque& bloque) : 
		cond(cond), bloque(bloque) { }	
};

class NIfThenElse : public NIf {
public:
	NCondicion& cond;
	NBloque& bloqueT;
	NBloque& bloqueF;
	NIfThenElse(NCondicion& cond, NBloque& bloqueT, NBloque& bloqueF) : 
		cond(cond), bloqueT(bloqueT), bloqueF(bloqueF) { }	
};

class NWhile : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloque;
	NWhile(NCondicion& cond, NBloque& bloque) : 
		cond(cond), bloque(bloque) { }
};

class NAsignacion : public NSentencia {
public:
	NIdentificador& lhs;
	NExpresion& rhs;
	NAsignacion(NIdentificador& lhs, NExpresion& rhs) : 
		lhs(lhs), rhs(rhs) { }
};

/***********************************************************/
/***********************************************************/
/*********************** EXPRESIONES ***********************/
/***********************************************************/
/***********************************************************/

class NExpresion : public Nodo {
};

class NNumero : public NExpresion{
public:
	double value;
};

class NLlamadaFuncion : public NExpresion {
public:
	const NIdentificador& id;
	ListaExpresiones argumentos;
	NLlamadaFuncion(const NIdentificador& id, ListaExpresiones& argumentos) :
		id(id), argumentos(argumentos) { }
};

class NOperacionAritmetica : public NExpresion {
public:
	int cod_op;
	NExpresion& expr1;
	NExpresion& expr2;
	NOperacionAritmetica(int cod_op, NExpresion& expr1, NExpresion& expr2) :
		expr1(expr1), expr2(expr2), cod_op(cod_op) { }
};

class NEntero : public NNumero {
public:
	long long value;
	NEntero(long long val) : value(val) { }; //Este constructor no carga el valor en value
	NEntero(){};
};

class NDouble : public NNumero {
public:
	double value;
	NDouble(double val) : value(val) { } //Este constructor no carga el valor en value
	NDouble(){};
};

class NIdentificador : public NExpresion {
public:
	std::string nombre;
	NIdentificador(const std::string& nombre) : nombre(nombre) { }
};

/***********************************************************/
/***********************************************************/
/*********************** CONDICIONES ***********************/
/***********************************************************/
/***********************************************************/

class NCondicion : public Nodo {
};

class NOperacionBooleana : public NCondicion {
public:
	int cod_op;
	NCondicion& cond1;
	NCondicion& cond2;
	NOperacionBooleana(int cod_op, NCondicion& cond1, NCondicion& cond2) :
		cond1(cond1), cond2(cond2), cod_op(cod_op) { }
	NOperacionBooleana(NCondicion& cond1) :
		cond1(cond1), cond2(cond1), cod_op(NOT) { } 	// cond2 no se mira
};

class NComparacion : public NCondicion { 
public:
	int cod_comp;
	NExpresion& expr1;
	NExpresion& expr2;
	NComparacion(int cod_comp, NExpresion& expr1, NExpresion& expr2) : 
		cod_comp(cod_comp), expr1(expr1), expr2(expr2) { }
};
