//
//  HGECanJott.h
//  hybridge
//
//  Created by The Narrator on 11/8/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANJOTT_H__
#define __HGECANJOTT_H__

#include "core/can/HGECanImp.h"
#include "service/HGEWorker.h"



NS_HGE_BEGIN



template < class Derived = HGENone, typename Parent = Derived >
class HGECanJott;


template <>
class HGECanJott<> {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
		virtual bool jott(JSONValue& json, bool purify = 0) = 0;
	};
};

template < class Derived, typename Parent >
class HGECanJott : public Parent {
public:
	
	typedef HGECanJott MagicJotter;
	typedef HGECanJott MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanJott RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanJott<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
		
		virtual bool jott(JSONValue& json, bool purify = 0) {
			return this->that->jott(json, purify);
		}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanJott ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanJott<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanJott()
	: magic(imp_cast<RealSelf *>(this))
	, publisher(0)
	{}
	
private:
	
	Magic magic;
	
public:
	
	typedef HGEWorker Publisher;
	
	/**
	 send json to a worker
	 */
	virtual bool jott(JSONValue& json, bool purify = 0) {
		return this->publisher->produceJSON(json, purify);
	}
	
	HGECanJott(Publisher * p)
	: magic(imp_cast<RealSelf *>(this))
	, publisher(p)
	{}
	
private:
	
	Publisher * publisher;
	
};



NS_HGE_END

#endif
