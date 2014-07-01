#include <iostream>
#include "node.h"

using namespace std;

extern int yyparse();
extern NPrograma* programBlock;

//void createCoreFunctions(CodeGenContext& context);

int main(int argc, char **argv)
{
	yyparse();
	std::cout << "parsee!!" << std::endl;
	std::cout << programBlock << endl;
	/*InitializeNativeTarget();
	CodeGenContext context;
	createCoreFunctions(context);
	context.generateCode(*programBlock);
	context.runCode();*/
	return 0;
}
