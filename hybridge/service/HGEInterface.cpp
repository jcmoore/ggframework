//
//  HGEInterface.cpp
//  hybridge
//
//  Created by The Narrator on 7/5/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGEInterface.h"

#include "service/HGELogger.h"
#include "service/HGEDispatch.h"

#define VAPIDJSON_USE_PARSE_INSITU		0

NS_HGE_BEGIN

HGEInterface::HGEInterface(void * c)
: client(c)
, logger(0)
, dispatcher(0)
, workers()
, capacity(0)
, buffer(0)
, mainQueue(0)
, offQueue(0)
{
	{
		HGEMutex::Lock lock(this->mutex);
		this->mainQueue = new JSONValue();
		this->mainQueue->SetArray();
		
		this->offQueue = new JSONValue();
		this->offQueue->SetArray();
	}
}

HGEInterface::~HGEInterface() {
	for (HGEWorkerMap::iterator iter = this->workers.begin();
		 iter != this->workers.end(); iter++) {
		HGEWorker * worker = (*iter).second;
		delete worker;
	}
	HGEDeleteNull(this->logger);
	HGEDeleteNull(this->dispatcher);
	{
		HGEMutex::Lock lock(this->mutex);
		HGEDeleteNull(this->mainQueue);
		HGEDeleteNull(this->offQueue);
	}
	HGEFreeNull(this->buffer);
	this->capacity = 0;
}

bool HGEInterface::EnableWorker(HGEWorker * worker) {
	HGEInterface::WorkerQueue().push_back(worker);
	
	return true;
}

HGEToolbox HGEInterface::getToolbox() {
	
	HGEToolbox box = HGEToolbox::Grab();
	
	box.client = this->client;
	box.interface = this;
	
	return box;
}

bool HGEInterface::initService(uint32_t argc/* argc */,
							  const char* argn/* argn */[],
							  const char* argv/* argv */[],
							  HGEToolbox * toolbox) {
	
	// assign the required workers
	this->logger = new HGELogger();
	this->dispatcher = new HGEDispatch();
	
	this->workers[this->logger->aliasName()] = this->logger;
	this->workers[this->dispatcher->aliasName()] = this->dispatcher;
	
	HGEToolbox box = this->getToolbox();
	
	// add optionally enabled workers
	for (HGEWorkerList::iterator iter = HGEInterface::WorkerQueue().begin();
		 iter !=  HGEInterface::WorkerQueue().end(); iter++) {
		HGEWorker * worker = (*iter);
		
		if (worker) {
			std::string& name = worker->aliasName();
			HGEWorkerMap::iterator jter = this->workers.find(name);
			
			if (jter != this->workers.end()) {
				HGEAssertC(false, "HGE worker collision on name %s", name.c_str());
				continue;
			}
			
			(*iter) = 0; // worker is now owned by this interface instance
			this->workers[name] = worker;
			worker->initService(argc, argn, argv, &box);
		}
	}
	
	// clean up any unused workers
	for (HGEWorkerList::iterator iter = HGEInterface::WorkerQueue().begin();
		 iter !=  HGEInterface::WorkerQueue().end(); iter++) {
		HGEWorker * worker = (*iter);
		delete worker;
	}
	
	// clear the worker list
	HGEInterface::WorkerQueue().clear();
	
	return !0;
}

void HGEInterface::flushMessage(JSONBuffer& stream) {
	JSONValue * temp = 0;
	{
		HGEMutex::Lock lock(this->mutex);
		temp = this->offQueue;
		this->offQueue = this->mainQueue;
		this->mainQueue = temp;
		temp = this->offQueue;
	}
	
	JSONWriter output(stream);
	
	temp->Accept(output);
	temp->Clear();
}

