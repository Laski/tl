#include "node.h"

double NFuncion::evaluar(DiccVariables& vars){
	//Ejecutar sentencias
	ListaSentencias sent = bloque.sentencias;
	ListaSentencias::iterator it = bloque.sentencias.begin();
	int estado = 0;
	while(estado != 1){
		estado = (*it)->evaluar(vars);
		it++;
	}
	return vars["__res__"];
}

int NBloque::evaluar(DiccVariables& vars){
	//Ejecutar sentencias
	ListaSentencias::iterator it = sentencias.begin();
	int estado = 0;
	while(estado != 1 && it != sentencias.end()){
		estado = (*it)->evaluar(vars);
		it++;
	}
	return estado;
}

int NReturn::evaluar(DiccVariables& vars){
	vars["__res__"] = expresion.evaluar(vars);
	return 1;
}

int NIfThen::evaluar(DiccVariables& vars){
	if(cond.evaluar(vars))
		return bloque.evaluar(vars);
}

int NIfThenElse::evaluar(DiccVariables& vars){
	if(cond.evaluar(vars))
		return bloqueT.evaluar(vars);
	else
		return bloqueF.evaluar(vars);
}

int NWhile::evaluar(DiccVariables& vars){
	int estado = 0;
	while(estado != 1 && cond.evaluar(vars)){
		estado = bloque.evaluar(vars);
	}
	return estado;		
}

int NAsignacion::evaluar(DiccVariables& vars){
	vars[lhs.nombre] = rhs.evaluar(vars);
	return 0;
}