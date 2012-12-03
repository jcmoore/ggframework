//
//  HGESuperior.cpp
//  hybridge
//
//  Created by The Narrator on 11/2/12.
//
//

#include "service/HGESuperior.h"

#include "service/HGEAPI.h"

NS_HGE_BEGIN

HGESuperior::~HGESuperior() {
	for (Team::iterator iter = this->hires.begin(); iter != this->hires.end(); ) {
		HGEWorker * worker = this->lossWorker(iter++); // NOTE: critical iterator incrementation timing here
		HGEAssertC(worker, "probably leaking memory . . .");
		HGEDeleteNull(worker);
	}
	
	HGEAssertC(!this->assistant, "leaking on superiors assistant");
	HGEAssertC(this->hires.empty(), "leaking team?");
}

HGESuperior::HGESuperior(const char * a)
: HGEWorker(a)
, assistant(0) {
	
}

bool HGESuperior::digestJSON(JSONValue& json) {
	
	bool result = !0;
	
	for (JSONValue::MemberIterator iter = json.MemberBegin();
		 iter != json.MemberEnd(); iter++) {
		
		JSONText const& key = iter->first;
		JSONValue& value = *iter->second;
		
		const JSONValue::Ch * str = key.c_str();
		
		TeamIterator jter = this->hires.find(str);
		
		if (jter != this->hires.end()) {
			HGEWorker * part = (*jter).second;
			if (part) {
				result = part->consumeJSON(value) && result;
				continue;
			}
		}
		
		HGEAssertC(0, "unhandled service request");
		result = 0;
	}
	
	return result;
}

bool HGESuperior::produceJSON(JSONValue& json, bool purify) {
	if (!this->assistant) {
		return this->escalateJSON(json, purify);
	}
	
	return this->assistant->produceJSON(json, purify);
}

bool HGESuperior::reportJSON(JSONValue& result) {
	
	result.SetArray();
	
	for (Rank::iterator jter = this->seniority.begin(); jter != this->seniority.end(); jter++) {
		HGEWorker * worker = (*jter);
		bool willReport = !0;
		do {
			JSONValue json(vapidjson::kUndefinedType);
			willReport = worker->reportJSON(json); // this is ugly...
			if (json.IsUndefined()) {
				continue;
			}
			JSONValue brief;
			this->markJSON(brief, json, 0);
			result.PushBack(brief);
		} while (willReport);
	}
	
	return !0;
	
	//if (!this->assistant) {
	//	return this->briefJSON(result);
	//}
	//
	//return this->assistant->reportJSON(result);
}



bool HGESuperior::gainWorker(HGEWorker * worker) {
	
	if (!worker) {
		HGEAssertC(0, "cannot add empty workers");
		return 0;
	}
	
	const char * a = worker->getAlias().c_str();
	HGEWorker *& teamMember = this->hires[a];
	
	if (teamMember != 0) {
		if (teamMember == worker) {
			return !0;
		}
		HGEAssertC(0, "worker collision on name %s", a);
		return false;
	}
	
	HGESuperior * boss = worker->getSuperior();
	
	if (boss != 0) {
		HGEAssertC(0, "worker already has an superior %s", boss->getAlias().c_str());
		return false;
	}
	
	teamMember = worker;
	worker->setSuperior(this);
	this->seniority.push_back(worker);
	
	return !0;
}

HGEWorker * HGESuperior::lossWorker(const char * a) {
	
	Team::iterator iter = this->hires.find(a);
	
	if (iter == this->hires.end()) {
		return 0;
	}
	
	return this->lossWorker(iter);
}

HGEWorker * HGESuperior::lossWorker(Team::iterator iter) {
	
	HGEWorker * worker = iter->second;
	this->hires.erase(iter);
	this->seniority.remove(iter->second);
	HGESuperior * boss = worker->getSuperior();
	
	
	if (boss != this) {
		HGEAssertC(boss->getHire(iter->first), "worker may be unemployed (but thinks its boss is %s)", boss->getAlias().c_str());
		HGEAssertC(0, "worker has an superior that is not this object: ", boss->getAlias().c_str());
		return 0;
	}
	
	return worker;
}

bool HGESuperior::assignAssistant(HGEWorker * helper) {
	if (this->assistant) {
		if (this->assistant == helper) {
			return !0;
		}
		HGEAssertC(0, "this superior already has an assistant");
		return 0;
	}
	
	if (helper == 0) {
		this->assistant = 0;
		return !0;
	} else if (this->gainWorker(helper)) {
		this->assistant = helper;
		return !0;
	}
	
	HGEAssertC(0, "failed to hire assistant");
	return false;
}

NS_HGE_END
