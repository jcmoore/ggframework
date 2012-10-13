//
//  HGEJSON.h
//  hybridge
//
//  Created by The Narrator on 8/16/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEJSON_H__
#define __HGEJSON_H__

#include "third_party/vapidjson/document.h"
#include "third_party/vapidjson/stringbuffer.h"
#include "third_party/vapidjson/writer.h"

#include "dev/HGEMacros.h"
#include "dev/HGEPlatformMacros.h"
#include "dev/HGEKeycode.h"
#include "util/HGEUtilString.h"
#include "core/HGEID.h"


#include <list>
#include <set>

#define HGEJSONRef_SUB_CACHED	1

NS_HGE_BEGIN

class HGEToolbox;

typedef vapidjson::Value JSONValue;
typedef vapidjson::Document JSONDoc;
typedef vapidjson::StringBuffer JSONBuffer;
typedef vapidjson::Writer< JSONBuffer > JSONWriter;
typedef vapidjson::SizeType JSONIndex;
typedef vapidjson::Value::Ch JSONKey;
typedef vapidjson::Value::Text JSONText;
typedef vapidjson::Value::Member JSONMember;
typedef vapidjson::Value::MemberIterator JSONMemberIter;
typedef vapidjson::Value::ValueIterator JSONValueIter;
typedef vapidjson::Type JSONType;

static JSONIndex const JSONZero = JSONIndex(0);

__attribute__ ((used)) static const char * JSON_SUBSTITUTION_DECLARATION = "$$";
__attribute__ ((used)) static const char * JSON_SUBSTITUTION_IDENTIFIER = "$";
__attribute__ ((used)) static const char * JSON_SUBSTITUTION_ESCAPER = "";

// TODO: consider just using a list<> * or something to store fusion element references

struct HGEJSONRef {
	
	typedef JSONType Type;
	typedef JSONValue Value;
	typedef JSONDoc Doc;
	typedef JSONKey Key;
	typedef JSONIndex Index;
	typedef JSONMember Member;
	typedef JSONMemberIter MemberIter;
	typedef JSONValueIter ValueIter;
	
	static Value& Null() {
		static Value instance;
		return instance;
	}
	
	static Value& Undefined() {
		static Value instance(vapidjson::kUndefinedType);
		return instance;
	}
	
	static HGEJSONRef& UndefinedRef() {
		static HGEJSONRef instance(&Undefined());
		return instance;
	}
	
	static Index UndefinedCount() {
		static Index instance(UINT_MAX);
		return instance;
	}
	
	bool unset () { return this->stored.value == 0 || this->stored.value->IsUndefined(); }
	
	HGEJSONRef fuser() {
#if !HGE_TEST
		HGEAssertC(0, "HGEJSONRef::fuser() should only be used during tests");
		return UndefinedRef();  //
#else
		return HGEJSONRef(&this->sub());
#endif
	}
	
	HGEJSONRef operator()(HGEJSONRef * shortcut) // WARNING:  NOT TO BE CACHED UNDER ANY CIRCUMSTANCES
	{
		HGEJSONRef s;
		
		if (!shortcut) {
			shortcut = &s;
		}
		
		if (this->unset()) {
			return (*shortcut = *this);
		} else {
			HGEJSONRef s(&this->sub());
			return (*shortcut = this->subShortcut(s));
		}
	}
	
	Value& operator()() // TODO: try to make this const return value
	{
		HGEJSONRef result;
		
		if (this->unset()) return Undefined();
			
			Value * pointer = &this->sub();
			result = this->subShortcut(result.hold(pointer));
			
			if (result.unset()) {
				return Undefined();
			}
		
		pointer = &result.sub();
		
		if (pointer->IsUndefined()) {
			return *result.stored.value;
		}
		
		pointer = &result.subEscape();
		
		if (!pointer->IsUndefined()) {
			// WARNING: using this return value to initalize another HGEJSONRef instance may circumvent the ability to "escape"
			return *pointer;
		}
		
		//HGEAssertC(0, "this result is very misleading, the value is undefined because the reference is a fusion");
		return Undefined();
	}
	
