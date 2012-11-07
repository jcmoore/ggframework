//
//  HGEGate.h
//  hybridge
//
//  Created by The Narrator on 11/5/12.
//
//

#ifndef __HGEGATE_H__
#define __HGEGATE_H__

#include "service/HGEWorker.h"

#include "util/HGEUtilMutex.h"

NS_HGE_BEGIN

/**
 worker in charge of handling arbitrary log messages
 from the serializing application in the web environment
 */
class HGEGate : public HGEWorker {
	
public:
	HGEGate(const char * a);
	virtual ~HGEGate();
	
protected:
	
	bool openJSON(JSONValue& result);
	
	virtual bool digestJSON(JSONValue& json);
	
public:
	/**
	 furnish service responses to superior
	 */
	virtual bool produceJSON(JSONValue& json, bool purify = 0);
	
	/**
	 report any pending work that has been completed but not yet furnished
	 */
	virtual bool reportJSON(JSONValue& result);
	
private:
	
	JSONValue * mainQueue;
	JSONValue * offQueue;
	
	HGEMutex mutex;
	
};

NS_HGE_END

#endif