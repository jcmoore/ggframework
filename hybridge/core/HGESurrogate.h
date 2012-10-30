//
//  HGESurrogate.h
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGESURROGATE_H__
#define __HGESURROGATE_H__

#include "core/HGECarrier.h"

NS_HGE_BEGIN

/**
 class acts as a reference for a json value, particularly used in the context of HGEJSONRef fusions
 */
class HGESurrogate : public HGECarrier {
	HGEClassifyKind(HGESurrogate, HGECarrier);
	
public:
	HGESurrogate(id_hge unique);
	virtual ~HGESurrogate();
	
	/**
	 destruction: () none
	 */
	virtual bool destroyJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	/**
	 construction: ({'$$':substituteJSON, ...}) carry part of the provided json and act as a substitute for it through HGEJSONRef fusions
	 */
	virtual bool createJSON(JSONValue& json, bool firstResponder, HGEToolbox * toolbox);
	
	/**
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, HGEToolbox * toolbox);
	
	friend class HGEJSONRef;
};

NS_HGE_END

#endif
