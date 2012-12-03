//
//  HGEGate.cpp
//  hybridge
//
//  Created by The Narrator on 11/5/12.
//
//

#include "service/HGEGate.h"

#include "service/HGESuperior.h"



#define NOTIFY_FLUSH		0



NS_HGE_BEGIN

HGEGate::HGEGate(const char * a, bool doubleBuffered)
: HGEWorker(a)
, isDoubleBuffered(doubleBuffered)
, mainQueue(new JSONValue())
, offQueue(new JSONValue())
, reporter(vapidjson::kUndefinedType)
, rter(0)
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
	
#if NOTIFY_FLUSH
	// WARNING: json is about to be sullied!
	this->markJSON(brief, json, 0);
	
	this->produceJSON(brief);
#endif
	
	if (!this->isDoubleBuffered) {
		return !0;
	}
	
	if (!this->offQueue->Empty()) {
		//HGEAssertC(0, "unexpected (but note catastrophic[?]) gate flush request when off queue is not empty (investigate why)");
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
	
	bool willReport = 0;
	
	if (this->reporter.IsUndefined()) {
		this->openJSON(this->reporter);
		HGEAssertC(this->reporter.IsArray(), "reporter must be an array...");
		this->rter = this->reporter.Begin();
	}
	
	HGEAssertC(this->reporter.IsArray(), "reporter must be an array");
	
	if ( ( willReport = !( this->rter == this->reporter.End() ) ) ) {
		result = (**this->rter);
		willReport = !( ++this->rter == this->reporter.End() );
	}
	
	if (willReport) {
		return !0;
	}
	
	this->reporter.SetUndefined();
	return 0;
	
	//JSONValue brief;
	//this->openJSON(brief);
	//this->getSuperior()->markJSON(result, brief, 0);
	//return !0;
}

bool HGEGate::openJSON(JSONValue& result) {
	
	{
		HGEMutex::Lock lock(this->mutex);
		if (this->isDoubleBuffered) {
			result = *this->offQueue;
			this->offQueue->SetArray();
		} else {
			result = *this->mainQueue;
			this->mainQueue->SetArray();
		}
	}
	return !0;
}

NS_HGE_END
