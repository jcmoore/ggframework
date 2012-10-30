//
//  HGELogger.h
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#ifndef __HGELOGGER_H__
#define __HGELOGGER_H__

#include "service/HGEWorker.h"

NS_HGE_BEGIN

/**
 worker in charge of handling arbitrary log messages
 from the serializing application in the web environment
 */
class HGELogger : public HGEWorker {
	
public:
	HGELogger();
	virtual ~HGELogger();
	
	/**
	 prepare to provide service
	 */
	virtual bool initService(uint32_t argc, const char* argn[], const char* argv[], HGEToolbox * toolbox);
protected:
	/**
	 hge<<: (messageJSON) logs the messageJSON as a string
	 */
	bool consumeJSON(JSONValue& json, HGEToolbox * toolbox);
	
public:
	std::string& aliasName() { static std::string instance(HGE_KEYTEXT_SERVICE_LOGGER); return instance; }
	
private:
	
};

NS_HGE_END

#endif
