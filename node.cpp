#include <cmath>
#include "node.h"

double NFuncion::evaluar(ListaDoubles& argumentos, DiccFunciones& funcs){
	DEBUG_OUT("evaluar funcion: " << id.nombre);
	//chequeo cantidad de argumentos
	if (parametros.size() != argumentos.size()){
		ERROR_OUT("de ejecución: La cantidad de argumentos de '" << id.nombre << "' no coincide con su declaración.");
	}

	//instancio argumentos como variables locales
	DiccVariables vars;
	for(int i = 0; i < argumentos.size(); i++){
		vars[parametros[i]->nombre] = argumentos[i];
		DEBUG_OUT("	" << parametros[i]->nombre << " <- " << argumentos[i]);
	}

	//evaluar sentencias
	bloque.evaluar(vars, funcs);
	DEBUG_OUT("	resultado funcion " << id.nombre << ": " << vars["__res__"]);
	return vars["__res__"];
}

void NFuncion::verificar(NBloque& bloque){
	// toda función debe tener un return en su bloque de sentencias
	if (bloque.tiene_return()){
		return;
	} else {
		ERROR_OUT("de ejecución: Toda función debe devolver algo.");
	}
}

int NBloque::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	//DEBUG_OUT("evaluar bloque: ");

	//ejecutar sentencias
	ListaSentencias::iterator it = sentencias.begin();
	int estado = 0;
	while(estado != 1 && it != sentencias.end()){
		estado = (*it)->evaluar(vars, funcs);
		it++;
	}
	return estado;
}

bool NBloque::tiene_return(){
	for(ListaSentencias::iterator it = sentencias.begin(); it != sentencias.end(); it++){
		if ((*it)->tiene_return()){
			return true;
		}
	}
	return false;
}

int NReturn::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar return");
	vars["__res__"] = expresion.evaluar(vars, funcs);
	//DEBUG_OUT("	resultado return: " << vars["__res__"]);
	return 1;
}

int NIfThen::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar ifthen");
	if(cond.evaluar(vars, funcs)){
		DEBUG_OUT("	dentro del if");
		return bloque.evaluar(vars, funcs);
	}
}

int NIfThenElse::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar ifthenelse");
	if(cond.evaluar(vars, funcs)){
		DEBUG_OUT("	dentro del if");
		return bloqueT.evaluar(vars, funcs);
	}
	else{
		DEBUG_OUT("	dentro del else");
		return bloqueF.evaluar(vars, funcs);
	}
}

int NWhile::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar while");
	int estado = 0;
	while(estado != 1 && cond.evaluar(vars, funcs)){
		DEBUG_OUT("	dentro del while");
		estado = bloque.evaluar(vars, funcs);
	}
	return estado;
}

int NAsignacion::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar asignacion");
	double valor = rhs.evaluar(vars, funcs); 
	vars[lhs.nombre] = valor;
	DEBUG_OUT("	resultado asignacion: " << lhs.nombre << " = " << valor);
	return 0;
}

double NLlamadaFuncion::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar llamada funcion: " << id.nombre);
	//evaluo los argumentos
	ListaDoubles args_evaluados;
	for (int i = 0; i < argumentos.size(); i++){
		args_evaluados.push_back(argumentos[i]->evaluar(vars, funcs));
	}
	//verifico que exista una función con este nombre
	if (funcs.find(id.nombre) == funcs.end()){
		ERROR_OUT("de ejecución: La función de nombre '" << id.nombre << "' no está definida.");
	}
	double res = funcs[id.nombre]->evaluar(args_evaluados, funcs);
	//DEBUG_OUT("	resultado llamada funcion: " << res);
	return res;
}

double NDouble::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	//DEBUG_OUT("evaluar double: " << value);
	return value;
};

double NOperacionAritmetica::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar operacion aritmetica: " << cod_op);
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
		case POT:
			res = pow(expr1.evaluar(vars, funcs), expr2.evaluar(vars, funcs));
			break;
	}
	DEBUG_OUT("	resultado operacion artimetica: " << res);
	return res;
}

double NIdentificador::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar identificador: " << nombre << " -> " << vars[nombre] );
	return vars[nombre];
}

bool NOperacionBooleana::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar operacion booleana: " << cod_op);
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
	DEBUG_OUT("	resultado operacion booleana: " << res);
}

bool NComparacion::evaluar(DiccVariables& vars, DiccFunciones& funcs){
	DEBUG_OUT("evaluar comparacion: " << cod_op);
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
	DEBUG_OUT("	resultado comparacion: " << res);
	return res;
}