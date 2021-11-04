/** \file smsDevice.hpp
 * Contains smsDevice class declaration.
 * Class smsDevice is used by smsSender class as dependency.
 * Class smsDevice doesn't use any other classes dependency. 
 */
#ifndef _smsDevice_hpp
#define _smsDevice_hpp

#include <string>

/**
 *  A smsDevice class.
 *  Provides functionality to initialize deinitialize and send sms message to dedicated numer.
 */
class smsDevice
{
public:
	/**
	 * Standard constructor.
	 * Initialize private members.
	 */
	smsDevice();

	/**
	 * Standard destructor.
	 * Just print some debug data.
	 */
	~smsDevice();

	/**
	 * Init device before sending a message.
         * @see send()
	 * @return true if initialization was successful or otherwise return false.
	 */
	virtual bool init();

	/**
	 * Send sms with the message to provided number.
	 * @param number as std::string which is requested number to receive message.
	 * @param message as std::string which is requested message to be send.
	 * @see init()
	 * @return true if sending was successful or otherwise return false.
	 */
	virtual bool send(std::string number, std::string message);

	/**
	 * Deinit device at the end of work.
	 * @return true if deinitialization was successful or otherwise return false.
	 */
	virtual bool deInit();

private:
	/** 
	 * Is device already initialized.
	 */
	bool initialized;
};

#endif //_smsDevice_hpp
