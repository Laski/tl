#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

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

//COD OP SENTENCIAS
#define IFTHEN  		1
#define IFTHENELSE  	2
#define WHILE  			3
#define ASIGNACION  	4
#define RETURN		  	5

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
	const NIdentificador& id;
	ListaVariables argumentos;
	NBloque& bloque;
	NFuncion(const NIdentificador& id, 
			const ListaVariables& argumentos, NBloque& bloque) :
		id(id), argumentos(argumentos), bloque(bloque) { }
	~NFuncion(){};
	double evaluar(DiccVariables& vars);
};

class NBloque : public Nodo {
public:
	ListaSentencias& sentencias;
	NBloque() : sentencias(*(new ListaSentencias())) {};
	NBloque(ListaSentencias& sentencias) :
		sentencias(sentencias) { };
	~NBloque(){};
	int evaluar(DiccVariables& vars);
};

class NPloteo : public Nodo {
public:
	NLlamadaFuncion& func1;
	NLlamadaFuncion& func2;
	NNumero& desde;
	NNumero& paso;
	NNumero& hasta;
	NPloteo(NLlamadaFuncion& func1, NLlamadaFuncion& func2, NNumero& desde, NNumero& paso, NNumero& hasta) : 
		func1(func1), func2(func2), desde(desde), paso(paso), hasta(hasta) { };
	~NPloteo(){};
};

/***********************************************************/
/***********************************************************/
/*********************** SENTENCIAS ************************/
/***********************************************************/
/***********************************************************/


class NSentencia : public Nodo {
public:
	int cod_sent;
	virtual int evaluar(DiccVariables& vars) = 0; //devuelve 1 si ejecuto return
	NSentencia(){};
	~NSentencia(){};
};

class NReturn : public NSentencia {
public:
	NExpresion& expresion;
	NReturn(NExpresion& expresion) : 
		expresion(expresion) { cod_sent = RETURN; };
	~NReturn(){};

	int evaluar(DiccVariables& vars);

};

class NIfThen : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloque;
	NIfThen(NCondicion& cond, NBloque& bloque) : 
		cond(cond), bloque(bloque) { cod_sent = IFTHEN; }
	~NIfThen(){};

	int evaluar(DiccVariables& vars);
};

class NIfThenElse : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloqueT;
	NBloque& bloqueF;
	NIfThenElse(NCondicion& cond, NBloque& bloqueT, NBloque& bloqueF) : 
		cond(cond), bloqueT(bloqueT), bloqueF(bloqueF) { cod_sent = IFTHENELSE; }
	~NIfThenElse(){};
	int evaluar(DiccVariables& vars);
};

class NWhile : public NSentencia {
public:
	NCondicion& cond;
	NBloque& bloque;
	NWhile(NCondicion& cond, NBloque& bloque) : 
		cond(cond), bloque(bloque) { cod_sent = WHILE; }
	~NWhile(){};
	int evaluar(DiccVariables& vars);
};

class NAsignacion : public NSentencia {
public:
	NIdentificador& lhs;
	NExpresion& rhs;
	NAsignacion(NIdentificador& lhs, NExpresion& rhs) : 
		lhs(lhs), rhs(rhs) { cod_sent = ASIGNACION; }
	~NAsignacion(){};
	int evaluar(DiccVariables& vars);
};

/***********************************************************/
/***********************************************************/
/*********************** EXPRESIONES ***********************/
/***********************************************************/
/***********************************************************/

class NExpresion : public Nodo {
public:
	virtual double evaluar(DiccVariables& vars) = 0;
	NExpresion(){};
	~NExpresion(){};
};

class NNumero : public NExpresion{
public:
	double value;
	double evaluar(DiccVariables& vars){
		return value;
	};
	~NNumero(){};
};

