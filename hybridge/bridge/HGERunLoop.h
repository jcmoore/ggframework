//
//  HGERunLoop.h
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

#ifndef __HGERUNLOOP_H__
#define __HGERUNLOOP_H__

#include "dev/HGEPlatformMacros.h"

#include "service/HGEAPI.h"

NS_HGE_BEGIN

class HGERunLoop {
	
	static HGERunLoop *& sharedRunLoopReference();
	
protected:
	
	HGERunLoop();
	~HGERunLoop();
	
public:
	static HGERunLoop * sharedRunLoop();
	
	void setInterval(double interval);
	
	double getInterval();
	
	virtual void startApp(const char * path);
	
	virtual HGEAPI * attach(void * client) = 0;
	
	virtual void begin() = 0;
	
	virtual void update() = 0;
	
	virtual void present() = 0;
};

NS_HGE_END

#endif
