/** \file smsSender.hpp
 * Contains smsSender class declaration.
 * Class smsSender is used by smsPlanner class as dependency.
 * Class smsSender uses smsDevice class as dependency. 
 */
#ifndef _smsSender_hpp
#define _smsSender_hpp

#include <string>
#include <memory>

#include "smsDevice.hpp"

/**
 *  A smsSender class.
 *  Provides functionality to send sms message to dedicated number by dedicated smsDevice. 
 */
class smsSender
{
public:
	/**
	 * Constructor taking over smsDevice which will be used to send sms messages.
	 * @param sd as std::shared_ptr<smsDevice> which is used to send sms messages.
	 * @see send()
	 */
	smsSender(std::shared_ptr<smsDevice> sd);

	/**
	 * Standard destructor.
	 * Just print some debug data.
	 */
	~smsSender();

	/**
	 * Send sms with the message to provided number.
	 * @param number as std::string which is requested number to receive message.
	 * @param message as std::string which is requested message to be send.
	 * @return true if sending was successful or otherwise return false.
	 */
	virtual bool send(std::string number, std::string message);

private:
	/** 
	 * Device used to send sms messages.
	 */
	std::shared_ptr<smsDevice> sd;
};

#endif //_smsSender_hpp
