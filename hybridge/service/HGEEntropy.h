//
//  HGEEntropy.h
//  hybridge
//
//  Created by The Narrator on 12/1/12.
//
//

#ifndef __HGEENTROPY_H__
#define __HGEENTROPY_H__

#include "service/HGEWorker.h"

NS_HGE_BEGIN

class HGEEntropy : public HGEWorker {
	
public:
	HGEEntropy(const char * a);
	virtual ~HGEEntropy();
	
protected:
	
	virtual bool digestJSON(JSONValue& json);
	
public:
	/**
	 report any pending work that has been completed but not yet furnished
	 */
	virtual bool reportJSON(JSONValue& result);
	
private:
	std::string seed;
	unsigned char seeded;
};

NS_HGE_END

#endif
