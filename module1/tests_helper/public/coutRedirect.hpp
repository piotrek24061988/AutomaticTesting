#ifndef _coutRedirect_hpp
#define _coutRedirect_hpp

#include <iostream>
#include <streambuf>
#include <string>
#include <sstream>

class coutRedirect
{
public:
    //Redirect cout to buffer stream by creating object.
    coutRedirect();
    //Get redirected cout as string.
    std::string getString();
    //Reverse redirect by deleting this object.
    ~coutRedirect();

private:
    std::stringstream buffer;
    std::streambuf * old;
};

#endif //_coutRedirect_hpp
