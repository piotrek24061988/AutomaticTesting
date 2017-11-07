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
	EXPECT_TRUE(device->openDriver() != NULL);
}
#endif

#ifndef IntegrationTests
TEST_F(mailDevice_test1, closeDriverSuccess)
{
        FILE * id = device->openDriver();

	EXPECT_NE(-1 , device->closeDriver(id));
}
#endif

#ifndef IntegrationTests
TEST_F(mailDevice_test1, closeDriverFail)
{
	EXPECT_EQ(-1 , device->closeDriver(NULL));
}
#endif

#ifndef IntegrationTests
TEST_F(mailDevice_test1, sendMailSuccess)
{
        FILE * id = device->openDriver();

	EXPECT_NE(-1 , device->sendMail(id, "piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "topic1", "content1"));

        device->closeDriver(id);
}
#endif

#ifndef IntegrationTests
TEST_F(mailDevice_test1, sendMailFail)
{
	EXPECT_EQ(-1 , device->sendMail(NULL, "piotrek24061988@gmail.com", "piotrek24061988@gmail.com", "topic2", "content2"));
}
#endif
