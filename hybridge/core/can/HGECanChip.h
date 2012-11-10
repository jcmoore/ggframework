//
//  HGECanChip.h
//  hybridge
//
//  Created by The Narrator on 11/8/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANCHIP_H__
#define __HGECANCHIP_H__

#include "core/can/HGECanImp.h"



NS_HGE_BEGIN



template < typename Derived = HGENone, typename Parent = Derived >
class HGECanChip;

template < typename Parent >
class HGECanChip< Parent, Parent >;


template <>
class HGECanChip<> {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
	};
};

template < typename Derived, typename Parent >
class HGECanChip : public Parent {
public:
	
	typedef HGECanChip MagicChip;
	typedef HGECanChip MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanChip RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanChip<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanChip ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanChip<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanChip()
	: magic(static_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
public:
	
	typedef void * Condition;
	typedef bool (MagicDerived::*Matcher)(Condition condition, MagicDerived ** result);
	
protected:
	
	bool known (Matcher matcher, Condition condition, MagicDerived ** result = 0) {
		return (static_cast<MagicDerived *>(this)->*matcher)(condition, result);
	}
	
};



NS_HGE_END

#endif