	HGEJSONRef operator[](const Key* key) {
		if (!key) { return (*this)[JSONZero]; } // TODO: make sure this is wise...
		return this->getKey(key, 0, 0);
	}
	const HGEJSONRef operator[](const Key* key) const { return const_cast<HGEJSONRef&>(*this)[key]; }
	
	HGEJSONRef operator[](Index index) {
		Index count = 0;
		return this->getIndex(index, count, 0);
	}
	const HGEJSONRef operator[](Index index) const { return const_cast<HGEJSONRef&>(*this)[index]; }
	
	//operator Value&() { return (*this)(); }
	
	bool associative() {
		Index count = 0;
		this->getIndex(0, count, 0);
		return count == UndefinedCount();
	}
	
protected:
	
	struct Kter {
		static MemberIter const& UndefinedMemberIter() {
			static MemberIter instance;
			return instance;
		}
		
		bool terminated() {
			return this->refStack.empty();
		}
		
		MemberIter::value_type deref() {
			return (*this->mter);
		}
		
		const Key * key() {
			return (this->mter != UndefinedMemberIter()) ? ((*this->mter).first).c_str() : 0;
		}
		
		Value& value() {
			return (this->mter != UndefinedMemberIter() && (*this->mter).second) ? *((*this->mter).second) : HGEJSONRef::Undefined();
		}
		
