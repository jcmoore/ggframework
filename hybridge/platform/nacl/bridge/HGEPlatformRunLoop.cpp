//
//  HGERunLoop.cpp
//  hybridge
//
//  Created by The Narrator on 10/27/12.
//
//

#include "bridge/HGEPlatformRunLoop.h"
#include "bridge/HGERunLoop.h"

#include "bridge/HGETerminal.h"
#include "bridge/HGEPlatformTerminal.h"

#include "dev/HGEMacros.h"

NS_HGE_BEGIN

HGEPlatformRunLoop::HGEPlatformRunLoop(pp::Instance* instance)
: HGERunLoop()
, m_ppInstance(instance)
, m_obFactory(this)
, m_pobFileSystem(0) {
    HGEAssertC(!m_ppInstance, "pepper instance must be valid");
}

HGEPlatformRunLoop::~HGEPlatformRunLoop() {
	delete m_pobFileSystem;
}

void HGEPlatformRunLoop::startApp(const char * path) {
    HGEAssertC(!m_pobFileSystem, "filesystem has already been requested");
    m_pobFileSystem = new cocos2d::ZFileSystem(m_ppInstance, m_obFactory.NewCallback(&HGEPlatformRunLoop::startAppCallback));
    
    m_pobFileSystem->DownloadArchive(path);
    
    HGERunLoop::startApp(path);
}

void HGEPlatformRunLoop::startAppCallback(int32_t result) {
	HGEAssertC(result == PP_OK, "filesystem loading was problematic");
	
	//HGEPlatformTerminal::sharedPlatformTerminal()->loadApp(path);
	HGEPlatformTerminal * terminal = HGEPlatformTerminal::sharedPlatformTerminal();
	//HGEPlatformClient * client = new HGEPlatformClient(m_ppInstance);
	
	terminal->interfaceWithClient(m_ppInstance);
	
	this->begin();
	this->threadCycle(result);
}

void HGEPlatformRunLoop::threadCycle(int32_t result) {
	
	// present results of previous application tasks
	this->present();
	
	// swap messages across the bridge
	HGEPlatformTerminal::sharedPlatformTerminal()->bridgeInput();
	
	// process pending bridge messages
	HGEPlatformTerminal::sharedPlatformTerminal()->execute();
	
	// execute periodic application tasks
	this->update();
	
	
	
	static timeSD_hge checkpoint = 0;
	static unsigned int reps = 0;
	
	reps++;
	
	timeSD_hge marker = HGE_TIME_SD();
	
	if (checkpoint == 0)
	{
		checkpoint = marker;
	}
	else if (marker - checkpoint > 5)
	{
		float fps = (float)(reps / (marker - checkpoint));
		
		HGEPrintln("GL FPS: %f", fps);
		
		reps = 0;
		
		checkpoint = marker;
	}
}





HGERunLoop *& HGERunLoop::sharedRunLoopReference() {
	static HGERunLoop * instance = 0;
	
	return instance;
}

HGERunLoop * HGERunLoop::sharedRunLoop() {
	HGEAssertC(HGERunLoop::sharedRunLoopReference(), "shared run loop was not instantiated");
	
	return HGERunLoop::sharedRunLoopReference();
}

HGERunLoop::HGERunLoop() {
	HGEAssertC(!HGERunLoop::sharedRunLoopReference(), "unexpected construction of multiple HGERunLoop objects");
	HGERunLoop::sharedRunLoopReference() = this;
}

HGERunLoop::~HGERunLoop() {
	HGEAssertC(this == HGERunLoop::sharedRunLoopReference(), "unexpected destruction of a HGERunLoop object that was not the shared instance");
	HGERunLoop::sharedRunLoopReference() = 0;
}

void HGERunLoop::startApp(const char * path) {
	//cocos2d::CCApplication::sharedApplication()->load(this, "/hge2cc_0/Resources.zip");
	//[[HGEPlatformRunLoop sharedPlatformRunLoop] startApp:path];
}

void HGERunLoop::setInterval(double interval) {
	//[[HGEPlatformRunLoop sharedPlatformRunLoop] setLoopInterval:interval];
}

double HGERunLoop::getInterval() {
	HGEAssertC(0, "HGERunLoop::getInterval() is not properly implemented");
	return 0;
	//return [[HGEPlatformRunLoop sharedPlatformRunLoop] getLoopInterval];
}

NS_HGE_END
