int f(){
	return 42;
}

int main(){
	int x = 0;
	-f(); /* sin ';', esto se interpretaria como int x = 0-f() */
	return 0;
}
