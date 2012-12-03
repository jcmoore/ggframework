//
//  HGEClock.h
//  hybridge
//
//  Created by The Narrator on 11/30/12.
//
//

#ifndef __HGECLOCK_H__
#define __HGECLOCK_H__

#include "service/HGEWorker.h"

#include "dev/HGETime.h"

NS_HGE_BEGIN

class HGEClock : public HGEWorker {
	
public:
	HGEClock(const char * a);
	virtual ~HGEClock();
	
protected:
	
	virtual bool digestJSON(JSONValue& json);
	
public:
	/**
	 report any pending work that has been completed but not yet furnished
	 */
	virtual bool reportJSON(JSONValue& result);
	
private:
	
	timeSD_hge lastTime;
	timeSD_hge age;
	
};

NS_HGE_END

#endif
