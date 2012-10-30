//
//  HGELogger.cpp
//  hybridge
//
//  Created by The Narrator on 10/29/12.
//
//

#include "service/HGELogger.h"

#include "service/HGEInterface.h"

#define LOGGING_ENABLED			0

NS_HGE_BEGIN

HGELogger::HGELogger() {
}

HGELogger::~HGELogger() {
}

bool HGELogger::initService(uint32_t argc, const char* argn[], const char* argv[], HGEToolbox * toolbox) {
	return !0;
}

bool HGELogger::consumeJSON(JSONValue& json, HGEToolbox * toolbox) {
#if LOGGING_ENABLED
	jsonlog(json, toolbox);
#endif
	
	return !0;
}

NS_HGE_END
