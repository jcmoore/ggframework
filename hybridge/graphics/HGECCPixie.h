//
//  HGECCPixie.h
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#ifndef __HGECCPIXIE_H__
#define __HGECCPIXIE_H__

#include "graphics/HGECCNexus.h"
#include "core/HGERef.h"

NS_HGE_BEGIN

class HGECCFabric;

class HGECCPixie : public HGECCNexus {
	
	HGEClassifyKind(HGECCPixie, HGECCNexus);
public:
	
	HGECCPixie(id_hge unique)
	: HGECCNexus(unique)
	, fabricRef() {}
	
	virtual bool destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
	virtual void autoCorrect(update_priority phase);
	
	
	
	virtual bool setBox(JSONValue const& json, bool implicit = 0);
	virtual bool setBox(float x, float y, float w, float h);
	
	virtual bool setPigment(JSONValue const& json, bool implicit = 0);
	virtual bool setPigment(float r, float g, float b);
	
	virtual bool setDensity(JSONValue const& json, bool implicit = 0);
	virtual bool setDensity(float a);
	
	virtual bool setFabric(JSONValue const& json, bool implicit = 0);
	virtual bool setFabric(HGECCFabric * fabric);
	
protected:
	
	HGERef<HGECCFabric> fabricRef;
	
};

NS_HGE_END

#endif
