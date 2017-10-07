#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smsSender.hpp"

class smsDevice_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		device = NULL;
		device = new smsDevice();
	}
	virtual void TearDown()
	{
		if(device)
		{
			delete device;
			device = NULL;
		}
	}

	smsDevice * device;
};


TEST_F(smsDevice_test1, initialize)
{
	EXPECT_TRUE(device->init());
}

TEST_F(smsDevice_test1, deInitialize)
{
	EXPECT_TRUE(device->deInit());
}

TEST_F(smsDevice_test1, sendSmsWithoutInit)
{
	EXPECT_FALSE(device->send(std::string("537240688"), std::string("Hello")));
}

TEST_F(smsDevice_test1, sendSmsWithInit)
{
	EXPECT_TRUE(device->init());
	EXPECT_TRUE(device->send(std::string("537240688"), std::string("Hello")));
}

TEST_F(smsDevice_test1, sendSmsWithDeinit)
{
	EXPECT_TRUE(device->init());
	EXPECT_TRUE(device->deInit());
	EXPECT_FALSE(device->send(std::string("537240688"), std::string("Hello")));
}
