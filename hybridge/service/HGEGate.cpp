//
//  HGEGate.cpp
//  hybridge
//
//  Created by The Narrator on 11/5/12.
//
//

#include "service/HGEGate.h"

#include "service/HGESuperior.h"



NS_HGE_BEGIN

HGEGate::HGEGate(const char * a)
: HGEWorker(a)
, mainQueue(new JSONValue())
, offQueue(new JSONValue())
{
	{
		HGEMutex::Lock lock(this->mutex);
		this->mainQueue = new JSONValue();
		this->mainQueue->SetArray();
		
		this->offQueue = new JSONValue();
		this->offQueue->SetArray();
	}
}

HGEGate::~HGEGate() {
	{
		HGEMutex::Lock lock(this->mutex);
		HGEDeleteNull(this->mainQueue);
		HGEDeleteNull(this->offQueue);
	}
}

bool HGEGate::digestJSON(JSONValue& json) {
	
	JSONValue brief;
	
	// WARNING: json is about to be sullied!
	this->markJSON(brief, json, 0);
	
	this->produceJSON(brief);
	
	if (!this->offQueue->Empty()) {
		HGEAssertC(0, "unexpected (but note catastrophic[?]) gate flush request when off queue is not empty (investigate why)");
		this->openJSON(brief); // clear what will become the main queue
	}
	
	JSONValue * temp = 0;
	{
		HGEMutex::Lock lock(this->mutex);
		temp = this->offQueue;
		this->offQueue = this->mainQueue;
		this->mainQueue = temp;
		temp = this->offQueue;
	}
	
	HGESuperior * superior = this->getSuperior();
	
	if (superior) {
		bool escalated = 0;
		{
			HGEMutex::Lock lock(this->mutex);
			escalated = superior->escalateJSON(*this->offQueue);
			if (!this->offQueue->IsArray()) {
				this->offQueue->SetArray();
			}
			// this->offQueue must be an array when the lock is release!
		}
		if (escalated) {
			this->openJSON(brief);
		}
	} else {
		
	}
	
	return !0;
}

bool HGEGate::produceJSON(JSONValue& json, bool purify) {
	
	if (purify) {
		JSONValue dupe;
		JSONDoc doc;
		doc.Reproduce(json, dupe);
		
		{
			HGEMutex::Lock lock(this->mutex);
			this->mainQueue->PushBack(dupe);
		}
	} else {
		{
			HGEMutex::Lock lock(this->mutex);
			this->mainQueue->PushBack(json);
		}
	}
	
	return !0;
}

bool HGEGate::reportJSON(JSONValue& result) {
	JSONValue brief;
	this->openJSON(brief);
	this->getSuperior()->markJSON(result, brief, 0);
	return !0;
}

bool HGEGate::openJSON(JSONValue& result) {
	
	{
		HGEMutex::Lock lock(this->mutex);
		result = *this->offQueue;
		this->offQueue->SetArray();
	}
	return !0;
}

NS_HGE_END
