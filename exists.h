/*
NAME: Jordan Garcia
DATE: 09/19/2018

DESCRIPTION:

this is a generic function used in a function in the main file.
I made it generic originally because i used it for different types, 
however after rewriting my program, I eventually only needed in one place. 
I just left it as a generic function for convenience though.

*/

#include <vector>
using namespace std;

template <class T> 
bool exists(const T & element, const vector<T> & elements)
{
	for(int i = 0; i < elements.size(); i++)
	{
		if(elements[i] == element)
			return true;
	}
	return false;
}