		Kter& inc() {
			if (this->refStack.empty()) {
				return (*this);
			}
			
			Value * pointer = 0;
			
			while (!this->refStack.empty()) {
				if (!pointer) {
					pointer = &this->leafSub();
					HGEAssertC(pointer->IsUndefined(), "non-fusion leaf expected, fusion-leaf encountered, ref = this->refStack.back()(++(this->indexStack.back()), count, this) will yield undesireable results");
				} else {
					pointer = &this->leafSub();
				}
				
				if (!pointer->IsUndefined()) { // leaf is a fusion
					HGEJSONRef substitute(pointer);
					
					pointer = &substitute.sub();
					if (!pointer->IsUndefined()) { // leaf is a fusion (based on a fused fusion list)
						substitute = substitute.subShortcut(substitute);
						HGEJSONRef ref = substitute[++(this->indexStack.back())];
						if (!ref.unset()) { // exhausted this fusion leaf
							ref = ref.getKey(0, !0, this);
							if (!ref.unset()) {
								if (this->mter != UndefinedMemberIter()) {
									if (this->keySet.find((*this->mter).first.c_str()) == this->keySet.end()) {
										this->keySet.insert((*this->mter).first.c_str());
										return (*this);
									} else {
										continue;
									}
								}
								HGEAssertC(0, "plot hole candidate");
								return (*this);
							} else {  // exhausted this fusion leaf
								continue;
							}
						} else {
							this->refStack.pop_back();
							this->indexStack.pop_back();
							this->mter = UndefinedMemberIter();
						}
						continue;
					} else {
						pointer = substitute.stored.value;
						if (pointer->IsArray()) {
							HGEJSONRef ref(&(*pointer)[++(this->indexStack.back())]);
							//ref = ref.subShortcut(ref); // TODO: is this necessary?
							if (!ref.unset()) {
								ref = ref.getKey(0, !0, this);
								if (!ref.unset()) {
									if (this->mter != UndefinedMemberIter()) {
										if (this->keySet.find((*this->mter).first.c_str()) == this->keySet.end()) {
											this->keySet.insert((*this->mter).first.c_str());
											return (*this);
										} else {
											continue;
										}
									}
									HGEAssertC(0, "plot hole candidate");
									return (*this);
								} else {  // exhausted this fusion leaf
									continue;
								}
							} else { // exhausted this fusion leaf
								this->refStack.pop_back();
								this->indexStack.pop_back();
								this->mter = UndefinedMemberIter();
							}
						} else if (pointer->IsNumber()) {
							HGEAssertC(pointer->GetUint64() == 0,
									   "what was expected to be an escape sequence was found to have non-zero %s: %li",
									   HGE_KEYTEXT_UUID,
									   pointer->GetUint64());
							pointer = &this->refStack.back().subEscape();
							HGEAssertC(pointer->IsObject(), "plot hole found");
							if (this->mter != UndefinedMemberIter()) {
								this->mter++;
								while (this->mter != pointer->MemberEnd()) {
									if (this->keySet.find((*this->mter).first.c_str()) == this->keySet.end()) {
										this->keySet.insert((*this->mter).first.c_str());
										break;
									}
									this->mter++;
								}
							}
							if (this->mter != UndefinedMemberIter() &&
								this->mter != pointer->MemberEnd()) {
								this->indexStack.back()++;
								return (*this);
							} else { // exhausted this leaf
								this->refStack.pop_back();
								this->indexStack.pop_back();
								this->mter = UndefinedMemberIter();
							}
						}
					}
					continue;
				} else { // leaf is an object
					pointer = this->refStack.back().stored.value;
					HGEAssertC(pointer->IsObject(), "plot hole found");
					if (this->mter != UndefinedMemberIter()) {
						this->mter++;
						while (this->mter != pointer->MemberEnd()) {
							if (this->keySet.find((*this->mter).first.c_str()) == this->keySet.end()) {
								this->keySet.insert((*this->mter).first.c_str());
								break;
							}
							this->mter++;
						}
					}
					if (this->mter != UndefinedMemberIter() &&
						this->mter != pointer->MemberEnd()) {
						this->indexStack.back()++;
						return (*this);
					} else { // exhausted this leaf
						this->refStack.pop_back();
						this->indexStack.pop_back();
						this->mter = UndefinedMemberIter();
					}
				}
			}
			
			return (*this);
		}
#if HGE_DEBUG
		__attribute__ ((used))
#endif
		void logset() {
			HGEPrint("(<\n");
			std::set<const char *, cmpstr>::iterator iter = this->keySet.begin();
			for (; iter != this->keySet.end(); iter++) {
				HGEPrint("    %s\n", (*iter));
			}
			HGEPrint(">)\n");
		}
		
	protected:
		std::list<HGEJSONRef> refStack;
		std::list<Index> indexStack;
		std::set<const char *, cmpstr> keySet;
		
		MemberIter mter;
		
		Kter(HGEJSONRef * head) {
			if (!head) {
				return;
			}
			
			this->mter = UndefinedMemberIter();
			
			head->getKey(0, !0, this);
			
			this->keyFirst();
		}
		
		Kter(std::list<HGEJSONRef>& rStack, std::list<Index>& iStack, Value * kFound)
		: refStack(rStack)
		, indexStack(iStack)
		{
			HGEAssertC(kFound && kFound->IsObject(), "plot hole detected");
			this->mter = kFound ? kFound->MemberBegin() : UndefinedMemberIter();
			
			this->refStack.push_back(HGEJSONRef(kFound));
			this->indexStack.push_back(0);
			
			this->keyFirst();
		}
		
		void keyFirst() {
			if (this->keySet.empty()) {
				if (this->mter != UndefinedMemberIter()) {
					this->keySet.insert((*this->mter).first.c_str());
				}
			}
		}
		
		inline Value& leafSub() {
			return this->refStack.empty() ? HGEJSONRef::Undefined() : this->refStack.back().sub();
		}
		
		friend class HGEJSONRef;
		
		friend class Iter;
	};
	
	struct Iter {
		static ValueIter const& UndefinedValueIter() {
			static ValueIter instance;
			return instance;
		}
		
