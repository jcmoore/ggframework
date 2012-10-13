//
//  HGEToolbox.h
//  hybridge
//
//  Created by The Narrator on 9/2/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGETOOLBOX_H__
#define __HGETOOLBOX_H__

#define HGEToolboxLog(x, y)			HGEClientLog((x)->client, y)

#include "dev/HGEConfig.h"

#if HGE_TARGET_PLATFORM == HGE_PLATFORM_NACL
#	include "ppapi/cpp/instance.h"
#	include "ppapi/cpp/var.h"
#	define HGEClientLog(x, y)		{if (x) {((pp::Instance *)x)->PostMessage(pp::Var(y));}}
#else
#	define HGEClientLog(x, y)		HGEPrint("\nHGEClientLog: %s", y)
#endif

#include "dev/HGEPlatformMacros.h"

NS_HGE_BEGIN

class HGEInterface;

/**
 object to contain useful items for fulfilling service requests
 */
struct HGEToolbox {
	
	/**
	 factory function for toolboxes
	 */
	static HGEToolbox Grab() { return HGEToolbox(); }
	
	void * client;
	HGEInterface * interface;
	
private:
	HGEToolbox()
	: client(0)
	, interface(0)
	{
		
	}
};

NS_HGE_END

#endif
