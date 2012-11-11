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
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanRout RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanRout<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		
		Magic(RealSelf * d) : Trick(d) {};
		
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
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result, HGECantImp * compositExclusion) {
		if (HGE_LIKEA( hybridge::HGECanRout ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanRout<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result, compositExclusion);
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
	
	HGECanRout()
	: Parent()
	, magic(imp_cast<RealSelf *>(this))
	{}
	
	template <typename Delegate>
	HGECanRout(Delegate * delegate = 0)
	: Parent(delegate)
	, magic(imp_cast<RealSelf *>(this))
	{}
	
private:
	
	Magic magic;
	
};

NS_HGE_END

#endif
