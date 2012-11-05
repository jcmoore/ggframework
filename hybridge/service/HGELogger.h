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
	HGELogger(const char * a) : HGEWorker(a) {};
	virtual ~HGELogger() {};
	
protected:
	/**
	 <<: (messageJSON) logs the messageJSON as a string
	 */
	virtual bool digestJSON(JSONValue& json);
	
private:
	
};

NS_HGE_END

#endif
