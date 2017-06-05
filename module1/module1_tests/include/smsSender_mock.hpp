#ifndef _smsSender_mock_hpp
#define _smsSender_mock_hpp

#include "gmock/gmock.h"

#include "smsSender.hpp"

class smsSender_mock : public smsSender
{
public:
	MOCK_METHOD2(send, bool(std::string number, std::string message));
};

#endif //_smsSender_mock_hpp
