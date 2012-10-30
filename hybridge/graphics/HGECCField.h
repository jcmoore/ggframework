//
//  HGECCField.h
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#ifndef __HGECCFIELD_H__
#define __HGECCFIELD_H__

#include "graphics/HGECCNexus.h"

NS_HGE_BEGIN

class HGECCField : public HGECCNexus {
	
	HGEClassifyKind(HGECCField, HGECCNexus);
public:
	
	HGECCField(id_hge unique)
	: HGECCNexus(unique) {};
	
	virtual bool destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
protected:
	
};

NS_HGE_END

#endif
