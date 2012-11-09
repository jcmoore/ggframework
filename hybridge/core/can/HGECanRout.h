//
//  HGECanRout.h
//  hybridge
//
//  Created by The Narrator on 11/8/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANROUT_H__
#define __HGECANROUT_H__

#include "core/can/HGECanImp.h"

#include "service/HGEJSON.h"

NS_HGE_BEGIN


template < typename Derived = HGENone, typename Parent = Derived >
class HGECanRout;

template <>
class HGECanRout<> {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
		/**
		 using JSON as input, destroy the entity
		 */
		virtual bool destroyJSON(JSONValue& json, bool firstResponder) = 0;
		
		/**
		 using JSON as input, create the entity
		 */
		virtual bool createJSON(JSONValue& json, bool firstResponder) = 0;
		
		/**
		 using JSON as input, take some action
		 */
		virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder) = 0;
	};
};

template < typename Derived, typename Parent >
class HGECanRout : public Parent {
public:
	
	typedef HGECanRout MagicWhite;
	typedef HGECanRout MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	
private:
	typedef HGECanImp<>::Magic< MagicDerived, HGECanRout<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		
		Magic(MagicDerived * d) : Trick(d) {};
		
		/**
		 using JSON as input, destroy the entity
		 */
		virtual bool destroyJSON(JSONValue& json, bool firstResponder) {
			return this->that->destroyJSON(json, firstResponder);
		}
		
		/**
		 using JSON as input, create the entity
		 */
		virtual bool createJSON(JSONValue& json, bool firstResponder) {
			return this->that->createJSON(json, firstResponder);
		}
		
		/**
		 using JSON as input, take some action
		 */
		virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder) {
			return this->that->enactJSON(task, json, firstResponder);
		}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanRout ) == interface) {
			if (result) {
				*result = this->feat();
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	/**
	 using JSON as input, destroy the entity
	 */
	virtual bool destroyJSON(JSONValue& json, bool firstResponder) = 0;
	
	/**
	 using JSON as input, create the entity
	 */
	virtual bool createJSON(JSONValue& json, bool firstResponder) = 0;
	
	/**
	 using JSON as input, take some action
	 */
	virtual bool enactJSON(JSONValue& task, JSONValue& json, bool firstResponder) = 0;
	
	Magic * feat() { return &magic; }
	
	HGECanRout() : magic(static_cast<MagicDerived *>(this)) {}
	
private:
	
	Magic magic;
	
};

NS_HGE_END

#endif