//
//  HGECanImp.h
//  hybridge
//
//  Created by The Narrator on 11/8/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANIMP_H__
#define __HGECANIMP_H__

#include "dev/HGEMacros.h"
#include "dev/HGEPlatformMacros.h"
#include "core/HGEImplements.h"

#include <limits.h>

#include "service/HGEJSON.h"

NS_HGE_BEGIN



template < typename Parent = HGENone >
class HGECanImp;


template <>
class HGECanImp< HGENone > {
public:
	
	template < typename MagicDerived = void, typename DeepMagic = void >
	struct Magic;
};

// type-interface
template <>
struct HGECanImp<>::Magic< void, void > {
	virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) = 0;
	
	template < typename Kind >
	Magic * does() {
		Magic * result = 0;
		like_hge interface = HGE_LIKEAN_UNSAFE( Kind );
		if (this->does(interface, &result)) {
			return result;
		} else {
			return 0;
		}
	}
	
	template < typename Destination, typename Intermediate >
	Destination * with() {
		typedef typename Intermediate::MagicBlack::Magic Informer;
		Intermediate * concrete = 0;
		Magic * other = does< HGECanImp<> >();
		if (other) {
			Informer * informer = static_cast< Informer * >(other);
			if (informer->with(HGE_KINDOF(Destination), &concrete)) {
				return dynamic_cast< Destination * >( concrete );
			}
		}
		return 0;
	}
};

// typed-interface
template < typename MagicDerived >
struct HGECanImp<>::Magic< MagicDerived > : public HGECanImp<>::Magic<> {
	virtual bool with(kind_hge concrete, MagicDerived ** result) = 0;
};

template < typename MagicDerived, typename DeepMagic >
struct HGECanImp<>::Magic : public DeepMagic {
};


template < typename Parent >
class HGECanImp : public Parent {
public:
	typedef Parent RealParent;
	typedef typename Parent::MagicConcrete MagicConcrete;
	typedef typename Parent::MagicDerived MagicDerived;
	typedef HGECanImp MagicBlack;
	typedef HGECanImp MagicParent;
	
	struct Magic : public HGECanImp<>::Magic< MagicDerived > {
		
		virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) {
			return this->that->does(interface, result);
		}
		
		virtual bool with(kind_hge concrete, MagicDerived ** result) {
			return this->that->is(concrete, result);
		}
		
		Magic(MagicBlack * t) : that(t) {
			HGEAssertC(this->that, "cannot do magic without 'that'");
		}
	private:
		MagicBlack * that;
	};
	
	virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = this->trick();
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	virtual bool is(kind_hge concrete, MagicDerived ** result) {
		if (HGE_KINDOF( MagicConcrete ) == concrete) {
			if (result) {
				*result = static_cast< MagicDerived * >(this);
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	Magic * trick() { return &magic; }
	
	HGECanImp() : magic(this) {}
	
private:
	
	Magic magic;
	
};

NS_HGE_END

#endif