		Kter asKter() {
			return Kter(this->refStack, this->indexStack, this->keyedFound);
		}
		
		bool terminated() {
			return this->refStack.empty();
		}
		
		Value& deref() {
			return (this->vter != UndefinedValueIter() && (*this->vter)) ? *(*this->vter) : HGEJSONRef::Undefined();
		}
		
		Iter& inc() {
			if (this->refStack.empty()) {
				return (*this);
			}
			Value * pointer = 0;
			
			while (!this->refStack.empty()) {
				if (!pointer) {
					pointer = &this->leafSub();
					HGEAssertC(pointer->IsUndefined(), "non-fusion leaf expected, fusion-leaf encountered, ref = this->refStack.back()(++(this->indexStack.back()), count, this) will yield undesireable results");
				} else {
					pointer = &this->leafSub();
				}
				
				if (!pointer->IsUndefined()) { // leaf is a fusion
					HGEJSONRef substitute(pointer);
					
					pointer = &substitute.sub();
					if (!pointer->IsUndefined()) { // leaf is a fusion (based on a fused fusion list)
						substitute = substitute.subShortcut(substitute);
						Index size = 0;
						HGEJSONRef ref = substitute[++(this->indexStack.back())];
						if (!ref.unset()) { // exhausted this fusion leaf
							ref = ref.getIndex(0, size, this);
							if (!ref.unset()) {
								return (*this);
							} else {  // exhausted this fusion leaf
								continue;
							}
						} else {
							this->refStack.pop_back();
							this->indexStack.pop_back();
							this->vter = UndefinedValueIter();
						}
						continue;
					} else {
						pointer = substitute.stored.value;
						if (pointer->IsArray()) {
							Index size = 0;
							HGEJSONRef ref(&(*pointer)[++(this->indexStack.back())]);
							//ref = ref.subShortcut(ref); // TODO: is this necessary?
							if (!ref.unset()) { // exhausted this fusion leaf
								ref = ref.getIndex(0, size, this);
								if (!ref.unset()) {
									return (*this);
								} else {  // exhausted this fusion leaf
									continue;
								}
							} else {
								this->refStack.pop_back();
								this->indexStack.pop_back();
								this->vter = UndefinedValueIter();
							}
						} else if (pointer->IsNumber()) {
							HGEAssertC(pointer->GetUint64() == 0,
									   "what was expected to be an escape sequence was found to have non-zero %s: %li",
									   HGE_KEYTEXT_UUID,
									   pointer->GetUint64());
							pointer = &this->refStack.back().subEscape();
							HGEAssertC(pointer->IsArray(), "plot hole found");
							if (this->vter != UndefinedValueIter() &&
								(++this->vter) != pointer->End()) {
								this->indexStack.back()++;
								return (*this);
							} else { // exhausted this leaf
								this->refStack.pop_back();
								this->indexStack.pop_back();
								this->vter = UndefinedValueIter();
							}
						}
					}
					continue;
				} else { // leaf is an array
					pointer = this->refStack.back().stored.value;
					HGEAssertC(pointer->IsArray(), "plot hole found");
					if (this->vter != UndefinedValueIter() &&
						(++this->vter) != pointer->End()) {
						this->indexStack.back()++;
						return (*this);
					} else { // exhausted this leaf
						this->refStack.pop_back();
						this->indexStack.pop_back();
						this->vter = UndefinedValueIter();
					}
				}
			}
			
			return (*this);
		}
		
		bool associative () { return this->keyedFound ? !this->keyedFound->IsUndefined() : 0; }
		
	protected:
		std::list<HGEJSONRef> refStack;
		std::list<Index> indexStack;
		
		ValueIter vter;
		Value * keyedFound;
		
		Iter(HGEJSONRef * head) {
			this->keyedFound = 0;
			
			if (!head) {
				return;
			}
			
			this->vter = UndefinedValueIter();
			
			Index count = 0;
			head->getIndex(0, count, this);
		}
		
