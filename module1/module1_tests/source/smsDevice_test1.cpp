#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smsSender.hpp"

#include <memory>

class smsDevice_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		if(!device)
		{
			device = std::make_unique<smsDevice>();
		}
	}
	virtual void TearDown()
	{
		//smart_ptr used so no need to clean anything
	}

	std::unique_ptr<smsDevice> device;
};

#ifndef IntegrationTests
TEST_F(smsDevice_test1, initialize)
{
	EXPECT_TRUE(device->init());
}
#endif

#ifndef IntegrationTests
TEST_F(smsDevice_test1, deInitialize)
{
	EXPECT_TRUE(device->deInit());
}
#endif

#ifndef IntegrationTests
TEST_F(smsDevice_test1, sendSmsWithoutInit)
{
	EXPECT_FALSE(device->send(std::string("537240688"), std::string("Hello")));
}
#endif

#ifndef IntegrationTests
TEST_F(smsDevice_test1, sendSmsWithInit)
{
	EXPECT_TRUE(device->init());
	EXPECT_TRUE(device->send(std::string("537240688"), std::string("Hello")));
}
#endif

#ifndef IntegrationTests
TEST_F(smsDevice_test1, sendSmsWithDeinit)
{
	EXPECT_TRUE(device->init());
	EXPECT_TRUE(device->deInit());
	EXPECT_FALSE(device->send(std::string("537240688"), std::string("Hello")));
}
#endif
