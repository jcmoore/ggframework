//
//  HGERef.h
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEREF_H__
#define __HGEREF_H__

#include "core/HGEModel.h"

NS_HGE_BEGIN

/**
 base class of all reference templates
 subclass HGEModel because each reference itself is versioned
 its version can be compared to to the contained object's version
 to determine whether or not the reference is "out of date"
 */
class HGERefKind : public HGEModel {
	HGEClassifyBaseKind(HGERefKind); // TODO: FIX THIS !!! .. or not (maybe don't need different 'kinds' for each HGERef<Type>
public:
	HGERefKind() : HGEModel() {}
	virtual ~HGERefKind() {}
	
	virtual kind_hge refkind() = 0;
	virtual bool refkindof(kind_hge k) = 0;
};

class HGEEntity;

template <typename HGEPointed = HGEEntity>
class HGERef : public HGERefKind {
public:
	/**
	 get the kind uuid of the contained object (if one exists)
	 */
	virtual kind_hge refkind() { return this->obj ? this->obj->hgekind() : HGEKindNone(); }
	
	/**
	 test against the kind of the contained object (if one exists)
	 */
	virtual bool refkindof(kind_hge k) { return this->obj ? this->obj->hgekindof(k) : k == HGEKindNone(); }
	
private:
	
	// TODO: find a more elegant way to enforce this constraint
	HGEModel * proof () { return obj; } // template type must be subclass of HGEModel
	
public:
	HGERef() : HGERefKind(), obj(0) {}
	virtual ~HGERef() {}
	
	/**
	 true if the reference contains no object
	 */
	bool unset () { return this->obj == 0; }
	/**
	 true if the reference's version does not match the contained object's version
	 */
	bool dirty () { return (!this->unset() && this->proof()->hgeversion() != this->hgeversion()); }
	
	/**
	 matches the reference's version to the contained object's version --
	 typically called after the referrer as accounted for changes in the referree
	 */
	version_hge clean () {
		return (hgerevision() = (this->unset() ? HGE_VERSION_ZERO() : this->proof()->hgeversion()));
	}
	/**
	 refer to the indicated value
	 */
	version_hge set (HGEPointed * value) {
		obj = value;
		return clean();
	}
	
	/**
	 return the contained object pointer
	 */
	HGEPointed * arrow() { return obj; }
	
	/**
	 return the contained object reference -- be sure the reference is not unset!
	 */
	HGEPointed& dot() { return *obj; }
private:
	HGEPointed * obj;
};

NS_HGE_END

#endif