class NLlamadaFuncion : public NExpresion {
public:
	const NIdentificador& id;
	ListaExpresiones argumentos;
	NLlamadaFuncion(const NIdentificador& id, ListaExpresiones& argumentos) :
		id(id), argumentos(argumentos) { }
	~NLlamadaFuncion(){};
	double evaluar(DiccVariables& vars);
	// evaluar(DiccVariables& vars){
	// 	//Inicializo mis variables locales
	// 	DiccVariables vars;
	// 	for(int i = 0; i < params.size(); i++)
	// 		vars[funcion.argumentos[i]->nombre] = params[i];
	// }
};

class NOperacionAritmetica : public NExpresion {
public:
	int cod_op;
	NExpresion& expr1;
	NExpresion& expr2;
	NOperacionAritmetica(int cod_op, NExpresion& expr1, NExpresion& expr2) :
		expr1(expr1), expr2(expr2), cod_op(cod_op) { }
	~NOperacionAritmetica(){};
	double evaluar(DiccVariables& vars){
		double res;
		switch(cod_op){
			case MAS:
				res = expr1.evaluar(vars) + expr2.evaluar(vars);
				break;
			case MENOS:
				res = expr1.evaluar(vars) - expr2.evaluar(vars);
				break;
			case MUL:
				res = expr1.evaluar(vars) * expr2.evaluar(vars);
				break;
			case DIV:
				res = expr1.evaluar(vars) / expr2.evaluar(vars);
				break;
		}
		return res;
	}
};

class NEntero : public NNumero {
public:
	long long value;
	NEntero(long long val) : value(val) { }; //Este constructor no carga el valor en value
	NEntero(){};
	~NEntero(){};
};

class NDouble : public NNumero {
public:
	double value;
	NDouble(double val) : value(val) { } //Este constructor no carga el valor en value
	NDouble(){};
	~NDouble(){};
};

class NIdentificador : public NExpresion {
public:
	std::string nombre;
	NIdentificador(const std::string& nombre) : nombre(nombre) { }
	~NIdentificador(){};
	double evaluar(DiccVariables& vars){
		return vars[nombre]; //si la variable no está definida deberíamos poder atrapar el error en el parsing
	}
};

/***********************************************************/
/***********************************************************/
/*********************** CONDICIONES ***********************/
/***********************************************************/
/***********************************************************/

class NCondicion : public Nodo {
public:
	virtual bool evaluar(DiccVariables& vars) = 0;
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
	bool evaluar(DiccVariables& vars){
		bool res;
		switch(cod_op){
			case AND:
				res = cond1.evaluar(vars) && cond2.evaluar(vars);
				break;
			case OR:
				res = cond1.evaluar(vars) || cond2.evaluar(vars);
				break;
			case NOT:
				res = !cond1.evaluar(vars);
				break;
		}
		return res;
	}

};

class NComparacion : public NCondicion { 
public:
	int cod_op;
	NExpresion& expr1;
	NExpresion& expr2;

	NComparacion(int cod_comp, NExpresion& expr1, NExpresion& expr2) : 
		cod_op(cod_comp), expr1(expr1), expr2(expr2) { };
	~NComparacion(){};
	bool evaluar(DiccVariables& vars){
		bool res;
		switch(cod_op){
			case MAYOR:
				res = expr1.evaluar(vars) > expr2.evaluar(vars);
				break;
			case MAYORIGUAL:
				res = expr1.evaluar(vars) >= expr2.evaluar(vars);
				break;
			case MENOR:
				res = expr1.evaluar(vars) < expr2.evaluar(vars);
				break;
			case MENORIGUAL:
				res = expr1.evaluar(vars) <= expr2.evaluar(vars);
				break;
			case DISTINTO:
				res = expr1.evaluar(vars) != expr2.evaluar(vars);
				break;
			case IGUAL:
				res = expr1.evaluar(vars) == expr2.evaluar(vars);
				break;
		}
		return res;
	}
};







#endif // NODE_H_INCLUDED
