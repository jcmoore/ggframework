//
//  HGECanTask.h
//  hybridge
//
//  Created by The Narrator on 11/9/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANTASK_H__
#define __HGECANTASK_H__

#include "core/can/HGECanJott.h"
#include "core/can/HGECanIdentify.h"



NS_HGE_BEGIN



template < typename Derived = HGENone, typename Parent = Derived >
class HGECanTask;


template <>
class HGECanTask<> {
public:
	
	struct Magic : public HGECanJott<>::Magic {
		virtual bool task(JSONValue& json, bool purify = 0) = 0;
	};
};

template < typename Derived, typename Parent >
class HGECanTask : public Parent {
public:
	
	typedef HGECanTask MagicTasker;
	typedef HGECanTask MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanTask RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanTask<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
		
		virtual bool jott(JSONValue& json, bool purify = 0) {
			return this->that->task(json, purify);
		}
		
		virtual bool task(JSONValue& json, bool purify = 0) {
			return this->that->task(json, purify);
		}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
		if (HGE_LIKEA( hybridge::HGECanTask ) == interface) {
			if (result) {
				*result = imp_cast<hybridge::HGECanTask<>::Magic *>(this->feat());
			}
			return !0;
		} else if (HGE_LIKEA( hybridge::HGECanJott ) == interface) {
			if (result) {
				*result = imp_cast<hybridge::HGECanJott<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result, compositExclusion);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanTask()
	: Parent()
	, magic(imp_cast<RealSelf *>(this))
	, jotter(0)
	{}
	
	template <typename Delegate>
	HGECanTask(Delegate * delegate = 0)
	: Parent(delegate)
	, magic(imp_cast<RealSelf *>(this))
	, jotter(0)
	{}
	
private:
	
	Magic magic;
	
public:
	
	typedef HGECanJott<>::Magic Jotter;
	typedef HGECanIdentify<>::Magic Identifier;
	
	bool draft(JSONValue& json) {
		if (!json.IsObject()) {
			HGEAssertC(0, "tasker can only task JSON objects but was given JSON type: %i", json.GetType());
			return 0;
		}
		
		json.AddMember(HGE_KEYTEXT_BOTTOM_LEVEL_DOMAIN_NAME, this->identifier->getDomainName());
		json.AddMember(HGE_KEYTEXT_PORT_NUMBER, this->identifier->getPortNumber());
		return this->jotter->jott(json, 0);
	}
	
	virtual bool task(JSONValue& json, bool purify = 0) {
		if (purify) {
			JSONValue dupe;
			JSONDoc doc;
			doc.Reproduce(json, dupe);
			return this->draft(dupe);
		} else {
			return this->draft(json);
		}
	}

	HGECanTask(Jotter * j, Identifier * i)
	: Parent()
	, magic(imp_cast<RealSelf *>(this))
	, jotter(j)
	, identifier(i)
	{}
	
	template <typename Delegate>
	HGECanTask(Jotter * j, Identifier * i, Delegate * delegate = 0)
	: Parent(delegate)
	, magic(imp_cast<RealSelf *>(this))
	, jotter(j)
	, identifier(i)
	{}
	
private:
	
	Jotter * jotter;
	Identifier * identifier;
	
};



NS_HGE_END

#endif
