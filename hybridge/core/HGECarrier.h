//
//  HGECarrier.h
//  hybridge
//
//  Created by The Narrator on 8/23/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECARRIER_H__
#define __HGECARRIER_H__

#include "core/HGEEntity.h"
#include "service/HGEJSON.h"

NS_HGE_BEGIN

/**
 specialized entity that stores JSON
 */
class HGECarrier : public HGEEntity {
	HGEClassifyKind(HGECarrier, HGEEntity);
	
protected:
	HGECarrier(id_hge unique);
public:
	virtual ~HGECarrier();
	
protected:
	/**
	 get the contained data -- intended to be used only by subclasses
	 */
	HGEJSON& contents () { return this->data; }
	
	/**
	 hold the indicated JSON value
	 */
	void carry (JSONValue& json, HGEToolbox * toolbox) { this->data.mimic(json, toolbox); }
	
protected:
	HGEJSON data;
};

NS_HGE_END

#endif
