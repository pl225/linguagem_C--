#include <stack>
#include <iostream>	
#include <sstream>


using std::stack;    

int main(int argc, char const *argv[])
{
	stack< int > numbers;
	numbers.push( 1 );
	numbers.push( 2 );

	stack<int> m = numbers;

	while ( not m.empty( ) )
	{
	    int number = m.top( );
	    std::cout << number << std::endl;
	    m.pop( );
	}
	std::cout << numbers.size() << std::endl;
	return 0;
}