#ifndef _connectionChecker_hpp
#define _connectionChecker_hpp

class connectionChecker
{
public:
	connectionChecker();
	~connectionChecker();

	//Return true if there is internet connection available.
	//Otherwise return false.
	bool isInternetAvailable();
};

#endif //_connectionChecker_hpp
