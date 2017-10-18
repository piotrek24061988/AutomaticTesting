#include "coutRedirect.hpp"

//Redirect cout to buffer stream by creating object.
coutRedirect::coutRedirect()
{
	old = std::cout.rdbuf(buffer.rdbuf());
}

//Get redirected cout as string.
std::string coutRedirect::getString() 
{
	return buffer.str(); 
}

//Reverse redirect by deleting this object.
coutRedirect::~coutRedirect( ) 
{
	std::cout.rdbuf(old); 
}
