//
//  HGEPlatformTerminal.h
//  hybridge
//
//  Created by The Narrator on 10/27/12.
//
//

#ifndef __HGEPLATFORMTERMINAL_H__
#define __HGEPLATFORMTERMINAL_H__

#include "dev/HGEPlatformMacros.h"
#include "bridge/HGETerminal.h"
#include "util/HGEUtilMutex.h"

#include <ppapi/cpp/instance.h>

#include <string>
#include <deque>

NS_HGE_BEGIN

class HGEPlatformTerminal : public HGETerminal {
	
	typedef std::deque< std::string > MessageQueue;
	
	pp::Instance* m_ppInstance;
	
	MessageQueue * mainQueue;
	MessageQueue * offQueue;
	
	HGEMutex mutex;
	
	friend class HGETerminal;
	
	static HGEPlatformTerminal *& sharedPlatformTerminalReference();
	
public:
	static HGEPlatformTerminal * sharedPlatformTerminal();
	
	HGEPlatformTerminal(pp::Instance* instance);
	~HGEPlatformTerminal();
	
	void bridgeInput();
	void bridgeOutput(std::string const& message);
};

NS_HGE_END

#endif
