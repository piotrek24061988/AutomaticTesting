#include "connectionChecker.hpp"

#include <cstdio>
#include <iostream>

connectionChecker::connectionChecker()
{
}

connectionChecker::~connectionChecker()
{
}

bool connectionChecker::isInternetAvailable()
{
	std::cout << "bool connectionChecker::isInternetAvailable()" << std::endl;

	FILE * output;

	if(!(output = popen("/sbin/route -n | grep -c '^0\\.0\\.0\\.0'","r")))
	{
		return false;
	}

	unsigned int i;
	fscanf(output,"%u",&i);
	pclose(output);

	if(i == 0)
	{
		std::cout << "There is no internet connection" << std::endl;
		return false;
	}
	else if(i == 1)
	{
		std::cout << "There is internet connection" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}
