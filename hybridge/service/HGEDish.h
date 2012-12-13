//
//  HGEDish.h
//  hybridge
//
//  Created by The Narrator on 11/14/12.
//
//

#ifndef __HGEDISH_H__
#define __HGEDISH_H__

#include "service/HGEWorker.h"

NS_HGE_BEGIN

class HGEDish : public HGEWorker {
	
public:
	HGEDish(const char * a) : HGEWorker(a) {};
	virtual ~HGEDish() {};
	
protected:
	/**
	 >>: 
	 */
	virtual bool digestJSON(JSONValue& json);
	
public:
	
	virtual bool broadcastShake();
	
private:
	
};

NS_HGE_END

#endif
