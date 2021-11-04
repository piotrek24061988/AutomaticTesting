/** \file smsPlanner.hpp
 * Contains smsPlanner class declaration.
 * Class smsPlanner isn't used by any other class as dependency so far as it is top level interface.
 * Class smsPlanner uses timeKeeper and smsSender classes as dependency. 
 */
#ifndef _smsPlanner_hpp
#define _smsPlanner_hpp

#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "timeKeeper.hpp"
#include "smsSender.hpp"

/**
 *  A smsPlanner class.
 *  Provides functionality to add/remove messages to/from backlog which can be all send
 *  with one shut.
 */
class smsPlanner
{
public:
	/**
	 * Constructor taking over timeKeeper which will be used to verify send time
	 * and smsSender which will be used to send sms messages to dedicated numer.
	 * @param tk as std::shared_ptr<timeKeeper> which is used to verify send time.
	 * @param sd as std::shared_ptr<smsSender> which is used to send sms messages 
	 * to dedicated numer.
	 * @see addDelivery()
	 * @see sendAll()
	 */
	smsPlanner(std::shared_ptr<timeKeeper> tk, std::shared_ptr<smsSender> sd);

	/**
	 * Standard destructor.
	 * Just print some debug data.
	 */
	~smsPlanner();

	/**
	 * Add sms message to backlog (create delivery) to be send to dedicated number.
	 * @param targetNumber as std::string which is requested number to receive message.
	 * @param message as std::string which is requested message to be send.
	 * @param sendTime as std::time_t which is requested send time to be check.
	 * @see sendAll()
	 * @return id as int of prepared delivery if preparation was successful or -1 in not.
	 */
	int addDelivery(std::string targetNumber, std::string message, std::time_t sendTime);

	/**
	 * Remove sms message from backlog (cancel created delivery) to be send to dedicated number.
	 * @param curId as int which is id of prepared delivery.
	 * @see addDelivery()
	 * @return true if sms message was successfully removed from backlog or otherwise return false.
	 */
	bool cancelDelivery(int curId);

	/**
	 * Send backlog content (sms messages) to dedicated receivers (numbers) and clean backlog
	 * from each delivery which is succesfully send.
	 * @see addDelivery()
	 * @return true if whole backlog was succesfully send or otherwise return false.
	 */
	bool sendAll();

private:
	/** 
	 * Last created delivery id.
	 */
	int id;

	/** 
	 * All created delivery ids to be send.
	 */
	std::vector<int> ids;

	/** 
	 * Sms message and target number for each delivey.  
	 */
	std::map<int, std::pair<std::string, std::string>> contents;
	
	/** 
	 * Time validator for each created delivery.
	 */
	std::shared_ptr<timeKeeper> tk;

	/** 
	 * Sender which will be used to send each sms message to dedicated numer.
	 */
	std::shared_ptr<smsSender> sd;
};

#endif //_smsPlanner_hpp
