//
//  HGECanCircuit.h
//  hybridge
//
//  Created by The Narrator on 11/3/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGECANCIRCUIT_H__
#define __HGECANCIRCUIT_H__

#include "core/can/HGECanImp.h"

#include <limits.h>

NS_HGE_BEGIN



template < typename Derived = void, typename Parent = Derived >
class HGECanCircuit;


template <>
class HGECanCircuit< void, void > {
public:
	
	struct Magic : public HGECanImp<>::Magic<> {
	};
};

template < typename Derived, typename Parent >
class HGECanCircuit : public Parent {
public:
	
	typedef HGECanCircuit MagicCircuit;
	typedef HGECanCircuit MagicParent;
	typedef Derived MagicDerived;
	typedef Parent RealParent;
	typedef HGECanCircuit RealSelf;
	
private:
	typedef HGECanImp<>::Magic< RealSelf, HGECanCircuit<>::Magic > Trick;
public:
	
	struct Magic : public Trick {
		Magic(RealSelf * d) : Trick(d) {}
	};
	
	virtual bool canYou(like_hge interface, HGECanImp<>::Magic<> ** result) {
		if (HGE_LIKEA( hybridge::HGECanCircuit ) == interface) {
			if (result) {
				*result = static_cast<hybridge::HGECanCircuit<>::Magic *>(this->feat());
			}
			return !0;
		} else {
			return this->RealParent::canYou(interface, result);
		}
	}
	
	Magic * feat() { return &magic; }
	
	HGECanCircuit()
	: magic(static_cast<RealSelf *>(this))
	, favorite(this)
	, popularity(0)
	{}
	
	~HGECanCircuit() {
		LoopCheckCount count = 0;
		
		MagicCircuit * circuit = 0;
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
	
private:
	
	Magic magic;
	
	enum {
		FAVOR_CLIFF = 15,
		REBALANCE_FACTOR = (unsigned)1000000,
		POPSTAR = INT_MAX
	};
	
public:
	
	typedef MagicDerived Circuited;
	
	template <typename Cond>
	struct Compatibility {
		typedef bool (Circuited::*Matcher)(Cond condition, Circuited ** result);
	};
	
protected:
	
	typedef char LoopCheckCount;
	
	template <typename Condition>
	bool find (typename Compatibility<Condition>::Matcher matcher,
			   Condition condition,
			   Circuited ** result,
			   MagicCircuit * first,
			   MagicCircuit * higherAuthority) {
		LoopCheckCount count = 0;
		
		MagicCircuit * circuit = this;
		while (!(static_cast<Circuited *>(circuit)->*matcher)(condition, result)) {
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
		MagicCircuit * circuit = this;
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
			MagicCircuit * circuit = this;
			do {
				circuit->popularity /= REBALANCE_FACTOR;
				circuit = circuit->favorite;
			} while (this != circuit->favorite);
		} else {
			HGEAssertC(REBALANCE_FACTOR > 0, "this is really important...");
		}
	}
	
	bool fuse(Circuited * circuit) {
		
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
	
	Circuited * splice(Circuited * hint = 0) {
		
		if (this == this->favorite) {
			return 0;
		}
		
		MagicCircuit * authority = hint;
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
		
		return static_cast<Circuited *>(authority);
	}
	
private:
	
	MagicCircuit * favorite;
	int popularity;
	
public:
	
	Circuited * board (bool checkAll = !0) {
		if (this->popularity == POPSTAR ||
			this == this->favorite) {
			return static_cast<Circuited *>(this);
		} else if (checkAll) {
			LoopCheckCount count = 0;
			
			MagicCircuit * circuit = this;
			while (this != (circuit = circuit->favorite)) {
				if (circuit->popularity == POPSTAR) {
					return static_cast<Circuited *>(circuit);
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
	
	Circuited * under() {
		return static_cast<Circuited *>(this->favorite);
	}
	
	template <typename Condition>
	bool integrated (typename Compatibility<Condition>::Matcher matcher,
					 Condition conditon,
					 Circuited ** result) {
		return this->find(matcher, conditon, result, this, 0);
	}
	
	template <typename Condition>
	bool unknown (typename Compatibility<Condition>::Matcher matcher,
				  Condition conditon,
				  Circuited ** result) {
		return (this == this->favorite) ? 0 : this->find(matcher, conditon, result, this->favorite, this);
	}
	
};

NS_HGE_END

#endif
