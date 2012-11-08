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


template < typename Parent = HGENone >
class HGECanRout;


template <>
class HGECanRout< HGENone > {
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

template < typename Parent >
class HGECanRout : public Parent {
public:
	
	typedef Parent RealParent;
	typedef typename Parent::MagicConcrete MagicConcrete;
	typedef typename Parent::MagicDerived MagicDerived;
	typedef HGECanRout MagicWhite;
	typedef HGECanRout MagicParent;
	
	struct Magic : public HGECanImp<>::Magic< MagicDerived, HGECanRout<>::Magic > {
		
		virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) {
			return this->that->does(interface, result);
		}
		
		virtual bool with(kind_hge concrete, MagicDerived ** result) {
			return this->that->is(concrete, result);
		}
		
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
		
		Magic(MagicWhite * t) : that(t) {}
		
	private:
		MagicWhite * that;
	};
	
	virtual bool does(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanRout ) == interface) {
			if (result) {
				*result = this->trick();
			}
			return !0;
		} else {
			return this->RealParent::does(interface, result);
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
	
	Magic * trick() { return &magic; }
	
	HGECanRout() : magic(this) {}
	
private:
	
	Magic magic;
	
};

NS_HGE_END

#endif
