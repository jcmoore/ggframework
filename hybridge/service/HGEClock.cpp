//
//  HGEClock.cpp
//  hybridge
//
//  Created by The Narrator on 11/30/12.
//
//

#include "service/HGEClock.h"

NS_HGE_BEGIN

HGEClock::~HGEClock() {
}

HGEClock::HGEClock(const char * a)
: HGEWorker(a)
, lastTime(0)
, age(0) {
	
}

bool HGEClock::digestJSON(JSONValue& json) {
	return 0;
}

bool HGEClock::reportJSON(JSONValue& json) {
	//static bool did = 0;
	//if (did)
	//return 0;
	//did = !0;
	
	// this is clumsy
	JSONValue value(vapidjson::kObjectType);
	
	timeSD_hge t = HGE_TIME_SD();
	value.AddMember("t", t);
	//value.AddMember("tt", this->lastTime);
	
	//timeSD_hge dt = t - this->lastTime;
	//value.AddMember("dt", dt);
	
	//this->age += dt;
	//value.AddMember("a", age);
	
	this->markJSON(json, value, 0);
	this->lastTime = t;
	return 0; // nothing more to report
}

NS_HGE_END
