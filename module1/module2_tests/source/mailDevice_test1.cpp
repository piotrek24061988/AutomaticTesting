#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mailDevice.hpp"
#include <cstdio>

class mailDevice_test1 : public testing::Test
{
protected:
	virtual void SetUp()
	{
		device = NULL;
		device = new mailDevice();
	}
	virtual void TearDown()
	{
		if(device)
		{
			delete device;
			device = NULL;
		}
	}

	mailDevice * device;
};

#ifndef IntegrationTests
TEST_F(mailDevice_test1, openDriverSuccess)
{
	EXPECT_FALSE(device->openDriver() == NULL);
}

TEST_F(mailDevice_test1, closeDriverSuccess)
{
	FILE* id = device->openDriver();
	EXPECT_EQ(0, device->closeDriver(id));
}

#endif
