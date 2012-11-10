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

#define imp_cast		static_cast

template < typename Derived = HGENone, typename Parent = HGENone >
class HGECanImp;

template <>
class HGECanImp<> {
public:
	
	template < typename MagicDerived = void, typename DeepMagic = void >
	struct Magic;
	
};

// type-interface
template <>
struct HGECanImp<>::Magic< void, void > {
	
private:
	
	virtual bool how(HGECanImp<>::Magic<> * result) = 0;
	
public:
	
	virtual bool canDo(like_hge interface, HGECanImp<>::Magic<> ** result) = 0;
	
	template < typename Kind >
	Magic * canDo() {
		Magic * result = 0;
		like_hge interface = HGE_LIKEAN_UNSAFE( Kind );
		if (this->canDo(interface, &result)) {
			return result;
		} else {
			return 0;
		}
	}
	
	template < typename Destination >
	Destination * canTo() {
		typedef typename HGECanImp<>::Magic< Destination > Answer;
		
		Answer a;
		
		if (this->how(&a)) {
			Destination * concrete = 0;
			if (a.with(HGE_KINDOF(Destination), &concrete)) {
				return concrete;
			}
		}
		return 0;
	}
};

// typed-interface
template < typename MagicDerived >
struct HGECanImp<>::Magic< MagicDerived > : public HGECanImp<>::Magic<> {
	
	friend class HGECanImp<>::Magic<>;
	
	template < typename Derived, typename Base >
	friend class HGECanImp<>::Magic;
	
private:
	
	virtual bool how(HGECanImp<>::Magic<> * result) {
		if (result) {
			static_cast< HGECanImp<>::Magic<MagicDerived> * >(result)->that = this->that;
		}
		return !0;
	}
	
public:
	
	virtual bool canDo(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanImp<>::Magic<> *>(this);
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	virtual bool with(kind_hge concrete, MagicDerived ** result) {
		return this->that ? this->that->areYou(concrete, result) : 0;
	};
	
	Magic() : that(0) {}
	
//private:
	MagicDerived * that;
};

template < typename MagicDerived, typename BaseMagic >
struct HGECanImp<>::Magic : public BaseMagic {
	
	friend class HGECanImp<>::Magic<>;
	
private:
	
	virtual bool how(HGECanImp<>::Magic<> * result) {
		if (result) {
			static_cast< HGECanImp<>::Magic<MagicDerived> * >(result)->that = this->that;
		}
		return !0;
	};
	
public:
	
	virtual bool canDo(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanImp<>::Magic<> *>(this);
			}
			return !0;
		} else {
			return this->that->canYou(interface, result);
		}
	}
	
	//virtual bool with(kind_hge concrete, MagicDerived ** result) {
	//	return this->that->areYou(concrete, result);
	//}
	
	Magic(MagicDerived * t) : that(t) {
		HGEAssertC(this->that, "cannot do magic without 'that'");
	}

protected:
	MagicDerived * that;
};




// void implementation
template < typename Derived >
class HGECanImp< Derived, HGENone> {
public:
	typedef HGECanImp MagicBlack;
	typedef HGECanImp MagicParent;
	typedef Derived MagicDerived;
	typedef HGENone RealParent;
	typedef HGECanImp RealSelf;
	
	template <typename HiddenMagic>
	struct DeepMagic : public HiddenMagic {
		virtual bool with(kind_hge concrete, MagicDerived ** result) = 0;
	};
	
	typedef DeepMagic< HGECanImp<>::Magic<> > OtherMagic;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanImp<>::Magic<> > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
		
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = imp_cast<hybridge::HGECanImp<>::Magic<> *>(this->feat());
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	virtual bool areYou(kind_hge concrete, MagicDerived ** result) {
		if (HGE_KINDOF( MagicDerived ) == concrete) {
			if (result) {
				*result = imp_cast<MagicDerived *>(this);
			}
			return !0;
		} else {
			return 0;
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanImp()
	: magic(static_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
};

template < typename Derived, typename Parent >
class HGECanImp : public Parent {
public:
	typedef HGECanImp MagicBlack;
	typedef HGECanImp MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanImp RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanImp<>::Magic<> > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = imp_cast<hybridge::HGECanImp<>::Magic<> *>(this->feat());
			}
			return !0;
		} else {
			return RealParent::canYou(interface, result);
		}
	}
	
	virtual bool areYou(kind_hge concrete, MagicDerived ** result) {
		if (HGE_KINDOF( MagicDerived ) == concrete) {
			if (result) {
				*result = imp_cast<MagicDerived *>(this);
			}
			return !0;
		} else {
			typedef typename RealParent::MagicBlack::MagicDerived MagicConverter;
			MagicConverter * converter = 0;
			if (RealParent::areYou(concrete, (result ? &converter : 0))) {
				if (result) {
					*result = imp_cast< MagicDerived * >(this);
				}
				return !0;
			} else {
				return 0;
			}
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanImp()
	: magic(imp_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
};


NS_HGE_END

#endif
