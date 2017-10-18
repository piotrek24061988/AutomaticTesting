#include <stdio.h>
#include <iostream>
#include <streambuf>
#include <stdlib.h>
#include <string>
#include <sstream>

class coutRedirect {

public:
    coutRedirect() {
        old = std::cout.rdbuf( buffer.rdbuf() ); // redirect cout to buffer stream
    }

    std::string getString() {
        return buffer.str(); // get string
    }

    ~coutRedirect( ) {
        std::cout.rdbuf( old ); // reverse redirect
    }

private:
    std::stringstream buffer;
    std::streambuf * old;
};
