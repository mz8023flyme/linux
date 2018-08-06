#include <stdio.h> 


void inverse(char *p)
{
	if( *p == '\0' )
	return;
	inverse( p+1 );
	printf( "%c", *p );
}
int main(int argc, char *argv[])
{
	inverse("abc");
	return 0;
}

