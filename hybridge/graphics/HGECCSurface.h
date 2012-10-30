//
//  HGECCSurface.h
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#ifndef __HGECCSURFACE_H__
#define __HGECCSURFACE_H__

#include "graphics/HGECCNexus.h"

NS_HGE_BEGIN

class HGECCSurface : public HGECCNexus {
	
	HGEClassifyKind(HGECCSurface, HGECCNexus);
public:
	
	HGECCSurface(id_hge unique)
	: HGECCNexus(unique) {};
	
	virtual bool destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
	
	
	virtual bool run(JSONValue const& json, bool implicit = 0);
	virtual bool run();
	
protected:
	
};

NS_HGE_END

#endif
