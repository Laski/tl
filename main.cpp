#include <iostream>
#include <map>
#include "node.h"
#include <typeinfo>
#include <string>

extern int yyparse(); //metodos de parseo generado por bison, crea el AST
extern NPrograma* programBlock; //variable global definida en parser.y, instanciada con el root del AST en la primer produccion.

ListaDoubles ejecutar_funcion(NIdentificador& var, double desde, double hasta, double paso, DiccFunciones& funcs, NLlamadaFuncion& f){
	DiccVariables vars;
	ListaDoubles resultados;
	for(double i = desde; i <= hasta; i += paso){
		vars.clear();	// la lista de variables tiene que arrancar vacia
		vars[var.nombre] = i;
		resultados.push_back(f.evaluar(vars, funcs));
		DEBUG_OUT("");
		DEBUG_OUT("");
	}
	return resultados;
}

void imprimir_resultados(ListaDoubles resultados, std::string f_id){
	for (int i = 0; i < resultados.size(); i++){
		double res_i = resultados[i];
		std::cerr.precision(1);
		std::cerr << std::fixed << f_id << "(" << i << ") = " << res_i << std::endl;
	}
}

int main(int argc, char **argv)
{   
	yyparse();
	DEBUG_OUT("parse over" << std::endl);

	DiccFunciones& funcs = programBlock->funciones;

	NLlamadaFuncion& f1 = programBlock->ploteo.func1;
	NLlamadaFuncion& f2 = programBlock->ploteo.func2;
	NIdentificador& var = programBlock->ploteo.var;

	DiccVariables v;
	double desde = programBlock->ploteo.desde.evaluar(v, funcs);
	double paso = programBlock->ploteo.paso.evaluar(v, funcs);
	double hasta = programBlock->ploteo.hasta.evaluar(v, funcs);

	ListaDoubles resultados_1 = ejecutar_funcion(var, desde, hasta, paso, funcs, f1);
	ListaDoubles resultados_2 = ejecutar_funcion(var, desde, hasta, paso, funcs, f2);

	/* debug */
	imprimir_resultados(resultados_1, f1.id.nombre);
	imprimir_resultados(resultados_2, f2.id.nombre);

	/* salida */
	std::cout.precision(1);
	for (int i = 0; i < resultados_1.size(); i++){
		std::cout << std::fixed << resultados_1[i] << " " << resultados_2[i] << std::endl;
	}

	return 0;
}

