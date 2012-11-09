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
	
	typedef HGECanConnect MagicOnline;
	typedef HGECanConnect MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	
private:
	typedef HGECanImp<>::Magic< MagicDerived, HGECanConnect<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(MagicDerived * d) : Trick(d) {}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanConnect ) == interface) {
			if (result) {
				*result = this->feat();
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanConnect() : magic(static_cast<MagicDerived *>(this)),  bdns(0) {}
	
private:
	
	Magic magic;
	
public:
	
	typedef HGERouter::BottomLevelNameServerInterface NameServer;
	
	virtual void call () {}
	
protected:
	
	NameServer * bdns;
	
private:
	
};



NS_HGE_END

#endif
