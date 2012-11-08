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



template < typename Parent = HGENone >
class HGECanChip;


template <>
class HGECanChip< HGENone > {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
	};
};

template < typename Parent >
class HGECanChip : public Parent {
public:
	
	typedef Parent RealParent;
	typedef typename Parent::MagicConcrete MagicConcrete;
	typedef typename Parent::MagicDerived MagicDerived;
	typedef HGECanChip MagicChip;
	typedef HGECanChip MagicParent;
	
	struct Magic : public HGECanImp<>::Magic< MagicDerived, HGECanChip<>::Magic > {
		
		virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) {
			return this->that->does(interface, result);
		}
		
		virtual bool with(kind_hge concrete, MagicDerived ** result) {
			return this->that->is(concrete, result);
		}
		
		Magic(MagicChip * t) : that(t) {
			HGEAssertC(this->that, "cannot do magic without 'that'");
		}
	private:
		MagicChip * that;
	};
	
	virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanChip ) == interface) {
			if (result) {
				*result = this->trick();
			}
			return !0;
		} else {
			return this->RealParent::does(interface, result);
		}
	}
	
	Magic * trick() { return &magic; }
	
	HGECanChip() : magic(this) {}
	
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
