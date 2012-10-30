//
//  HGEInterface.h
//  hybridge
//
//  Created by The Narrator on 7/5/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEINTERFACE_H__
#define __HGEINTERFACE_H__

#include <map>
#include <list>
#include <string>

#include "service/HGEWorker.h"

#include "util/HGEUtilMutex.h"

NS_HGE_BEGIN

//class HGEWorker;
class HGELogger;
class HGEDispatch;
typedef std::list < HGEWorker * > HGEWorkerList;
typedef std::map< std::string, HGEWorker * > HGEWorkerMap;

/**
 the primary API through which JSON is bridged to services
 */
class HGEInterface : public HGEWorker {
public:
	HGEInterface(void * c);
	
	// The dtor makes the 3D context current before deleting the cube view, then
	// destroys the 3D context both in the module and in the browser.
	virtual ~HGEInterface();
	
	/**
	 prepare to provide service
	 */
	virtual bool initService(uint32_t argc, const char* argn[], const char* argv[], HGEToolbox * toolbox);
	
	/**
	 flush pending service response messages to the stream -- 
	 typically this should only be done by the specified client of the interface
	 which is responsible for passing the messages to the "web environment"
	 */
	void flushMessage(JSONBuffer& stream);
	
	/**
	 process a message and delegate the requested services to workers --
	 typically this should only be done by the specified client of the interface
	 which is responsible for collecting the messages from the "web environment"
	 */
	void handleMessage(const char * data, size_t length);
	
	/**
	 guard calls to moveJSON() from having to handle non-JSON-Object values
	 */
	void serveJSON(JSONValue& json);
	
private:
	/**
	 pass service request json to the relevant worker
	 */
	void moveJSON(JSONValue& json);
	
protected:
	/**
	 the interface itself is a worker and therefore must be equipped to handle service requests
	 */
	bool consumeJSON(JSONValue& json, HGEToolbox * toolbox);
	
public:
	/**
	 guard calls to deployJSON() from having to handle non-JSON-Object values -- 
	 typically this should only be done by HGEWorkers and HGEEntities
	 */
	void produceJSON(JSONValue& json, bool copy = !0);
	
private:
	/**
	 add json service responses to be flushed to the client
	 */
	void deployJSON(JSONValue& json, bool copy);
	
public:
	
	/**
	 recover when a worker fails to fulfill a service request -- 
	 typically this should only be done by the worker that itself failed
	 */
	void bounceJSON(JSONValue& json, HGEWorker * worker, HGEToolbox * toolbox);
	
	std::string& aliasName() { static std::string instance(HGE_KEYTEXT_SERVICE_INTERFACE); return instance; }
	
	/**
	 prepare a worker to respond to service requests
	 */
	static bool EnableWorker(HGEWorker * worker);
	
	/**
	 create a collection of utility references that is useful when providing services
	 */
	HGEToolbox getToolbox();
	
private:
	
	void * client;
	
	HGELogger * logger;
	HGEDispatch * dispatcher;
	
	/**
	 get the list of workers that to be that will respond to service requests
	 */
	static HGEWorkerList & WorkerQueue() {
		static HGEWorkerList * instance = new HGEWorkerList();
		return *instance;
	}
	
	HGEWorkerMap workers;
	
	size_t capacity;
	char * buffer;
	
	JSONValue * mainQueue;
	JSONValue * offQueue;
	
	HGEMutex mutex;
};

NS_HGE_END

#endif
