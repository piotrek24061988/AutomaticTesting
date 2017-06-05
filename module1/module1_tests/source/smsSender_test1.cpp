#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smsSender.hpp"

class smsSender_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		sender = NULL;
		sender = new smsSender();
	}
	virtual void TearDown()
	{
		if(sender)
		{
			delete sender;
			sender = NULL;
		}
	}

	smsSender * sender;
};

TEST_F(smsSender_test1, sendValidatedSms)
{
	EXPECT_TRUE(sender->send(std::string("537240688"), std::string("Hello")));
}
