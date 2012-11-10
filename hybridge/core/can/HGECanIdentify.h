//
//  HGECanIdentify.h
//  hybridge
//
//  Created by The Narrator on 11/9/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANIDENTIFY_H__
#define __HGECANIDENTIFY_H__

#include "core/can/HGECanImp.h"
#include "service/HGERouter.h"



NS_HGE_BEGIN



template < typename Derived = HGENone, typename Parent = Derived >
class HGECanIdentify;


template <>
class HGECanIdentify<> {
public:
	typedef HGEPortNumber PortNumber;
	typedef HGEBottomLevelDomainName DomainName;
	
	struct Magic : public HGECanImp<>::Magic<> {
		virtual PortNumber getPortNumber() = 0;
		virtual DomainName getDomainName() = 0;
	};
};

template < typename Derived, typename Parent >
class HGECanIdentify : public Parent {
public:
	
	typedef HGECanIdentify MagicIdentity;
	typedef HGECanIdentify MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanIdentify RealSelf;
	
	typedef HGECanIdentify<>::PortNumber PortNumber;
	typedef HGECanIdentify<>::DomainName DomainName;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanIdentify<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
		
		virtual PortNumber getPortNumber() {
			return this->that->getPortNumber();
		}
		virtual DomainName getDomainName() {
			return this->that->getDomainName();
		}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanIdentify ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanIdentify<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanIdentify()
	: magic(imp_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
public:
	
	virtual PortNumber getPortNumber() {
		return this->portNumber;
	}
	virtual DomainName getDomainName() {
		return this->domainName;
	}
	
	HGECanIdentify(DomainName dn, PortNumber pn)
	: magic(imp_cast<RealSelf *>(this))
	, domainName(dn)
	, portNumber(pn)
	{}
	
private:
	
	DomainName domainName;
	PortNumber portNumber;
	
};



NS_HGE_END

#endif
