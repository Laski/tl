#include <iostream>
#include <map>
#include "node.h"
#include <typeinfo>
#include <string>

using namespace std;

extern int yyparse(); //metodos de parseo generado por bison, crea el AST
extern NPrograma* programBlock; //variable global definida en parser.y, instanciada con el root del AST en la primer produccion.

std::vector<double> ejecutar_funcion(double desde, double hasta, double paso, DiccFunciones& funcs, string f_id){
	DiccVariables vars;
	ListaExpresiones args;
	NFuncion *f = funcs[f_id];
	std::vector<double> resultados;
	for(double i = desde; i <= hasta; i += paso){
		args.clear();	// la lista de parametros tiene que arrancar vacia
		args.push_back(new NDouble(i));
		double res_i = f->evaluar(args, vars, funcs);
		resultados.push_back(res_i);
		cerr << f_id << "(" << i << ") = " << res_i << endl;
	}
	return resultados;
}

int main(int argc, char **argv)
{   
	yyparse();
	cerr << "parse over" << endl <<endl;

	double desde = programBlock->ploteo.desde.value;
	double paso = programBlock->ploteo.paso.value;
	double hasta = programBlock->ploteo.hasta.value;

	DiccFunciones& funcs = programBlock->funciones;
	string id_f1 = programBlock->ploteo.func1.id.nombre;
	string id_f2 = programBlock->ploteo.func2.id.nombre;
	std::vector<double> resultados_1 = ejecutar_funcion(desde, hasta, paso, funcs, id_f1);
	std::vector<double> resultados_2 = ejecutar_funcion(desde, hasta, paso, funcs, id_f2);

	cout.precision(1);

	for (int i = 0; i < resultados_1.size(); i++){
		cout << fixed << resultados_1[i] << " " << resultados_2[i] << endl;
	}

	return 0;
}