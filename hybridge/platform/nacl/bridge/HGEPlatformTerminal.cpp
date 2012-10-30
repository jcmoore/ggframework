//
//  HGEPlatformTerminal.cpp
//  hybridge
//
//  Created by The Narrator on 10/27/12.
//
//

#include "dev/HGEMacros.h"

#include "bridge/HGETerminal.h"
#include "bridge/HGEPlatformTerminal.h"
#include "bridge/HGEPlatformRunLoop.h"


#include "service/HGEJSON.h"
#include "service/HGEInterface.h"

NS_HGE_BEGIN


HGEPlatformTerminal *& HGEPlatformTerminal::sharedPlatformTerminalReference() {
	static HGEPlatformTerminal * instance = 0;
	
	return instance;
}

HGEPlatformTerminal * HGEPlatformTerminal::sharedPlatformTerminal() {
	HGEAssertC(HGEPlatformTerminal::sharedPlatformTerminalReference(), "shared platform terminal was not instantiated");
	
	return HGEPlatformTerminal::sharedPlatformTerminalReference();
}

HGEPlatformTerminal::HGEPlatformTerminal(pp::Instance* instance)
: HGETerminal()
, m_ppInstance(instance)
, mainQueue(new MessageQueue())
, offQueue(new MessageQueue()) {
    HGEAssertC(!m_ppInstance, "pepper instance must be valid");
	HGEAssertC(!HGEPlatformTerminal::sharedPlatformTerminalReference(), "unexpected construction of multiple HGEPlatformTerminal objects");
	HGEPlatformTerminal::sharedPlatformTerminalReference() = this;
}

HGEPlatformTerminal::~HGEPlatformTerminal() {
	HGEAssertC(this == HGEPlatformTerminal::sharedPlatformTerminalReference(), "unexpected destruction of a HGEPlatformTerminal object that was not the shared instance");
	HGEPlatformTerminal::sharedPlatformTerminalReference() = 0;
}

void HGEPlatformTerminal::bridgeInput() {
	hybridge::JSONBuffer stream;
	
	this->interface->flushMessage(stream);
	
	std::string nacl = stream.GetString();
	nacl.insert(0, "hybridge: ");
	
	m_ppInstance->PostMessage(pp::Var(nacl.c_str()));
}

void HGEPlatformTerminal::bridgeOutput(std::string const& message) {
	if (!message.empty()) {
		{
			HGEMutex::Lock lock(this->mutex);
			mainQueue->push_back(message);
		}
	}
}






HGETerminal * HGETerminal::sharedTerminal() {
	return HGEPlatformTerminal::sharedPlatformTerminal();
}

HGETerminal::HGETerminal()
: interface(0) {
	
}

HGETerminal::~HGETerminal() {
	HGEDeleteNull(this->interface);
}

void HGETerminal::loadApp(const char * path) {
	//// WARNING: must be setup on the main thread!
	//[[HGEPlatformTerminal sharedPlatformTerminal] loadAppPath:path];
}

void HGETerminal::interfaceWithClient(void * client) {
	HGEDeleteNull(this->interface);
	this->interface = new HGEInterface(client);
}

void HGETerminal::bridgeInputAndOutput() {
}

void HGETerminal::execute() {
	HGEPlatformTerminal * platformTerminal = HGEPlatformTerminal::sharedPlatformTerminal();
	
	HGEPlatformTerminal::MessageQueue * queue = 0;
	
	{
		HGEMutex::Lock lock(platformTerminal->mutex);
		HGEPlatformTerminal::MessageQueue * mainQueue = platformTerminal->mainQueue;
		HGEPlatformTerminal::MessageQueue * offQueue = platformTerminal->offQueue;
		
		offQueue->clear();
		
		queue = offQueue;
		offQueue = mainQueue;
		mainQueue = queue;
		queue = offQueue;
		
		platformTerminal->offQueue = offQueue;
		platformTerminal->mainQueue = mainQueue;
	}
	
	for (HGEPlatformTerminal::MessageQueue::iterator iter = queue->begin();
		iter != queue->end(); iter++) {
		this->interface->handleMessage((*iter).c_str(), (*iter).size());
	}
}

NS_HGE_END
