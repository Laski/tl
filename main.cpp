#include <iostream>
#include <map>
#include "node.h"
#include <typeinfo>
#include <string>

using namespace std;

extern int yyparse(); //metodos de parseo generado por bison, crea el AST
extern NPrograma* programBlock; //variable global definida en parser.y, instanciada con el root del AST en la primer produccion.


//typedef vector<double> ListaParametros;
//typedef map<string, double> DiccVariables;

int main(int argc, char **argv)
{   
	yyparse();
	cout << "parse over" << endl<<endl<<endl;

	double desde = programBlock->ploteo.desde.value;
	double paso = programBlock->ploteo.paso.value;
	double hasta = programBlock->ploteo.hasta.value;

	DiccFunciones& funcs = programBlock->funciones;
	string id_f1 = programBlock->ploteo.func1.id.nombre;
	string id_f2 = programBlock->ploteo.func2.id.nombre;
	DiccVariables vars;
	ListaExpresiones args;
	double i;

	for(i = desde; i <= hasta; i += paso){
		args.push_back(new NDouble(i));
		double res_i = funcs[id_f1]->evaluar(args, vars, funcs);
		cout << id_f1 << "(" << i << ") = " << i << endl;
	}
	return 0;
}