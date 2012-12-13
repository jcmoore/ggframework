//
//  HGECap.h
//  hybridge
//
//  Created by The Narrator on 12/8/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECAP_H__
#define __HGECAP_H__

#include "dev/HGEMacros.h"
#include "dev/HGEPlatformMacros.h"

#include <vector>

NS_HGE_BEGIN

template < typename Storage >
class HGECap : public std::vector< Storage > {
	
	typedef std::vector< Storage > Collection;
	
	size_t indexCap;
	
public:
	
	HGECap()
	: indexCap(0) {
	}
	
	size_t recap() const { return this->indexCap; }
	size_t recap(size_t after) {
		size_t before = this->indexCap;
		this->indexCap = after;
		if (after > this->size()) {
			this->resize(after);
		}
		return before;
	}
	
	size_t upcap() {
		size_t c = ++this->indexCap;
		if (c > this->size()) {
			this->resize(c);
		}
		return c;
	}
	size_t upcap(Storage const value) {
		size_t c = ++this->indexCap;
		if (c > this->size()) {
			this->resize(c);
		}
		this->at(c - 1) = value;
		return c;
	}
	
	size_t downcap() {
		if (this->indexCap == 0) {
			return 0;
		}
		return --this->indexCap;
	}
	size_t downcap(Storage const value) {
		if (this->indexCap == 0) {
			return 0;
		}
		size_t c = --this->indexCap;
		this->at(c) = value;
		return c;
	}
	
	size_t incap(size_t idx, Storage const value) {
		if (idx >= this->indexCap) {
			return this->upcap(value);
		}
		size_t c = this->upcap() - 1;
		for (size_t i = c; i > idx; i--) {
			this->at(i) = this->at(i - 1);
		}
		this->at(idx) = value;
		return c+1;
	}
	
	bool excap(size_t idx, Storage * result = 0, bool cycle = 0) {
		if (this->indexCap == 0) {
			return 0;
		}
		size_t c = --this->indexCap;
		idx = idx > c ? c : idx;
		if (result) {
			*result = this->at(idx);
		}
		for (size_t i = idx; i < c; i++) {
			this->at(i) = this->at(i+1);
		}
		if (cycle) {
			this->at(c+1) = *result;
		}
		return !0;
	}
	bool excap(size_t idx, Storage const value, Storage * result) {
		if (this->indexCap == 0) {
			return 0;
		}
		size_t c = --this->indexCap;
		idx = idx > c ? c : idx;
		if (result) {
			*result = this->at(idx);
		}
		for (size_t i = idx; i < c; i++) {
			this->at(i) = this->at(i+1);
		}
		this->at(c+1) = value;
		return !0;
	}
	
	bool atcap(Storage * result) {
		if (this->indexCap == 0) {
			return 0;
		}
		if (result) {
			*result = this->at(this->indexCap - 1);
		}
		return !0;
	}
	bool atcap(Storage const after, Storage * result = 0) {
		if (this->indexCap == 0) {
			return 0;
		}
		if (result) {
			*result = this->at(this->indexCap - 1);
		}
		this->at(this->indexCap - 1) = after;
		return !0;
	}
	
	bool fromcap(int position, Storage * result) {
		size_t c = this->indexCap;
		if (position < 0 &&
			(size_t)(-position) >= c) {
			return 0;
		}
		size_t p = position + c - 1;
		if (result) {
			*result = this->at(p);
		}
		return !0;
	}
	bool fromcap(int position, Storage const after, Storage * result = 0) {
		size_t c = this->indexCap;
		if (position < 0 &&
			(size_t)(-position) >= c) {
			return 0;
		}
		size_t p = position + c - 1;
		if (result) {
			*result = this->at(p);
		}
		this->at(p) = after;
		return !0;
	}
	
	void flipcap() {
		size_t c = this->indexCap;
		for (size_t i = 1 + (c/2) - 1; i-- > 0; ) {
			Storage swap = this->at(i);
			this->at(i) = this->at(c-1-i);
			this->at(c-1-i) = swap;
		}
	}
	
	bool capped() {
		return this->size() == this->indexCap;
	}
};

NS_HGE_END

#endif
