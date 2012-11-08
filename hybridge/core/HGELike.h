//
//  HGELike.h
//  hybridge
//
//  Created by The Narrator on 11/7/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGELIKE_H__
#define __HGELIKE_H__

#include "core/HGEKind.h"

#define HGE_LIKEA(i)			HGELike< i<> >()
#define HGE_LIKEAN_UNSAFE(i)	HGELike< i >()

NS_HGE_BEGIN

// type for hybrid like uuids
typedef kind_hge like_hge;

// WARNING: currently the numeric value of a kind is execution order dependent!  (NEED TO ADD SERIALIZATION)

/**
 specialized class to handle generation of kind uuids
 */
struct HGELikeId {
private:
	HGELikeId() {};
	
	template < typename HGEDefaultTemplateType >
	friend class HGELike;
	
	static like_hge uniqueLike() {
		static like_hge instance = 0;
		return ++instance;
	}
};

/**
 template class that holds the kind uuid for a specific class
 */
template < typename HGETemplateType >
struct HGELike {
	
	HGELike() {};
	
	operator like_hge () {
		static like_hge instance = HGELikeId::uniqueLike();
		
		return instance;
	}
};

template <typename Kind = HGENone>
class HGENothing {
};

/**
 specialized class used to express the kind uuid for undefined kinds
 */
template<>
struct HGELike< HGENothing<> > {
	
	HGELike() {};
	
	operator kind_hge () { return 0; }
};

typedef HGELike< HGENothing<> >	HGELikeNothing;

NS_HGE_END

#endif
