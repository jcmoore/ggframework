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

typedef void HGECantImp;

template < typename Derived = HGENone, typename Parent = HGENone >
class HGECanImp;

template <>
class HGECanImp<> {
public:
	
	template < typename MagicDerived = void, typename MagicBase = void >
	struct Magic;
	
};



template <>
struct HGECanImp<>::Magic< void, void > {
	
private:
	
	virtual HGECanImp<>::Magic<> * passTo(HGECanImp<>::Magic<> * result) = 0;
	
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
		typedef typename HGECanImp<>::Magic< Destination > Messenger;
		
		Messenger courier;
		
		HGECanImp<>::Magic<> * delegator = this;
		
		while ((delegator = delegator->passTo(&courier))) {
		}
		
		Destination * concrete = 0;
		if (courier.sendTo(HGE_KINDOF(Destination), &concrete)) {
			return concrete;
		}
		return 0;
	}
};

// messanger interface
template < typename MagicDerived >
struct HGECanImp<>::Magic< MagicDerived > : public HGECanImp<>::Magic<> {
	
	friend class HGECanImp<>::Magic<>;
	
	template <typename Derived, typename Base>
	friend class HGECanImp<>::Magic;
	
private:
	
	HGECanImp<>::Magic<> * passTo(HGECanImp<>::Magic<> * result) {
		return 0;
	}
	
	bool sendTo(kind_hge concrete, MagicDerived ** result) {
		return this->that ? this->that->areYou(concrete, result) : 0;
	};
	
public:
	
	bool canDo(like_hge interface, HGECanImp<>::Magic<> ** result) {
		return 0;
	}
	
private:
	Magic() : that(0) {}
	
	MagicDerived * that;
};

template < typename MagicDerived, typename MagicBase >
struct HGECanImp<>::Magic : public MagicBase {
	
	friend class HGECanImp<>::Magic<>;
	
private:
	
	HGECanImp<>::Magic<> * passTo(HGECanImp<>::Magic<> * result) {
		if (result) {
			HGECanImp<>::Magic<> * delegateMagic = this->that->delegator();
			if (delegateMagic &&
				this != delegateMagic) {
				return delegateMagic;
			} else {
				static_cast< HGECanImp<>::Magic<MagicDerived> * >(result)->that = this->that;
				return 0;
			}
		} else {
			return 0;
		}
	};
	
public:
	
	virtual bool canDo(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanImp<>::Magic<> *>(this);
			}
			return !0;
		} else {
			return this->that->canYou(interface, result, 0);
		}
	}
	
	Magic(MagicDerived * t) : that(t) {
		HGEAssertC(this->that, "cannot do magic without 'that'");
	}

protected:
	MagicDerived * that;
};




// delegate forwarding implementation
template < typename Derived >
class HGECanImp< Derived, HGENone> {
public:
	typedef HGECanImp MagicImp;
	typedef HGECanImp MagicParent;
	typedef Derived MagicDerived;
	typedef HGENone RealParent;
	typedef HGECanImp RealSelf;
	typedef Derived FakeSelf;
	
private:
	typedef HGECanImp<>::Magic< FakeSelf, HGECanImp<>::Magic<> > Trick;
public:
	
	struct Magic : public Trick {
		Magic(FakeSelf * d) : Trick(d) {}
		
	private:
		
		friend class HGECanImp;
		
		bool canWe(like_hge interface,
				   HGECanImp<>::Magic<> ** result,
				   HGECantImp * compositExclusion,
				   HGECanImp * inquirer) {
			if ((HGECantImp *)this->that != compositExclusion) {
				return this->that->canYou(interface, result, (HGECantImp *)inquirer);
			} else {
				return 0;
			}
		}
		
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
		return this->feat()->canWe(interface, result, compositExclusion, this);
	}
	
//private:
	
	virtual bool areYou(kind_hge concrete, MagicDerived ** result) {
		if (result) {
			*result = this->feat()->template canTo<MagicDerived>();
			return !0;
		} else {
			return 0;
		}
	}
	
public:
	
	Magic * feat() { return &magic; }
	
	HGECanImp(MagicDerived * delegate)
	: magic(static_cast<FakeSelf *>(delegate))
	{}
	
private:
	
	Magic magic;
	
public:
	
	Magic * delegator() { return this->feat(); }
	
};

// superclass implementation
template < typename Derived >
class HGECanImp< Derived, Derived> {
public:
	typedef HGECanImp MagicImp;
	typedef HGECanImp MagicParent;
	typedef Derived MagicDerived;
	typedef HGENone RealParent;
	typedef HGECanImp RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanImp<>::Magic<> > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
		
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
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
	
	HGECanImp(MagicDerived * delegate = 0)
	: magic(static_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
public:
	
	Magic * delegator() { return 0; }
	
};



// subclass implementation
template < typename Derived, typename Parent >
class HGECanImp : public Parent {
public:
	typedef HGECanImp MagicImp;
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
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
		if (HGE_LIKEA( hybridge::HGECanImp ) == interface) {
			if (result) {
				*result = imp_cast<hybridge::HGECanImp<>::Magic<> *>(this->feat());
			}
			return !0;
		} else {
			return RealParent::canYou(interface, result, compositExclusion);
		}
	}
	
	virtual bool areYou(kind_hge concrete, MagicDerived ** result) {
		if (HGE_KINDOF( MagicDerived ) == concrete) {
			if (result) {
				*result = imp_cast<MagicDerived *>(this);
			}
			return !0;
		} else {
			typedef typename RealParent::MagicImp::MagicDerived MagicConverter;
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
	
	HGECanImp(MagicDerived * delegate = 0)
	: magic(imp_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
public:
	
	Magic * delegator() { return 0; }
	
};


NS_HGE_END

#endif
