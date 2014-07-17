#include <iostream>
#include <map>
#include "node.h"
#include <typeinfo>
#include <string>

extern int yyparse(); //metodos de parseo generado por bison, crea el AST
extern NPrograma* programBlock; //variable global definida en parser.y, instanciada con el root del AST en la primer produccion.

std::vector<double> ejecutar_funcion(double desde, double hasta, double paso, DiccFunciones& funcs, std::string f_id){
	DiccVariables vars;
	ListaExpresiones args;
	NFuncion *f = funcs[f_id];
	std::vector<double> resultados;
	for(double i = desde; i <= hasta; i += paso){
		args.clear();	// la lista de parametros tiene que arrancar vacia
		args.push_back(new NDouble(i));
		//cout << "ARG: " << args[0]->evaluar(vars, funcs) << endl;
		double res_i = f->evaluar(args, vars, funcs);
		resultados.push_back(res_i);
	}
	return resultados;
}

void imprimir_resultados(std::vector<double> resultados, std::string f_id){
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
	std::string id_f1 = programBlock->ploteo.func1.id.nombre;
	std::string id_f2 = programBlock->ploteo.func2.id.nombre;

	DiccVariables v;
	double desde = programBlock->ploteo.desde.evaluar(v, funcs);
	double paso = programBlock->ploteo.paso.evaluar(v, funcs);
	double hasta = programBlock->ploteo.hasta.evaluar(v, funcs);

	std::vector<double> resultados_1 = ejecutar_funcion(desde, hasta, paso, funcs, id_f1);
	std::vector<double> resultados_2 = ejecutar_funcion(desde, hasta, paso, funcs, id_f2);

	/* debug */
	imprimir_resultados(resultados_1, id_f1);
	imprimir_resultados(resultados_2, id_f2);

	/* salida */
	std::cout.precision(1);
	for (int i = 0; i < resultados_1.size(); i++){
		std::cout << std::fixed << resultados_1[i] << " " << resultados_2[i] << std::endl;
	}

	return 0;
}