		inline Value& leafSub() {
			return this->refStack.empty() ? HGEJSONRef::Undefined() : this->refStack.back().sub();
		}
		
		friend class HGEJSONRef;
	};
	
	HGEJSONRef getKey(const Key* key, bool anykey, Kter * ater) {
		HGEJSONRef result;
		
		if (this->unset()) return (*this);
		
		Value * pointer = &this->sub();
		result = this->subShortcut(result.hold(pointer));
		
		if (result.unset()) {
			return UndefinedRef();
		}
		
		pointer = &result.sub();
		
		if (!pointer->IsUndefined()) { // shortcut is a fusion (or an escape)
			if (ater) {
				ater->refStack.push_back(result);
				ater->indexStack.push_back(0);
				ater->mter = Kter::UndefinedMemberIter();
			}
			pointer = &result.subKey(pointer, key, anykey, ater);
			return result.hold(pointer);
		} else {
			pointer = result.stored.value;
			if (pointer->IsObject()) {
				Index counter = 0;
				size_t range = 0;
				if (key) {
					range = strlen(key);
				} else {
					HGEAssertC(anykey, "this is only permissable when any key is acceptible");
				}
				for (MemberIter m = pointer->MemberBegin(); m != pointer->MemberEnd(); m++) {
					if (anykey ||
						(memcmp((*m).first.c_str(), key, range * sizeof(Key)) == 0)) {
						if (ater) {
							ater->refStack.push_back(result);
							ater->indexStack.push_back(counter);
							ater->mter = m;
						}
						return result.hold((*m).second);
					}
					counter++;
				}
				
				return UndefinedRef();
			} else {
				return UndefinedRef();
			}
		}
	}
	
	HGEJSONRef getIndex(Index index, Index& count, Iter * ater) {
		HGEJSONRef result;
		
		if (this->unset()) return (*this);
		
		Value * pointer = &this->sub();
		result = this->subShortcut(result.hold(pointer));
		
		if (result.unset()) {
			return UndefinedRef();
		}
		
		pointer = &result.sub();
		
		if (!pointer->IsUndefined()) { // shortcut is a fusion (or an escape)
			if (ater) {
				ater->refStack.push_back(result);
				ater->indexStack.push_back(0);
				ater->vter = Iter::UndefinedValueIter();
			}
			pointer = &result.subIndex(pointer, index, count, ater);
			return result.hold(pointer);
		} else { // shortcut is not a fusion
			pointer = result.stored.value;
			if (pointer->IsArray()) { // shortcut is a standard array
				Index size = pointer->Size();
				count += size;
				if (size > index) {
					for (ValueIter v = pointer->Begin(); v != pointer->End(); v++) {
						if (index-- == 0) {
							pointer = (*v);
							if (ater) {
								ater->refStack.push_back(result);
								ater->indexStack.push_back(index);
								ater->vter = v;
								if (!ater->keyedFound) {
									ater->keyedFound = &Undefined();
								}
							}
							return result.hold(pointer);
						}
					}
					HGEAssert(0, "size inconsistency");
					return UndefinedRef();
				} else {
					return UndefinedRef();
				}
			} else { // shortcut is not an array...
				if (pointer->IsObject()) { // indicate to fusion-type-testers that this fusion is keyed (i.e. associative) not indexed
					count = UndefinedCount();
					if (ater &&
						!ater->keyedFound) {
						ater->keyedFound = pointer;
					}
				}
				return UndefinedRef();
			}
		}
	}
	
	union Storage { // this seems somewhat unsafe... oh well
		Value * value;
		//Doc * doc;
	};
	Storage stored;
	
#if HGEJSONRef_SUB_CACHED
	Value * subCache; // caching is not mandatory...
#endif
	
	HGEJSONRef(Value * v);
	
	HGEJSONRef& hold(Value * v);
	
	Value& sub ();
	
