//
//  HGECanConnect.h
//  hybridge
//
//  Created by The Narrator on 11/8/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANCONNECT_H__
#define __HGECANCONNECT_H__

#include "core/can/HGECanImp.h"
#include "service/HGERouter.h"



NS_HGE_BEGIN
template < typename Derived = HGENone, typename Parent = Derived >
class HGECanConnect;


template <>
class HGECanConnect<> {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
	};
};

template < typename Derived, typename Parent >
class HGECanConnect : public Parent {
public:
	
	typedef HGECanConnect MagicConnector;
	typedef HGECanConnect MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanConnect RealSelf;
	
	typedef HGERouter::BottomLevelNameServerInterface NameServer;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanConnect<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
		if (HGE_LIKEA( hybridge::HGECanConnect ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanConnect<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result, compositExclusion);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanConnect(NameServer * ns = 0)
	: Parent()
	, magic(static_cast<RealSelf *>(this))
	, bdns(ns)
	{}
	
	template <typename Delegate>
	HGECanConnect(NameServer * ns = 0, Delegate * delegate = 0)
	: Parent(delegate)
	, magic(static_cast<RealSelf *>(this))
	, bdns(ns)
	{}
	
private:
	
	Magic magic;
	
public:
	
	virtual HGEHandler * whois(HGEBottomLevelDomainName bldn, HGEPortNumber port) {
		return this->bdns->whois(bldn, port);
	}
	
protected:
	
	NameServer * bdns;
	
private:
	
};



NS_HGE_END

#endif
