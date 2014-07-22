#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <cstdlib>
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
typedef std::vector<double> ListaDoubles;
typedef std::map<std::string, double> DiccVariables;

//COD OP CONDICIONES
#define NOT 	0
#define AND 	1
#define OR  	2
#define IGUAL  	3
#define MAYOR  	4
#define MENOR  	5
#define MENORIGUAL  	6
#define MAYORIGUAL  	7
#define DISTINTO  		8

//COD OP ARITMETICAS
#define MAS  	9
#define MENOS  	10
#define MUL  	11
#define DIV  	12
#define POT 	13

//COD OP SENTENCIAS
#define IFTHEN  		1
#define IFTHENELSE  	2
#define WHILE  			3
#define ASIGNACION  	4
#define RETURN		  	5

#define DEBUG 0
#define DEBUG_OUT(s) (DEBUG == 1) ? std::cerr << s << std::endl : std::cerr << ""
#define ERROR_OUT(s) std::cerr << "ERROR " << s << std::endl; std::exit(1)

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
	~NPrograma(){}
};

class NFuncion : public Nodo {
public:
	NIdentificador& id;
	ListaVariables& parametros;
	NBloque& bloque;
	void verificar(NBloque& bloque);
	NFuncion(NIdentificador& id, 
			ListaVariables& parametros, NBloque& bloque) :
		id(id), parametros(parametros), bloque(bloque) {verificar(bloque);}
	~NFuncion(){};
	double evaluar(ListaDoubles& argumentos, DiccFunciones& funcs);
};

class NBloque : public Nodo {
public:
	ListaSentencias& sentencias;
	NBloque(ListaSentencias& sentencias) :
		sentencias(sentencias) { };
	~NBloque(){};
	int evaluar(DiccVariables& vars, DiccFunciones& funcs);
	bool tiene_return();
};

class NPloteo : public Nodo {
public:
	NLlamadaFuncion& func1;
	NLlamadaFuncion& func2;
	NIdentificador& var;
	NExpresion& desde;
	NExpresion& paso;
	NExpresion& hasta;
	NPloteo(NLlamadaFuncion& func1, NLlamadaFuncion& func2, NIdentificador& var, NExpresion& desde, NExpresion& paso, NExpresion& hasta) : 
		func1(func1), func2(func2), var(var), desde(desde), paso(paso), hasta(hasta) {  }
	~NPloteo(){};
};

/***********************************************************/
/***********************************************************/
/*********************** SENTENCIAS ************************/
/***********************************************************/
/***********************************************************/


class NSentencia : public Nodo {
public:
	NSentencia(){};
	~NSentencia(){};
	virtual int evaluar(DiccVariables& vars, DiccFunciones& funcs) = 0; //devuelve 1 si ejecuto return
	virtual bool tiene_return(){return false;}
};

class NReturn : public NSentencia {
public:
	NExpresion& expresion;
	NReturn(NExpresion& expresion) : 
		expresion(expresion) { };
	~NReturn(){};

	int evaluar(DiccVariables& vars, DiccFunciones& funcs);
	bool tiene_return(){return true;}
};

class NIfThen : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloque;
	NIfThen(NCondicion& cond, NBloque& bloque) : 
		cond(cond), bloque(bloque) { }
	~NIfThen(){};

	int evaluar(DiccVariables& vars, DiccFunciones& funcs);
	bool tiene_return(){return bloque.tiene_return();}
};

class NIfThenElse : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloqueT;
	NBloque& bloqueF;
	NIfThenElse(NCondicion& cond, NBloque& bloqueT, NBloque& bloqueF) : 
		cond(cond), bloqueT(bloqueT), bloqueF(bloqueF) { }
	~NIfThenElse(){};
	int evaluar(DiccVariables& vars, DiccFunciones& funcs);
	bool tiene_return(){return bloqueT.tiene_return() and bloqueF.tiene_return();}
};

class NWhile : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloque;
	NWhile(NCondicion& cond, NBloque& bloque) : 
		cond(cond), bloque(bloque) { }
	~NWhile(){};
	int evaluar(DiccVariables& vars, DiccFunciones& funcs);
	bool tiene_return(){return bloque.tiene_return();}
};

class NAsignacion : public NSentencia {
public:
	NIdentificador& lhs;
	NExpresion& rhs;
	NAsignacion(NIdentificador& lhs, NExpresion& rhs) : 
		lhs(lhs), rhs(rhs) { }
	~NAsignacion(){};
	int evaluar(DiccVariables& vars, DiccFunciones& funcs);
};

/***********************************************************/
/***********************************************************/
/*********************** EXPRESIONES ***********************/
/***********************************************************/
/***********************************************************/

class NExpresion : public Nodo {
public:
	virtual double evaluar(DiccVariables& vars, DiccFunciones& funcs) = 0;
	NExpresion(){};
	~NExpresion(){};
};

class NNumero : public NExpresion{
public:
	double value;
	//double evaluar(DiccVariables& vars, DiccFunciones& funcs);
	~NNumero(){};
};

class NLlamadaFuncion : public NExpresion {
public:
	NIdentificador& id;
	ListaExpresiones argumentos;
	NLlamadaFuncion(NIdentificador& id, ListaExpresiones& argumentos) :
		id(id), argumentos(argumentos) { }
	~NLlamadaFuncion(){};
	double evaluar(DiccVariables& vars, DiccFunciones& funcs);
};

class NOperacionAritmetica : public NExpresion {
public:
	int cod_op;
	NExpresion& expr1;
	NExpresion& expr2;
	NOperacionAritmetica(int cod_op, NExpresion& expr1, NExpresion& expr2) :
		expr1(expr1), expr2(expr2), cod_op(cod_op) { }
	~NOperacionAritmetica(){};
	double evaluar(DiccVariables& vars, DiccFunciones& funcs);
};

class NDouble : public NNumero {
public:
	double value;
	NDouble(double val) { value = val; };
	double evaluar(DiccVariables& vars, DiccFunciones& funcs);
	NDouble(){};
	~NDouble(){};
};

class NIdentificador : public NExpresion {
public:
	std::string nombre;
	NIdentificador(std::string& nombre) : nombre(nombre) { }
	~NIdentificador(){};
	double evaluar(DiccVariables& vars, DiccFunciones& funcs);
};

/***********************************************************/
/***********************************************************/
/*********************** CONDICIONES ***********************/
/***********************************************************/
/***********************************************************/

class NCondicion : public Nodo {
public:
	virtual bool evaluar(DiccVariables& vars, DiccFunciones& funcs) = 0;
	NCondicion(){};
	~NCondicion(){};
};

class NOperacionBooleana : public NCondicion {
public:
	int cod_op;
	NCondicion& cond1;
	NCondicion& cond2;
	NOperacionBooleana(int cod_op, NCondicion& cond1, NCondicion& cond2) :
		cond1(cond1), cond2(cond2), cod_op(cod_op) { };
	NOperacionBooleana(NCondicion& cond1) :
		cond1(cond1), cond2(cond1), cod_op(NOT) { }; 	// cond2 no se mira
	~NOperacionBooleana(){};
	bool evaluar(DiccVariables& vars, DiccFunciones& funcs);

};

class NComparacion : public NCondicion { 
public:
	int cod_op;
	NExpresion& expr1;
	NExpresion& expr2;

	NComparacion(int cod_comp, NExpresion& expr1, NExpresion& expr2) : 
		cod_op(cod_comp), expr1(expr1), expr2(expr2) { };
	~NComparacion(){};
	bool evaluar(DiccVariables& vars, DiccFunciones& funcs);
};







#endif // NODE_H_INCLUDED
