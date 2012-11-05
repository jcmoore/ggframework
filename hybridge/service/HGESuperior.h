//
//  HGESuperior.h
//  hybridge
//
//  Created by The Narrator on 11/2/12.
//
//

#ifndef __HGESUPERIOR_H__
#define __HGESUPERIOR_H__

#include "dev/HGEPlatformMacros.h"

#include "service/HGEWorker.h"

#include <map>

NS_HGE_BEGIN

/**
 abstract base object capable of fulfilling a hybrid service request
 */
class HGESuperior : public HGEWorker {
	
	typedef std::map<const char *, HGEWorker *, cmpstr> Team;
	
public:
	
	typedef Team::const_iterator TeamIterator;
	
	HGESuperior(const char * a);
	virtual ~HGESuperior();
	
protected:
	/**
	 fulfill a service request specified by the provided JSON
	 */
	virtual bool digestJSON(JSONValue& json);
	
public:
	
	/**
	 furnish service responses to superior
	 */
	virtual bool produceJSON(JSONValue& json, bool purify = 0);
	
	/**
	 report any pending work that has been completed but not yet furnished
	 */
	virtual bool reportJSON(JSONValue& result);
	
	
	
	bool escalateJSON(JSONValue& json, bool purify = 0) {
		return HGEWorker::produceJSON(json, purify);
	}
	
	bool briefJSON(JSONValue& result) {
		return HGEWorker::reportJSON(result);
	}
	
	/**
	 add a new worker to the superior's team
	 */
	bool gainWorker(HGEWorker * worker);
	
	/**
	 remove a worker from the superior's team
	 (invokers of this method are responsible for deleting the worker)
	 */
	HGEWorker * lossWorker(const char * a);
	
	/**
	 assign a specific worker to be suprior's direct assistant
	 (add worker to the team if necessary)
	 */
	bool assignAssistant(HGEWorker * helper);
	
	HGEWorker * getAssistant() {
		return this->assistant;
	}
	
	HGEWorker * getHire (const char * a) {
		TeamIterator iter = this->hires.find(a);
		
		if (iter == this->hires.end()) {
			return 0;
		} else {
			return iter->second;
		}
	}
	
	TeamIterator teamBegin() { return this->hires.begin(); }
	TeamIterator teamEnd() { return this->hires.end(); }
	
protected:
	HGEWorker * lossWorker(Team::iterator iter);
	
	HGEWorker * assistant;
	Team hires;
};

NS_HGE_END

#endif