	HGEJSONRef subShortcut(HGEJSONRef& substitute); // return value is either a no-sub, a fusion (with either a fusion list or a fused fusion list), or an escape sequence
	Value& subEscape();
	
	Type subType(Value& substitute);
	
	HGEJSONRef& subDeref(Value * pointer);
	HGEJSONRef& subDeref(id_hge hgeuuid);
	
	Value& subValue(Value& substitute, Type kind);
	
	Value& subKey(Value * pointer, const Key* key, bool anykey, Kter * ater);
	
	Value& subIndex(Value * pointer, Index index, Index& count, Iter * ater);
	
	bool subbing() { return !this->sub().IsUndefined(); }
	
public:
	HGEJSONRef();
	
	
	
	template <typename Handler>
	HGEJSONRef& traverse(Handler& handler) {
		return this->traverse(handler, 0);
	}
	
	template <typename Handler>
	HGEJSONRef& reinterpret(Handler& handler) {
		return this->traverse(handler, !0);
	}
	
protected:
	
	template <typename Handler>
	HGEJSONRef& traverse(Handler& handler, bool replace) {
		Value& data = this->stored.value ? *this->stored.value : Undefined();
		Value * pointer = 0;
		HGEJSONRef substitute;
		switch(data.GetType()) {
			case vapidjson::kUndefinedType:	VAPIDJSON_ASSERT(0); handler.Null(); break;
			case vapidjson::kNullType:		handler.Null(); break;
			case vapidjson::kFalseType:		handler.Bool(false); break;
			case vapidjson::kTrueType:		handler.Bool(true); break;
				
			case vapidjson::kObjectType:
				if (!replace ||
					(pointer = &this->sub())->IsUndefined()) {
					handler.StartObject();
					for (MemberIter m = data.MemberBegin(); m != data.MemberEnd(); ++m) {
						handler.String(m->first.c_str(), m->first.length(), false);
						substitute.hold(m->second).traverse(handler, replace); // standard behavior
					}
					handler.EndObject((data.Count()) / sizeof(MemberIter));
				} else {
					substitute = this->subShortcut(substitute.hold(pointer));
					pointer = &substitute.sub();
					
					if (!pointer->IsUndefined()) {
						HGEJSONRef ref = substitute;
						substitute.hold(pointer);
						pointer = &substitute.sub();
						
						if (!pointer->IsUndefined()) { // this is a fusion (based on a fused fusion list)
							Index count = 0;
							Iter iter(&ref); // TODO: check that this should be ref and not substitute
							if (!iter.associative()) {
								handler.StartArray();
								while (!iter.terminated()) {
									iter.deref().Accept(handler);
									iter.inc();
									count++;
								}
								handler.EndArray(count);
							} else {
								Kter kter = iter.asKter();
								handler.StartObject();
								while (!kter.terminated()) {
									HGEAssert(kter.key(), "bad iterator");
									JSONValue field(kter.key());
									field.Accept(handler);
									kter.value().Accept(handler);
									kter.inc();
									count++;
								}
								handler.EndObject(count);
							}
						} else {
							pointer = substitute.stored.value;
							
							if (pointer->IsArray()) { // this is a fusion
								Index count = 0;
								Iter iter(&ref);
								if (!iter.associative()) {
									handler.StartArray();
									while (!iter.terminated()) {
										iter.deref().Accept(handler);
										iter.inc();
										count++;
									}
									handler.EndArray(count);
								} else {
									Kter kter = iter.asKter();
									handler.StartObject();
									while (!kter.terminated()) {
										HGEAssert(kter.key(), "bad iterator");
										JSONValue field(kter.key());
										field.Accept(handler);
										kter.value().Accept(handler);
										kter.inc();
										count++;
									}
									handler.EndObject(count);
								}
							} else if (pointer->IsNumber()) { // this is an escape sequence
								HGEAssertC(pointer->GetUint64() == 0,
										   "what was expected to be an escape sequence was found to have non-zero %s: %li",
										   HGE_KEYTEXT_UUID,
										   pointer->GetUint64());
								pointer = &this->subEscape();
								pointer->Accept(handler);
							} else {
								HGEAssertC(0, "only escapes sequences and fusions should be the candidates to execute this sequence");
								Undefined().Accept(handler);
							}
						}
					} else {
						substitute.traverse(handler, replace);
					}
				}
				break;
				
			case vapidjson::kArrayType:
				handler.StartArray();
				for (ValueIter v = data.Begin(); v != data.End(); ++v) {
					substitute.hold((*v)).traverse(handler, replace);
				}
				handler.EndArray(data.Size());
				break;
				
			case vapidjson::kStringType:
				handler.String(data.GetString(), data.GetStringLength(), false);
				break;
				
			case vapidjson::kNumberType:
				if (data.IsInt())			handler.Int(data.GetInt());
				else if (data.IsUint())		handler.Uint(data.GetUint());
				else if (data.IsInt64())	handler.Int64(data.GetInt64());
				else if (data.IsUint64())	handler.Uint64(data.GetUint64());
				else						handler.Double(data.GetDouble());
				break;
		}
		return *this;
	}
	
public:
#if HGE_DEBUG
	__attribute__ ((used))
#endif
	void log(HGEToolbox * toolbox);
};

