#include <iostream>
using namespace std;

int test(){
	/* devuelve siempre 0... */
	if (0)
		if (1)
			return 1;
		else	/* ...porque este else se asocia con el último if */
			return 2;
	return 0;
}

int main(){
	cout << test() << endl;
	return 0;
}