void HGEInterface::handleMessage(const char * data, size_t length) {
	
	JSONDoc json;
	
#if VAPIDJSON_USE_PARSE_INSITU
	// WARNING: IT IS CRITICAL THAT EVERYTHING THAT NEEDS JSON VALUES FROM A ParseInsitu MAKE A COPY ITSELF (BUFFER FREEING CAN CAUSE SHENANIGANS -- ESP. FOR JSONObjects)
	if (this->capacity <= 0 ||
		this->capacity - 1 < length) {
		if (this->capacity == 0) {
			this->capacity = 1024;
		}
		
		while (this->capacity - 1 < length) {
			this->capacity *= 2;
		}
		
		free(this->buffer);
		this->buffer = (char *)malloc(this->capacity * sizeof(char));
	}
	
	
	memcpy(this->buffer, data, length);
	this->buffer[length] = '\0';
	
	bool blewup = json.ParseInsitu<0>(buffer).HasParseError(); blewup = blewup;
	// WARNING: IT IS CRITICAL THAT EVERYTHING THAT NEEDS JSON VALUES FROM A ParseInsitu MAKE A COPY ITSELF (BUFFER FREEING CAN CAUSE SHENANIGANS -- ESP. FOR JSONObjects)
#else
	bool blewup = json.Parse<0>(data).HasParseError(); blewup = blewup;
#endif
	
	this->serveJSON(json.GetValue());
}

void HGEInterface::serveJSON(JSONValue& json) {
	
	if (json.IsObject()) {
		this->moveJSON(json);
	} else if (json.IsArray()) {
		for (JSONValue::ValueIterator iter = json.Begin();
			 iter != json.End(); iter++) {
			JSONValue& value = *(*iter);
			this->moveJSON(value);
		}
	}
}

void HGEInterface::moveJSON(JSONValue& json) {
	if (!json.IsObject()) {
		HGEAssertC(0, "json argument must be a JSON Object");
		return;
	}
	
	HGEToolbox box = this->getToolbox();
	
	this->consumeJSON(json, &box);
}

void HGEInterface::bounceJSON(JSONValue& json, HGEWorker * worker, HGEToolbox * toolbox) {
	
	HGEAssertC(0, "service was unhandled by worker -- defaulting to dispatcher (which seems like bad form...)");
	//if (worker != this->dispatcher) {
	//	this->dispatcher->receiveJSON(json, toolbox);
	//}
}

bool HGEInterface::consumeJSON(JSONValue& json, HGEToolbox * toolbox) {
	
	bool result = !0;
	
	for (JSONValue::MemberIterator iter = json.MemberBegin();
		 iter != json.MemberEnd(); iter++) {
		
		JSONText const& key = iter->first;
		JSONValue& value = *iter->second;
		
		const JSONValue::Ch * str = key.c_str();
		
		HGEWorkerMap::iterator jter = this->workers.find(str);
		
		if (jter != this->workers.end()) {
			HGEWorker * part = (*jter).second;
			if (part) {
				result = part->receiveJSON(value, toolbox) && result;
				continue;
			}
		}
		
		result = 0;
		HGEAssertC(0, "unhandled service request");
	}
	
	return result;
}

void HGEInterface::deployJSON(JSONValue& json, bool copy) {
	
	if (json.IsObject()) {
		this->produceJSON(json, copy);
	} else if (json.IsArray()) {
		for (JSONValue::ValueIterator iter = json.Begin();
			 iter != json.End(); iter++) {
			JSONValue& value = *(*iter);
			this->produceJSON(value, copy);
		}
	}
}

void HGEInterface::produceJSON(JSONValue& json, bool copy) {
	
	if (!json.IsObject()) {
		HGEAssertC(0, "json argument must be a JSON Object");
		return;
	}
	
	{
		HGEMutex::Lock lock(this->mutex);
		// TODO: verify that the "copy" branch is different in a meaningful way from the "non-copy" branch
		if (copy) {
			JSONValue dupe;
			JSONDoc doc;
			doc.Reproduce(json, dupe);
			this->mainQueue->PushBack(dupe);
		} else {
			this->mainQueue->PushBack(json);
		}
	}
}

NS_HGE_END
