//
//  HGEWorker.h
//  hybridge
//
//  Created by The Narrator on 7/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEHANDLER_H__
#define __HGEHANDLER_H__

#include "dev/HGEPlatformMacros.h"

#include "service/HGEToolbox.h"
#include "service/HGEJSON.h"

#include <string>

NS_HGE_BEGIN

/**
 abstract base object capable of fulfilling a hybrid service request
 */
class HGEWorker {
	
public:
	HGEWorker() {};
	virtual ~HGEWorker() {};
	
	/**
	 prepare to fulfill service requests
	 */
	virtual bool initService(uint32_t argc, const char* argn[], const char* argv[], HGEToolbox * toolbox) = 0;
	
	/**
	 service requests are expected to be JSON Objects for consumption
	 however, JSON Arrays, may be provided to support the fulfillment
	 of multiple sequential requests
	 this public interface handles requests whether Array or Object
	 */
	virtual bool receiveJSON(JSONValue& json, HGEToolbox * toolbox);
protected:
	/**
	 fulfill a service request specified by the provided JSON
	 */
	virtual bool consumeJSON(JSONValue& json, HGEToolbox * toolbox) = 0;
	
public:
	/**
	 the name of the service the worker can fulfill requests for
	 */
	virtual std::string& aliasName() = 0;
};

NS_HGE_END

#endif