struct HGEJSON : public HGEJSONRef {
	
	static HGEJSON& UndefinedDoc() {
		static HGEJSON instance;
		return instance;
	}
	
	HGEJSON(JSONValue& json, HGEToolbox * toolbox);
	HGEJSON();
	~HGEJSON();
	
	HGEJSON& mimic(JSONValue& json, HGEToolbox * toolbox);
	
protected:
	
	HGEJSON& mimic(JSONValue& json);
};

//typedef HGEJSONRef JSONRef; // TODO: make this type the type that gets passed around between HGE services (or maybe not...)

__attribute__ ((used)) void jsonlog(JSONValue& json, HGEToolbox * toolbox); // this will be a no-op #if !HGE_DEBUG -- (so I think it should be compiled out)

__attribute__ ((used)) void jsonlogr(HGEJSONRef& json, bool replace, HGEToolbox * toolbox); // this will be a no-op #if !HGE_DEBUG (so I think it should be compiled out)

extern "C" {
#if HGE_DEBUG
	__attribute__ ((used)) static void jsout(JSONValue& json) {
		return jsonlog(json, 0);
	}
	__attribute__ ((used)) static void jsouto(HGEJSONRef& json) {
		return jsonlogr(json, 0, 0);
	}
	__attribute__ ((used)) static void jsoutr(HGEJSONRef& json) {
		return jsonlogr(json, !0, 0);
	}
	__attribute__ ((used)) static void lljlog(std::list<HGEJSONRef>& list) {
		HGEPrint("(<\n");
		for (std::list<HGEJSONRef>::iterator i = list.begin(); i != list.end(); i++) {
			HGEPrint("    ");
			(*i).log(0);
			HGEPrint("\n");
		}
		HGEPrint(">)\n");
		return;
	}
	__attribute__ ((used)) static void llilog(std::list<JSONIndex>& list) {
		HGEPrint("(<\n");
		for (std::list<JSONIndex>::iterator i = list.begin(); i != list.end(); i++) {
			HGEPrint("     %i\n", (*i));
		}
		HGEPrint(">)\n");
		return;
	}
	__attribute__ ((used)) static void llvlog(std::list<JSONValue *>& list) {
		HGEPrint("(<\n");
		for (std::list<JSONValue *>::iterator i = list.begin(); i != list.end(); i++) {
			HGEPrint("    ");
			jsonlog(*(*i), 0);
			HGEPrint("\n");
		}
		HGEPrint(">)\n");
		return;
	}
#endif
}

NS_HGE_END

#endif
