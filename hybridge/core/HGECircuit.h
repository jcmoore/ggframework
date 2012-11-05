//
//  HGECircuit.h
//  hybridge
//
//  Created by The Narrator on 11/3/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECIRCUIT_H__
#define __HGECIRCUIT_H__

#include "dev/HGEMacros.h"
#include "dev/HGEPlatformMacros.h"
#include "core/HGEImplements.h"

#include <limits.h>

NS_HGE_BEGIN




HGE_BOILERPLATE(HGEChip, BaseType)
{
	HGE_BOILERPLATE_SUGAR_IMP(HGEChip, ImpChip);
	HGE_BOILERPLATE_SUGAR_REAL(BaseType, RealChip);
	
public:
	
	virtual bool does(kind_hge isolatedInterface) {
		return isolatedInterface == HGEKind< HGEChip < HGEBasis > >() || BaseType::does(isolatedInterface);
	}
	
	typedef void * Condition;
	typedef bool (RealChip::*Matcher)(Condition condition, RealChip ** result);
	
protected:
	
	bool known (Matcher matcher, Condition condition, RealChip ** result = 0) {
		return (static_cast<RealChip *>(this)->*matcher)(condition, result);
	}
	
public:
	
};





HGE_BOILERPLATE(HGECircuit, BaseType)
{
	HGE_BOILERPLATE_SUGAR_IMP(HGECircuit, ImpCircuit);
	HGE_BOILERPLATE_SUGAR_REAL(BaseType, RealCircuit);
	
public:
	
	virtual bool does(kind_hge isolatedInterface) {
		return isolatedInterface == HGEKind< HGECircuit < HGEBasis > >() || BaseType::does(isolatedInterface);
	}
	
private:
	
	enum {
		FAVOR_CLIFF = 15,
		REBALANCE_FACTOR = (unsigned)1000000,
		POPSTAR = INT_MAX
	};
	
public:
	
	typedef void * Condition;
	typedef bool (RealCircuit::*Matcher)(HGECircuit::Condition condition, RealCircuit ** result);
	
protected:
	
	typedef char LoopCheckCount;
	
	bool find (Matcher matcher,
			   Condition condition,
			   RealCircuit ** result,
			   ImpCircuit * first,
			   ImpCircuit * higherAuthority) {
		LoopCheckCount count = 0;
		
		ImpCircuit * circuit = this;
		while (!(static_cast<RealCircuit *>(circuit)->*matcher)(condition, result)) {
			if (circuit->favorite == first) {
				return 0;
			} else if (++count > 0) {
				circuit->tune(0);
				circuit = circuit->favorite;
				higherAuthority = higherAuthority ? higherAuthority->favorite : (circuit != first) ? first : 0;
				continue;
			} else {
				HGEAssertC(0, "suspected infinite loop");
				return 0;
			}
		}
		
		circuit->tune(!0);
		
		// the pop star cannot be promoted any further
		// nor can the pop star's favorite be promoted
		if (higherAuthority &&
			circuit->popularity != POPSTAR &&
			higherAuthority->favorite->popularity != POPSTAR) {
			// circuit is ("much") more popular than its immediate authority then promote it
			if (higherAuthority->favorite->popularity < circuit->popularity - FAVOR_CLIFF) {
				higherAuthority->favorite->favorite = circuit->favorite;
				circuit->favorite = higherAuthority->favorite;
				higherAuthority->favorite = circuit;
			}
		}
		
		return !0;
	}
	
	void tune (bool positively) {
		ImpCircuit * circuit = this;
		if (!circuit->board(0)) {
			int amount = positively ? 1 : -1;
			circuit->popularity += amount;
			if (circuit->board(0)) {
				circuit->popularity -= amount;
				HGEAssertC(!circuit->board(0), "somehow circuit was not properly adjusted");
				circuit->rebalance();
			}
		}
	}
	
	void rebalance() {
		HGEAssertC(0, "a bit surprised(/worried) this actually happened...");
		if (REBALANCE_FACTOR > 0) {
			HGECircuit * circuit = this;
			do {
				circuit->popularity /= REBALANCE_FACTOR;
				circuit = circuit->favorite;
			} while (this != circuit->favorite);
		} else {
			HGEAssertC(REBALANCE_FACTOR > 0, "this is really important...");
		}
	}
	
	bool fuse(RealCircuit * circuit) {
		
		if (circuit != circuit->favorite) {
			return 0;
		}
		
		circuit->popularity = this->favorite->popularity;
		
		if (this == this->favorite) {
			this->popularity = POPSTAR;
		}
		
		circuit->favorite = this->favorite;
		this->favorite = circuit;
		
		return !0;
	}
	
	RealCircuit * splice(RealCircuit * hint = 0) {
		
		if (this == this->favorite) {
			return 0;
		}
		
		ImpCircuit * authority = hint;
		LoopCheckCount count = 0;
		
		while (authority) {
			if (authority->favorite == this) {
				break;
			} else if (authority->favorite == hint) {
				authority = 0;
			} else if (++count > 0) {
				authority = authority->favorite;
			} else {
				HGEAssertC(0, "suspected infinite loop");
				return 0;
			}
		}
		
		if (!authority) {
			count = 0;
			authority = this->favorite;
			while (authority) {
				if (authority->favorite == this) {
					break;
				} else if (authority->favorite == hint) {
					authority = 0;
				} else if (++count > 0) {
					authority = authority->favorite;
				} else {
					HGEAssertC(0, "suspected infinite loop");
					return 0;
				}
			}
		}
		
		if (this->popularity == POPSTAR) {
			this->favorite->popularity = POPSTAR;
		}
		
		authority->favorite = this->favorite;
		
		if (authority == authority->favorite) {
			HGEAssertC(authority->popularity == POPSTAR, "assumptions failed!");
			authority->popularity = 0;
		}
		
		// the new authority is the old favorite
		authority = this->favorite;
		this->favorite = this;
		this->popularity = 0;
		
		return static_cast<RealCircuit *>(authority);
	}
	
private:
	
	ImpCircuit * favorite;
	int popularity;
	
public:
	
	HGECircuit() : favorite(this) , popularity(0) {}
	~HGECircuit() {
		LoopCheckCount count = 0;
		
		ImpCircuit * circuit = 0;
		while (this != (circuit = this->favorite) &&
			   circuit->splice(0)) {
			if (++count > 0) {
				delete circuit;
			} else {
				delete circuit;
				HGEAssertC(0, "suspected infinite loop");
				break;
			}
		}
		HGEAssertC(this->popularity == 0, "assumption failed!");
	}
	
	RealCircuit * board (bool checkAll = !0) {
		if (this->popularity == POPSTAR ||
			this == this->favorite) {
			return static_cast<RealCircuit *>(this);
		} else if (checkAll) {
			LoopCheckCount count = 0;
			
			ImpCircuit * circuit = this;
			while (this != (circuit = circuit->favorite)) {
				if (circuit->popularity == POPSTAR) {
					return static_cast<RealCircuit *>(circuit);
				} else if (++count > 0) {
					circuit = circuit->favorite;
					continue;
				} else {
					HGEAssertC(0, "suspected infinite loop");
					return 0;
				}
			}
			return 0;
		} else {
			return 0;
		}
	}
	
	RealCircuit * under() {
		return static_cast<RealCircuit *>(this->favorite);
	}
	
	bool integrated (Matcher matcher,
					 Condition conditon,
					 RealCircuit ** result) {
		return this->find(matcher, conditon, result, this, 0);
	}
	
	bool unknown (Matcher matcher,
				  Condition conditon,
				  RealCircuit ** result) {
		return (this == this->favorite) ? 0 : this->find(matcher, conditon, result, this->favorite, this);
	}
	
};

NS_HGE_END

#endif
