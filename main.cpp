#include <iostream>
#include <map>
#include <typeinfo>
#include "node.h"
#include <string>

using namespace std;

extern int yyparse(); //metodos de parseo generado por bison, crea el AST
extern int yylex();
extern NPrograma* programBlock; //variable global definida en parser.y, instanciada con el root del AST en la primer produccion.


typedef std::vector<double> ListaParametros;
double evaluarFuncion(NFuncion& funcion, ListaParametros& params);

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

double evaluarFuncion(NFuncion& funcion, ListaParametros& params){

	/*Error de llamada
	if(params.size() != funcion.argumentos.size()){
		cout << "ERROR: llamada a la funcion " << funcion.id.nombre << " con cantidad de parametros incorrecta." << endl;
		return 0;
	} Deberia chequearse antes de evaluarFuncion() en la clase NLlamadaFuncion */


	//Inicializo mi pila de variables locales
	map<string, double> stack;
	for(int i = 0; i < params.size(); i++)
		stack[funcion.argumentos[i]->nombre] = params[i];

	//Ejecutar sentencias
	ListaSentencias::iterator it = funcion.bloque.sentencias.begin();

	while(it != funcion.bloque.sentencias.end()){
		cout << typeid(*it).name() << endl; // para ver si diferencia el tipo de cada subclase -> NO LO DIFERENCIA :(
		it++;
	}
}	
