#include "node.h"

#define DEBUG 0
#define DEBUG_OUT(s) (DEBUG == 1) ? std::cerr << s << std::endl : std::cerr << ""

double NFuncion::evaluar(ListaExpresiones& argumentos, DiccVariables& vars, DiccFunciones& funcs){


	DEBUG_OUT("evaluar funcion: " << id.nombre);
	//Ejecutar sentencias
	
	std::vector<double> args;
	//evaluar argumentos
	for(int i = 0; i < argumentos.size(); i++)
		args.push_back(argumentos[i]->evaluar(vars, funcs));

	for(int i = 0; i < argumentos.size(); i++)
		vars[parametros[i]->nombre] = args[i];

	ListaSentencias sent = bloque.sentencias;
	ListaSentencias::iterator it = bloque.sentencias.begin();
	int estado = 0;
	while(estado != 1){
		estado = (*it)->evaluar(vars, funcs);
		it++;
	}
	return vars["__res__"];
}

int NBloque::evaluar(DiccVariables& vars, DiccFunciones& funcs){

	DEBUG_OUT("evaluar bloque: ");

	//Ejecutar sentencias
	ListaSentencias::iterator it = sentencias.begin();
	int estado = 0;
	while(estado != 1 && it != sentencias.end()){
		estado = (*it)->evaluar(vars, funcs);
		it++;
	}
	return estado;
}

int NReturn::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar return");

	vars["__res__"] = expresion.evaluar(vars, funcs);
	return 1;
}

int NIfThen::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar ifthen");

	if(cond.evaluar(vars, funcs))
		return bloque.evaluar(vars, funcs);
}

int NIfThenElse::evaluar(DiccVariables& vars, DiccFunciones& funcs){

	DEBUG_OUT("evaluar ifthenelse");

	if(cond.evaluar(vars, funcs))
		return bloqueT.evaluar(vars, funcs);
	else
		return bloqueF.evaluar(vars, funcs);
}

int NWhile::evaluar(DiccVariables& vars, DiccFunciones& funcs){

	DEBUG_OUT("evaluar while");

	int estado = 0;
	while(estado != 1 && cond.evaluar(vars, funcs)){
		estado = bloque.evaluar(vars, funcs);
	}
	return estado;		
}

int NAsignacion::evaluar(DiccVariables& vars, DiccFunciones& funcs){

	DEBUG_OUT("evaluar asignacion");

	vars[lhs.nombre] = rhs.evaluar(vars, funcs);
	return 0;
}

double NLlamadaFuncion::evaluar(DiccVariables& vars, DiccFunciones& funcs){

	DEBUG_OUT("evaluar llamada funcion: " << id.nombre);

	//Inicializo mis variables locales
	return funcs[id.nombre]->evaluar(argumentos, vars, funcs);
}

double NDouble::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar double: " << value);

	return value;
};

double NEntero::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar entero: " << value);

	return value;
};


double NOperacionAritmetica::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar operacion aritmentica: " << cod_op);

	double res;
	switch(cod_op){
		case MAS:
			res = expr1.evaluar(vars, funcs) + expr2.evaluar(vars, funcs);
			break;
		case MENOS:
			res = expr1.evaluar(vars, funcs) - expr2.evaluar(vars, funcs);
			break;
		case MUL:
			res = expr1.evaluar(vars, funcs) * expr2.evaluar(vars, funcs);
			break;
		case DIV:
			res = expr1.evaluar(vars, funcs) / expr2.evaluar(vars, funcs);
			break;
	}
	return res;
}

double NIdentificador::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar identificador: " << nombre << "->" << vars[nombre] );

	return vars[nombre]; //si la variable no está definida deberíamos poder atrapar el error en el parsing
}

bool NOperacionBooleana::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar operacion booleana: " << cod_op );

	bool res;
	switch(cod_op){
		case AND:
			res = cond1.evaluar(vars, funcs) && cond2.evaluar(vars, funcs);
			break;
		case OR:
			res = cond1.evaluar(vars, funcs) || cond2.evaluar(vars, funcs);
			break;
		case NOT:
			res = !cond1.evaluar(vars, funcs);
			break;
	}
	return res;
}

bool NComparacion::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar comparacion: " << cod_op );

	bool res;
	switch(cod_op){
		case MAYOR:
			res = expr1.evaluar(vars, funcs) > expr2.evaluar(vars, funcs);
			break;
		case MAYORIGUAL:
			res = expr1.evaluar(vars, funcs) >= expr2.evaluar(vars, funcs);
			break;
		case MENOR:
			res = expr1.evaluar(vars, funcs) < expr2.evaluar(vars, funcs);
			break;
		case MENORIGUAL:
			res = expr1.evaluar(vars, funcs) <= expr2.evaluar(vars, funcs);
			break;
		case DISTINTO:
			res = expr1.evaluar(vars, funcs) != expr2.evaluar(vars, funcs);
			break;
		case IGUAL:
			res = expr1.evaluar(vars, funcs) == expr2.evaluar(vars, funcs);
			break;
	}
	return res;
}