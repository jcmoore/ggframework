//
//  HGETerminal.h
//  hybridge
//
//  Created by The Narrator on 10/7/12.
//
//

#ifndef __HGETERMINAL_H__
#define __HGETERMINAL_H__

#include "dev/HGEPlatformMacros.h"

NS_HGE_BEGIN

class HGEInterface;

class HGETerminal {
	
protected:
	
	HGETerminal();
	~HGETerminal();
	
	HGEInterface * interface;
	
public:
	
	void loadApp(const char * path);
	
	void interfaceWithClient(void * client);
	
	void bridgeInputAndOutput();
	
	void execute();
	
	static HGETerminal * sharedTerminal();
};

NS_HGE_END

#endif
