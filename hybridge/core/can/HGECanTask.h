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
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
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
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanTask()
	: magic(imp_cast<RealSelf *>(this))
	, jotter(0)
	{}
	
private:
	
	Magic magic;
	
public:
	
	typedef HGECanJott<>::Magic Jotter;
	typedef HGECanIdentify<>::Magic Identifier;
	
	virtual bool task(JSONValue& json, bool purify = 0) {
		return this->jotter->jott(json, purify);
	}
	
	HGECanTask(Jotter * j, Identifier * i)
	: magic(imp_cast<RealSelf *>(this))
	, jotter(j)
	, identifier(i)
	{}
	
private:
	
	Jotter * jotter;
	Identifier * identifier;
	
};



NS_HGE_END

#endif
