//
//  HGEUtilString.h
//  hybridge
//
//  Created by The Narrator on 8/26/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEUTILSTRING_H__
#define __HGEUTILSTRING_H__

#include "dev/HGEPlatformMacros.h"

#include <string.h>

NS_HGE_BEGIN

// const char * comparison class for use with STL collections
struct cmpstr
{
	bool operator()(const char * a, const char * b)
	{
		return strcmp(a, b) < 0;
		//return std::strcmp(a, b) < 0;
	}
};

NS_HGE_END

#endif
