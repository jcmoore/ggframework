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



template < typename Derived = HGENone, typename Parent = Derived >
class HGECanJott;


template <>
class HGECanJott<> {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
	};
};

template < typename Derived, typename Parent >
class HGECanJott : public Parent {
public:
	
	typedef HGECanJott MagicJotter;
	typedef HGECanJott MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	
private:
	typedef HGECanImp<>::Magic< MagicDerived, HGECanJott<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(MagicDerived * d) : Trick(d) {}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanJott ) == interface) {
			if (result) {
				*result = this->feat();
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanJott() : magic(static_cast<MagicDerived *>(this)) {}
	
private:
	
	Magic magic;
	
public:
	
	typedef HGEWorker Producer;
	
	/**
	 send json to a worker
	 */
	bool produce(JSONValue& json, bool purify = 0) {
		return this->producer->produceJSON(json, purify);
	}
	
protected:
	
	Producer * producer;
	
};



NS_HGE_END

#endif
