#include "mailDevice.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    int expectedArgNr = 5;

    std::cout << "Input arguments:" << std::endl;
    for (int i = 1; i < argc; i++)
    {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    std::cout << std::endl;

    if(argc == expectedArgNr)
    {
        mailDevice device;
        FILE * id = device.openDriver();
        device.sendMail(id, std::string(argv[1]), std::string(argv[2]), std::string(argv[3]), std::string(argv[4]));
        return device.closeDriver(id);
    }
    else
    {
        std::cout << "Please provide 4 input arguments for client application:" << std::endl;
        std::cout << "Argument nr 1: destination email address" << std::endl;
        std::cout << "Argument nr 2: source email adress" << std::endl;
        std::cout << "Argument nr 3: email topic" << std::endl;
        std::cout << "Argument nr 4: email content" << std::endl;
        return -1;
    }
}
