#ifndef _smsSender_mock_hpp
#define _smsSender_mock_hpp

#include "gmock/gmock.h"

#include "mailSender.hpp"
#include "mailDevice_mock.hpp"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class mailSender_mock : public mailSender
{
public:
        mailSender_mock() : mailSender(nullptr){};

	MOCK_METHOD4(send, int(std::string from, std::string to, std::string topic, std::string content));

};

#endif //_mailSender_mock_hpp
