//
//  HGEWorker.h
//  hybridge
//
//  Created by The Narrator on 7/4/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEWORKER_H__
#define __HGEWORKER_H__

#include "dev/HGEPlatformMacros.h"

#include "service/HGEJSON.h"

#include "core/HGEEntity.h"

#include <string>

NS_HGE_BEGIN

class HGESuperior;

/**
 abstract base object capable of fulfilling a hybrid service request
 */
class HGEWorker : public
HGECircuit <
HGEImplementer	<
HGEWorker, HGEEntity > > {
	
public:
	HGEWorker(const char * a)
	: alias(a),
	superior(0) {
	};
	virtual ~HGEWorker() {};
	
	/**
	 service requests are expected to be JSON Objects for consumption
	 however, JSON Arrays, may be provided to support the fulfillment
	 of multiple sequential requests
	 this public interface handles requests whether Array or Object
	 */
	bool consumeJSON(JSONValue& json);
	
protected:
	/**
	 fulfill a service request specified by the provided JSON
	 */
	virtual bool digestJSON(JSONValue& json) = 0;
	
public:
	
	/**
	 furnish service responses to superior
	 */
	virtual bool produceJSON(JSONValue& json, bool purify = 0);
	
	/**
	 report any pending work that has been completed but not yet furnished
	 */
	virtual bool reportJSON(JSONValue& result);
	
private:
	
	std::string alias;
	
	HGESuperior * superior;
	
public:
	
	/**
	 attribute a response according to alias
	 */
	void markJSON(JSONValue& result, JSONValue& json, bool purify);
	
	/**
	 get the name of the service the worker can fulfill requests for
	 */
	std::string const& getAlias() { return this->alias; };
	
	/**
	 get the superior of the worker
	 */
	HGESuperior * getSuperior() { return this->superior; }
	
private:
	
	/**
	 set the superior of the worker (only to be invoked by the superior itself)
	 */
	void setSuperior(HGESuperior * s) { this->superior = s; }
	
	friend class HGESuperior;
};

NS_HGE_END

#endif
