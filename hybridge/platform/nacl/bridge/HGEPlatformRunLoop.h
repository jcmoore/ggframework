//
//  HGEPlatformRunLoop.h
//  hybridge
//
//  Created by The Narrator on 10/27/12.
//
//

#ifndef __HGEPLATFORMRUNLOOP_H__
#define __HGEPLATFORMRUNLOOP_H__

#include "dev/HGEPlatformMacros.h"
#include "bridge/HGERunLoop.h"

#include "platform/nacl/ZFileSystem.h"

#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/utility/completion_callback_factory.h>

NS_HGE_BEGIN

class HGETerminal;

class HGEPlatformRunLoop : public HGERunLoop {
	
protected:
	
	HGEPlatformRunLoop(pp::Instance* instance);
	~HGEPlatformRunLoop();
    
    pp::Instance* m_ppInstance;
    
    pp::CompletionCallbackFactory<HGEPlatformRunLoop> m_obFactory;
    
    cocos2d::ZFileSystem* m_pobFileSystem;
    
    HGETerminal* m_pobTerminal;
	
public:
	virtual void startApp(const char * path);
	
	void startAppCallback(int32_t result);
	
	void threadCycle(int32_t result);
};

NS_HGE_END

#endif
