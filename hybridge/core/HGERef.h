//
//  RefHGE.h
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


class HGEHandle {
public:
	HGEHandle() : obj(0) {}
	
	/**
	 true if the reference contains no object
	 */
	bool unset () { return this->obj == 0; }
	/**
	 refer to the indicated value
	 */
	void set (HGEEntity * value) {
		this->obj = value;
	}
	
	/**
	 return the contained object pointer if it can be coerced into the specified type
	 */
	template < typename T >
	T * coerce() {
		HGEEntity * entity = this->arrow();
		return (entity ? entity->asKind<T>() : 0);
	}
	
	/**
	 return the contained object pointer
	 */
	HGEEntity * arrow() { return this->obj; }
	
	/**
	 return the contained object reference -- be sure the reference is not unset!
	 */
	HGEEntity& dot() { return *this->obj; }
private:
	HGEEntity * obj;
};



class HGERef {
	
	HGE_VERSIONED_WILL_INITIALIZE(ver);
	
public:
	HGERef() : ver(HGE_VERSION_NONE()), handle() {}
	
	/**
	 true if the reference contains no object
	 */
	bool unset () { return this->handle.unset() == 0; }
	/**
	 true if the reference's version does not match the contained object's version
	 */
	bool dirty () { return (!this->cache ? !this->unset() : (this->vnumber() != this->cache->vnumber())); }
	
	/**
	 matches the reference's version to the contained object's version --
	 typically called after the referrer as accounted for changes in the referree
	 */
	version_hge clean () {
		return (this->revision() = (this->cache ? this->cache->vnumber() : HGE_VERSION_NONE()));
	}
	/**
	 refer to the indicated value
	 */
	version_hge set (HGEEntity * value) {
		this->handle.set(value);
		
		this->cache = 0;
		if (value) {
			HGEEntity * model = 0;
			if (value->knownKind(HGE_KINDOF( HGEModel ), &model)) {
				this->cache = (HGEModel *)model;
			}
		}
		return this->clean();
	}
	
	/**
	 return the contained object pointer if it can be coerced into the specified type
	 */
	template < typename T >
	T * coerce() {
		return this->handle.coerce<T>();
	}
	
	/**
	 return the contained object pointer
	 */
	HGEEntity * arrow() { return this->handle.arrow(); }
	
	/**
	 return the contained object reference -- be sure the reference is not unset!
	 */
	HGEEntity& dot() { return this->handle.dot(); }
	
private:
	HGEHandle handle;
	HGEModel * cache;
};

class HGEUmp : public HGEModel {
	
protected:
	
	virtual bool beKind (MagicChip::Condition condition, MagicChip::MagicDerived ** result) {
		if (kind_hge(condition) == HGE_KINDOF( HGEUmp ) ||
			HGEEntity::beKind(condition, result)) { // act like an entity but not a model (even though it is a model...)
			if (result) {
				*result = this;
			}
			return !0;
		} else {
			return 0;
		}
	}
	
public:
	HGEUmp() : HGEModel() {}
	virtual ~HGEUmp() {}
	
	/**
	 true if the reference contains no object
	 */
	bool unset () { return this->handle.unset() == 0; }
	/**
	 true if the reference's version does not match the contained object's version
	 */
	bool dirty () { return (!this->cache ? !this->unset() : (this->vnumber() != this->cache->vnumber())); }
	
	/**
	 matches the reference's version to the contained object's version --
	 typically called after the referrer as accounted for changes in the referree
	 */
	version_hge clean () {
		return (this->revision() = (this->cache ? this->cache->vnumber() : HGE_VERSION_NONE()));
	}
	/**
	 refer to the indicated value
	 */
	version_hge set (HGEEntity * value) {
		this->handle.set(value);
		
		this->cache = 0;
		if (value) {
			HGEEntity * model = 0;
			if (value->knownKind(HGE_KINDOF( HGEModel ), &model)) {
				this->cache = (HGEModel *)model;
			}
		}
		return this->clean();
	}
	
	/**
	 return the contained object pointer if it can be coerced into the specified type
	 */
	template < typename T >
	T * coerce() {
		return this->handle.coerce<T>();
	}
	
	/**
	 return the contained object pointer
	 */
	HGEEntity * arrow() { return this->handle.arrow(); }
	
	/**
	 return the contained object reference -- be sure the reference is not unset!
	 */
	HGEEntity& dot() { return this->handle.dot(); }
	
private:
	HGEHandle handle;
	HGEModel * cache;
};

NS_HGE_END

#endif
