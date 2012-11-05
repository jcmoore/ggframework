//
//  HGELogger.cpp
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#include "service/HGELogger.h"

#define LOGGING_ENABLED			0

NS_HGE_BEGIN

bool HGELogger::digestJSON(JSONValue& json) {
#if LOGGING_ENABLED
	jsonlog(json);
#endif
	
	return !0;
}

NS_HGE_END
