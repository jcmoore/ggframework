//
//  HGEEntropy.cpp
//  hybridge
//
//  Created by The Narrator on 12/1/12.
//
//

#include "service/HGEEntropy.h"

#include <sstream>

NS_HGE_BEGIN

HGEEntropy::~HGEEntropy() {
}

HGEEntropy::HGEEntropy(const char * a)
: HGEWorker(a)
, seed()
, seeded(0) {
	time_t t;
	time(&t);
	std::ostringstream s;
	s << t;
	this->seed.assign(s.str());
}

bool HGEEntropy::digestJSON(JSONValue& json) {
	return 0;
}

bool HGEEntropy::reportJSON(JSONValue& json) {
	if (this->seeded) {
		return 0;
	}
	this->seeded = !0;
	// this is clumsy
	JSONValue value(vapidjson::kObjectType);
	value.AddMember("seed", this->seed.c_str());
	this->markJSON(json, value, 0);
	return 0; // nothing more to report
}

NS_HGE_END
