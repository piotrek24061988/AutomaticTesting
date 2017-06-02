
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "smsPlanner.hpp"

using ::testing::Return;

class smsPlannerTest : public testing::Test
{
protected:
	virtual void SetUp()
	{
		planner = NULL;
		planner = new smsPlanner();
	} 

	virtual void TearDown()
	{
		delete planner;
		planner = NULL;
	}

	smsPlanner *planner;
	std::time_t time1;
};

TEST_F(smsPlannerTest, addDeliveryNoError)
{
	std::localtime(&time1);
	EXPECT_LT(-1, planner->addDelivery(std::string("537240688"), std::string("Hello"), time1));
}

TEST_F(smsPlannerTest, addDeliveryRetDiffId)
{
	std::localtime(&time1);
	int id = planner->addDelivery(std::string("537240688"), std::string("Hello"), time1);
	std::localtime(&time1);
	EXPECT_NE(id, planner->addDelivery(std::string("537240688"), std::string("Hello"), time1));
}

TEST_F(smsPlannerTest, cancelDeliveries)
{
	int id = planner->addDelivery(std::string("537240688"), std::string("Hello"), time1);
	EXPECT_TRUE(planner->cancelDelivery(id));
	EXPECT_FALSE(planner->cancelDelivery(id));		
}
