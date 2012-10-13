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

NS_HGE_BEGIN

class HGERunLoop {
	
	HGERunLoop();
	~HGERunLoop();
	
public:
	static HGERunLoop * sharedRunLoop();
	
	void startApp(const char * path);
	
	void setInterval(double interval);
	
	double getInterval();
};

NS_HGE_END

#endif
