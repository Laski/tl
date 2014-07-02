#include <iostream>
#include <map>
#include <typeinfo>
#include <string>

using namespace std;

extern int yyparse(); //metodos de parseo generado por bison, crea el AST
extern int yylex();
//extern NPrograma* programBlock; //variable global definida en parser.y, instanciada con el root del AST en la primer produccion.


//typedef vector<double> ListaParametros;
//typedef map<string, double> DiccVariables;

//double evaluarFuncion(NFuncion& funcion, ListaParametros& params);

int main(int argc, char **argv)
{
	/*cout << "yyparse() output: " << yyparse() << endl;
	cout << programBlock->funciones.size() << endl;
   
	double desde = programBlock->ploteo.desde.value;
	double paso = programBlock->ploteo.paso.value;
	double hasta = programBlock->ploteo.hasta.value;

	for(double i = desde; i <= hasta; i += paso)
		cout << "for: "<< i << endl;

	ListaParametros params(1);
	params[0] = 2;
	evaluarFuncion(*(programBlock->funciones[programBlock->ploteo.func1.id.nombre]), params);
	*/
	yyparse();
	/*yylex();*/
	return 0;
}