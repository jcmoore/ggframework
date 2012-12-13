//
//  HGEAPI.h
//  hybridge
//
//  Created by The Narrator on 7/5/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEAPI_H__
#define __HGEAPI_H__

#include <map>
#include <list>
#include <string>

#include "service/HGEWorker.h"
#include "service/HGEMemoizer.h"

NS_HGE_BEGIN

/**
 the primary API through which JSON is bridged to services
 */
class HGEAPI {
public:
	HGEAPI(void * c)
	: client(c) {}
	
	// The dtor makes the 3D context current before deleting the cube view, then
	// destroys the 3D context both in the module and in the browser.
	virtual ~HGEAPI() {}
	
	/**
	 flush pending service response messages to the stream -- 
	 typically this should only be done by the specified client of the interface
	 which is responsible for passing the messages to the "web environment"
	 */
	virtual void flushMessage(JSONBuffer& stream) = 0;
	
	/**
	 process a message and delegate the requested services to workers --
	 typically this should only be done by the specified client of the interface
	 which is responsible for collecting the messages from the "web environment"
	 */
	virtual void handleMessage(const char * data, size_t length) = 0;
	
protected:
	
	void * client;
};





class HGEAPIWorker : public HGEAPI {
public:
	HGEAPIWorker(void * c, HGEWorker * worker, HGEMemoizer * memoizer = 0)
	: HGEAPI(c)
	, chief(worker)
	, scribe(memoizer)
	, capacity(0)
	, buffer(0) {}
	
	// The dtor makes the 3D context current before deleting the cube view, then
	// destroys the 3D context both in the module and in the browser.
	virtual ~HGEAPIWorker() {
		HGEFreeNull(this->chief);
		HGEFreeNull(this->buffer);
		this->capacity = 0;
	}
	
	/**
	 flush pending service response messages to the stream --
	 typically this should only be done by the specified client of the interface
	 which is responsible for passing the messages to the "web environment"
	 */
	virtual void flushMessage(JSONBuffer& stream);
	
	/**
	 process a message and delegate the requested services to workers --
	 typically this should only be done by the specified client of the interface
	 which is responsible for collecting the messages from the "web environment"
	 */
	virtual void handleMessage(const char * data, size_t length);
	
protected:
	
	HGEWorker * chief;
	HGEMemoizer * scribe;
	size_t capacity;
	char * buffer;
};

NS_HGE_END

#endif
