//
//  HGEKind.h
//  hybridge
//
//  Created by The Narrator on 8/25/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEKIND_H__
#define __HGEKIND_H__

#include "core/HGEID.h"
#include "dev/HGEMacros.h"

#define HGE_KINDOF(t)			HGEKind<t>()

//// add kind component -- base-kind
//#define HGEClassifyBaseKind(kind)		\
//public: \
//virtual kind_hge hgekind() { return HGEKind<kind>(); } \
//virtual bool hgekindof(kind_hge k) { return k == this->hgekind(); }

// add kind component -- sub-kind
//#define HGEClassifyKind(subkind, superkind)		\
//public: \
//virtual kind_hge hgekind() { return HGEKind<subkind>(); } \
//virtual bool hgekindof(kind_hge k) { return k == subkind::hgekind() || superkind::hgekindof(k); }

NS_HGE_BEGIN

typedef void HGENone;

// type for hybrid kind uuids
typedef id_hge kind_hge;

// WARNING: currently the numeric value of a kind is execution order dependent!  (NEED TO ADD SERIALIZATION)

/**
 specialized class to handle generation of kind uuids
 */
struct HGEKindId {
private:
	HGEKindId() {};
	
	template <typename HGEClassType>
	friend class HGEKind;
	
	static kind_hge uniqueKind() {
		static kind_hge instance = 0;
		return ++instance;
	}
};

/**
 template class that holds the kind uuid for a specific class
 */
template <typename HGEClassType>
struct HGEKind {
	
	HGEKind() {};
	
	operator kind_hge () {
		static kind_hge instance = HGEKindId::uniqueKind();
		
		return instance;
	}
};

/**
 specialized class used to express the kind uuid for undefined kinds
 */
template<>
struct HGEKind< HGENone > {
	
	HGEKind() {};
	
	operator kind_hge () { return 0; }
};

typedef HGEKind< HGENone >	HGEKindNone;

NS_HGE_END

#endif
