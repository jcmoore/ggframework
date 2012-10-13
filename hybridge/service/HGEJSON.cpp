//
//  HGEJSON.cpp
//  hybridge
//
//  Created by The Narrator on 8/16/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGEJSON.h"

#include "service/HGEInterface.h"
#include "service/HGEDispatch.h"

#include "core/HGESurrogate.h"

NS_HGE_BEGIN

void jsonlog(JSONValue& json, HGEToolbox * toolbox) {
#if HGE_DEBUG
	JSONBuffer stream;
	JSONWriter output(stream);
	json.Accept(output);
	HGEToolboxLog(toolbox, stream.GetString());
#endif
}

void jsonlogr(HGEJSONRef& json, bool replace, HGEToolbox * toolbox) {
#if HGE_DEBUG
	JSONBuffer stream;
	JSONWriter output(stream);
	if (!replace) {
		json.traverse(output);
	} else {
		json.reinterpret(output);
	}
	HGEToolboxLog(toolbox, stream.GetString());
#endif
}

void HGEJSONRef::log(HGEToolbox * toolbox) {
#if HGE_DEBUG
	if (this->unset()) {
		return jsonlog(Undefined(), 0);
	} else {
		return jsonlog(*this->stored.value, 0);
	}
#endif
}

HGEJSONRef::HGEJSONRef() {
	this->hold(0);
}

HGEJSONRef::HGEJSONRef(HGEJSONRef::Value * v) {
	this->hold(v);
}

HGEJSONRef& HGEJSONRef::hold(HGEJSONRef::Value * v){
	this->subCache = 0;
	this->stored.value = v;
	
	if (this->stored.value) {
		this->sub(); // pre-fetch sub for caching
	}
	
	return *this;
}

HGEJSONRef::Value& HGEJSONRef::sub() {
	if (!this->stored.value) {
		return HGEJSONRef::Undefined();
	}
	
	if (this->subCache) {
		return *this->subCache;
	}
	
	if (!this->stored.value->IsObject()) {
		return *(this->subCache = &HGEJSONRef::Undefined());
	}
	
	Value& s = (*this->stored.value)[JSON_SUBSTITUTION_IDENTIFIER];
	
	if (s.IsUndefined())
	{
		return *(this->subCache = &HGEJSONRef::Undefined());
	}
	
#if HGEJSONRef_SUB_CACHED
	this->subCache = &s;
	
	return *this->subCache;
	return s;
#else
	return s;
#endif
}

HGEJSON::Value& HGEJSONRef::subEscape() {
	if (!this->stored.value) {
		HGEAssertC(0, "reached escape invovation with an unset json reference");
		return HGEJSONRef::Undefined();
	}
	
	if (!this->stored.value->IsObject()) {
		HGEAssertC(0, "reached escape invocation with a non-object json reference");
		return HGEJSONRef::Undefined();
	}
	
	return (*this->stored.value)[JSON_SUBSTITUTION_ESCAPER];
}

HGEJSONRef HGEJSONRef::subShortcut(HGEJSONRef& substitute) {
	
	HGEJSONRef parent = *this;
	
	if (substitute.unset()) {
		return parent;
	}
	
	Value * pointer = 0;
	
	pointer = &substitute.sub();
	
	for (int safety = 0; safety < 1000; safety++) {
		HGEAssertC(substitute.stored.value, "ref value must be assigned!");
		
		if (pointer->IsUndefined()) { // substitute is plain-old-json
			
			pointer = substitute.stored.value;
			
			if (pointer->IsArray()) { // fusion list
				return parent;
			} else if (pointer->IsUint64()) { // probably a deref
				id_hge hgeuuid = pointer->GetUint64();
				if (hgeuuid != 0) { // deref
					substitute = parent.subDeref(hgeuuid);
					
					if (substitute.unset()) {
						HGEAssertC(0, "failed to dereference %s: %li", HGE_KEYTEXT_UUID, hgeuuid);
						return HGEJSONRef::UndefinedRef();
					}
					
					pointer = &substitute.sub();
					
					if (pointer->IsUndefined()) {
						return substitute;
					}
					
					// recursively use dereferenced value
					parent = substitute;
					substitute.hold(pointer);
					pointer = &substitute.sub();
					continue;
				} else { // escape sequence!
					pointer = &parent.subEscape();
					
					if (!pointer->IsUndefined()) {
						return parent;
					} else {
						HGEAssertC(0, "shortcut to undefined escape sequence", pointer->GetType());
						return HGEJSONRef::UndefinedRef();
					}
				}
			} else {
				HGEAssertC(pointer->IsUndefined(), "tried to shortcut to an object that is neither a fusion nor deref (GetType() == %i)", pointer->GetType());
				return HGEJSONRef::UndefinedRef();
			}
		} else { // substitute itself is a substitution
			if (pointer->IsArray()) { // substitute itself is a fused fusion list substitution
				return parent;
			} else if (pointer->IsUint64()) { // substitute itself is probably a deref
				id_hge hgeuuid = pointer->GetUint64();
				if (hgeuuid != 0) { // substitute itself is a deref
					substitute = parent.subDeref(hgeuuid);
					
					if (substitute.unset()) {
						HGEAssertC(0, "failed to dereference %s: %li", HGE_KEYTEXT_UUID, hgeuuid)
						return HGEJSONRef::UndefinedRef();
					}
					
					// recursively use dereferenced value as substitute
					pointer = &substitute.sub();
					continue;
				} else { // substitute itself is an escape sequence!
					HGEAssertC(0, "escape sequences are unsupported as $ substitute values");
					return HGEJSONRef::UndefinedRef();
				}
			} else { // test for recursion
				HGEJSONRef ref(pointer);
				ref = ref(0); // shortcut the substitute's substitution
				
				if (ref.unset()) {
					HGEAssertC(0, "recursive determination of substitution's shortcut failed");
					return HGEJSONRef::UndefinedRef();
				}
				
				pointer = &ref.sub();
				
				if (!pointer->IsUndefined()) { // if shortcut has a substitution, most resolvable shortcut must have been a fusion or an escape sequence
					if (pointer->IsNumber()) {
						HGEAssertC(pointer->GetUint64() == 0, "substitution's shortcut contained a numeric and non-zero (i.e. non-escape) value: ", pointer->GetUint64());
						HGEAssertC(0, "escape sequences are unsupported as $ substitute values");
						return HGEJSONRef::UndefinedRef();
					}
					
					// treat substitute as a fused fusion list substitution
					return parent;
				} else {
					pointer = ref.stored.value;
					
					if (pointer->IsArray()) { // treat substitute itself as a fused fusion list substitution
						return parent;
					} else if (pointer->IsUint64()) { // treat substitute itself probably as a deref
						id_hge hgeuuid = pointer->GetUint64();
						if (hgeuuid != 0) { // treat substitute itself as a deref
							substitute = parent.subDeref(hgeuuid);
							
							if (substitute.unset()) {
								HGEAssertC(0, "failed to dereference %s: %li", HGE_KEYTEXT_UUID, hgeuuid)
								return HGEJSONRef::UndefinedRef();
							}
							
							// recursively use dereferenced value as substitute
							pointer = &substitute.sub();
							continue;
						} else { // substitute itself is an escape sequence!
							HGEAssertC(0, "escape sequences are unsupported as $ substitute values");
							return HGEJSONRef::UndefinedRef();
						}
					} else {
						HGEAssertC(0, "only fusions (arrays) and dereferences (integers) are supported as substitution values (GetType() == )", pointer->GetType());
						return HGEJSONRef::UndefinedRef();
					}
				}
			}
		}
	}
	
	HGEAssertC(0, "probably just avoided an infinite loop ...");
	return HGEJSONRef::UndefinedRef();
}

HGEJSONRef& HGEJSONRef::subDeref(Value * pointer) {
	if (pointer->IsUint64()) {
		return this->subDeref(pointer->GetUint64());
	}
	
	return UndefinedRef();
}

HGEJSONRef& HGEJSONRef::subDeref(id_hge hgeuuid) {
	HGEEntity * entity = HGEDispatch::EntityWithId(hgeuuid);
	if (entity &&
		entity->HGEModel::hgekindof(HGEKind<HGESurrogate>())) {
		HGESurrogate * target = (HGESurrogate *)entity;
		return target->contents();
	}
	
	return UndefinedRef();
}

HGEJSONRef::Type HGEJSONRef::subType(Value& substitute) {
	if (substitute.IsUint64()) {
		HGEJSONRef& ref = this->subDeref(&substitute);
		Value& s = ref.sub(); // does the substitute have a substitute?
		return !s.IsUndefined() ? ref.subType(s) : ref().GetType();
	} else if (substitute.IsArray()) {
		Index range = substitute.Size();
		// use the object in the array to determine type ... somewhat dangerous and perhaps unintuitive
		// the reasoning is that an empty array or object can be used at the end of the object to specify type
		// when at the end, the empty collection will not impact key/index access into combined collection substitutions
		Value& element = substitute[range - 1];
		// TODO: I have an urge to cover cases where the last object is not a collection, but the next to last is a collection . . .
		// should I default to array type, or walk backwards?
		if (element.IsUint64()) {
			HGEJSONRef& ref = this->subDeref(&element);
			Value& s = ref.sub(); // does the substitute have a substitute?
			Type candidate = !s.IsUndefined() ? ref.subType(s) : ref().GetType();
			if (candidate == vapidjson::kObjectType ||
				candidate == vapidjson::kArrayType) {
				return candidate;
			}
		} else if (element.IsObject() ||
				   element.IsArray()) {
			return element.GetType();
		}
		//return vapidjson::kUndefinedType;
		return vapidjson::kArrayType; // default to array type
	} else { // this can be used to "escape" the special meaning of the JSON_SUBSTITUTION_IDENTIFIER ("$")
		return substitute.GetType();
	}
}

HGEJSONRef::Value& HGEJSONRef::subValue(Value& substitute, Type kind) {
	if (substitute.IsUint64()) {
		HGEJSONRef& ref = this->subDeref(&substitute);
		Value& s = ref.sub(); // does the substitute have a substitute?
		return !s.IsUndefined() ? ref.subValue(s, kind) : ref();
	} else if (substitute.IsArray()) {
		Index range = substitute.Size();
		if (kind == vapidjson::kUndefinedType) {
			kind = this->subType(substitute);
		}
		if (kind != vapidjson::kUndefinedType) {
			for (Index i = 0; i < range; i++) {
				Value& element = substitute[i];
				if (element.GetType() == kind) {
					return element;
				} else if (element.IsUint64()) {
					HGEJSONRef& ref = this->subDeref(&element);
					Value * pointer = &ref.sub(); // does the substitute have a substitute?
					if (!pointer->IsUndefined()) {
						return ref.subValue(*pointer, kind);
					} else if ((pointer = &ref())->GetType() == kind) {
						return *pointer;
					}
				}
			}
			// TODO: default type is now array, so assertion may be inappropriate
			//HGEAssertC(0, "substitute.IsArray() but found no element of subType(%i)", kind); // at least the object from the call to subType() should have been found
		} else {
			HGEAssertC(0, "encountered undefined multi-substitution (this can cause serious problems during traversal)");
		}
	} else {
		// TODO: there is a potential problem here if this value is used to initialize a new HGEJSONRef (because "escaping" may be prevented)
		// Granted, the only place HGEJSONRef::subValue() gets called is for operator()()
		// In practice, when HGEJSONRef a; it is more likely that HGEJSONRef b = a; than HGEJSONRef b(&a()); . . .
		return substitute;
	}
	
	return HGEJSONRef::Undefined();
}

HGEJSONRef::Value& HGEJSONRef::subKey(Value * pointer, const Key* key, bool anykey, Kter * ater) {
	
	HGEJSONRef substitute(pointer);
	
	HGEAssertC(substitute.stored.value, "ref value must be assigned!");
	
	pointer = &substitute.sub();
	
	// TODO: make this less silly -- substitute as passed in was not actually the value opposite the $ in "this" HGEJSONRef
	//substitute.hold(pointer);
	//pointer = &substitute.sub();
	
	if (!pointer->IsUndefined()) { // this is a fusion (based on a fused fusion list)
		substitute = substitute.subShortcut(substitute);
		
		Index j = 0;
		HGEJSONRef ref;
		Index fallback = 0;
		Index& marker = ater ? ater->indexStack.back() : fallback;
		// TODO: implement fusion iterators
		while (!(ref = substitute[j++]).unset()) { // ref is an object from the fusion based on the fused fusion list
			marker++; // TODO: why is this necessary
			ref = ref.getKey(key, anykey, ater); // try to get the keyed element from ref
			
			if (!ref.unset()) {
				return *ref.stored.value;
			}
		}
		
		if (ater) {
			ater->refStack.pop_back();
			ater->indexStack.pop_back();
			ater->mter = Kter::UndefinedMemberIter();
		}
		
		// key not found in fusion
		return HGEJSONRef::Undefined();
	} else {
		pointer = substitute.stored.value;
		
		if (pointer->IsArray()) { // this is a fusion
			Index range = pointer->Size();
			HGEJSONRef ref;
			Index fallback = 0;
			Index& marker = ater ? ater->indexStack.back() : fallback;
			for (Index i = 0; i < range; i++) {
				//HGEAssertC(marker == i, "this should probably be true... and so marker++ should be unnecessary");
				//marker++;
				marker = i;
				ref.hold(&(*pointer)[i]);
				//ref = ref.subShortcut(ref); // TODO: is this necessary?
				ref = ref.getKey(key, anykey, ater); // try to get the keyed element from ref
				
				if (!ref.unset()) {
					return ref();
				}
			}
			
			if (ater) {
				ater->refStack.pop_back();
				ater->indexStack.pop_back();
				ater->mter = Kter::UndefinedMemberIter();
			}
			
			// key not found in fusion
			return HGEJSONRef::Undefined();
		} else if (pointer->IsNumber()) { // this is an escape sequence
			HGEAssertC(pointer->GetUint64() == 0,
					   "what was expected to be an escape sequence was found to have non-zero %s: %li", HGE_KEYTEXT_UUID, pointer->GetUint64());
			pointer = &this->subEscape();
			if (pointer->IsObject()) {
				size_t range = strlen(key);
				for (MemberIter m = pointer->MemberBegin(); m != pointer->MemberEnd(); m++) {
					if (anykey ||
						(memcmp((*m).first.c_str(), key, range * sizeof(Key)) == 0)) {
						if (ater) {
							ater->mter = m;
						}
						return *(*m).second;
					}
				}
				
				if (ater) {
					ater->refStack.pop_back();
					ater->indexStack.pop_back();
					ater->mter = Kter::UndefinedMemberIter();
				}
				
				// key not found in escape sequence
				return HGEJSONRef::Undefined();
			} else {
				HGEAssertC(pointer->IsUndefined(), "tried to use json value of type %i as an object", pointer->GetType());
				return HGEJSONRef::Undefined();
			}
		} else {
			HGEAssertC(0, "only escapes sequences and fusions should be the candidates to call this function");
			return HGEJSONRef::Undefined();
		}
	}
}


HGEJSONRef::Value& HGEJSONRef::subIndex(Value * pointer, Index index, Index& count, Iter * ater) {
	HGEJSONRef substitute(pointer);
	
	HGEAssertC(substitute.stored.value, "ref value must be assigned!");
	
	pointer = &substitute.sub();
	
	// TODO: make this less silly -- substitute as passed in was not actually the value opposite the $ in "this" HGEJSONRef
	//substitute.hold(pointer);
	//pointer = &substitute.sub();
	
	if (!pointer->IsUndefined()) { // this is a fusion (based on a fused fusion list)
		substitute = substitute.subShortcut(substitute);
		
		Index j = 0;
		HGEJSONRef ref;
		Index fallback = 0;
		Index& marker = ater ? ater->indexStack.back() : fallback;
		// TODO: implement fusion iterators
		while (!(ref = substitute[j++]).unset()) { // ref is an object from the fusion based on the fused fusion list
			marker++; // TODO: why is this necessary
			Index size = 0;
			ref = ref.getIndex(index, size, ater); // try to get the indexed element from ref
			
			if (!ref.unset()) {
				return ref();
			}
			
			if (size != UndefinedCount()) {
				index -= size;
			} else {
				if (ater &&
					ater->keyedFound) {
					return ref();
				}
				//return ref();
			}
		}
		
		if (ater) {
			ater->refStack.pop_back();
			ater->indexStack.pop_back();
			ater->vter = Iter::UndefinedValueIter();
		}
		
		// index not found in fusion
		return HGEJSONRef::Undefined();
	} else {
		pointer = substitute.stored.value;
		
		if (pointer->IsArray()) { // this is a fusion
			Index range = pointer->Size();
			HGEJSONRef ref;
			Index fallback = 0;
			Index& marker = ater ? ater->indexStack.back() : fallback;
			for (Index i = 0; i < range; i++) {
				//HGEAssertC(marker == i, "this should probably be true... and so marker++ should be unnecessary");
				//marker++;
				marker = i;
				ref.hold(&(*pointer)[i]);
				//ref = ref.subShortcut(ref); // TODO: is this necessary?
				Index size = 0;
				ref = ref.getIndex(index, size, ater); // try to get the indexed element from ref
				
				if (!ref.unset()) {
					return ref();
				}
				
				if (size != UndefinedCount()) {
					index -= size;
				} else {
					if (ater &&
						ater->keyedFound) {
						return ref();
					}
					//return ref();
				}
			}
			
			if (ater) {
				ater->refStack.pop_back();
				ater->indexStack.pop_back();
				ater->vter = Iter::UndefinedValueIter();
			}
			
			// index not found in fusion
			return HGEJSONRef::Undefined();
		} else if (pointer->IsNumber()) { // this is an escape sequence
			HGEAssertC(pointer->GetUint64() == 0,
					   "what was expected to be an escape sequence was found to have non-zero %s: %li", HGE_KEYTEXT_UUID, pointer->GetUint64());
			pointer = &this->subEscape();
			if (pointer->IsArray()) {
				Index size = pointer->Size();
				count += size;
				if (size > index) {
					for (ValueIter v = pointer->Begin(); v != pointer->End(); v++) {
						if (index-- == 0) {
							pointer = (*v);
							if (ater) {
								ater->vter = v;
							}
							return *pointer;
						}
					}
					HGEAssert(0, "size inconsistency");
					return HGEJSONRef::Undefined();
				}
				
				if (ater) {
					ater->refStack.pop_back();
					ater->indexStack.pop_back();
					ater->vter = Iter::UndefinedValueIter();
				}
				
				// index not found in escape sequence
				return HGEJSONRef::Undefined();
			} else {
				HGEAssertC(pointer->IsUndefined(), "tried to use json value of type %i as an object", pointer->GetType());
				return HGEJSONRef::Undefined();
			}
		} else {
			HGEAssertC(0, "only escapes sequences and fusions should be the candidates to call this function");
			return HGEJSONRef::Undefined();
		}
	}
}

HGEJSON::HGEJSON()
: HGEJSONRef(0)
{
	
}

HGEJSON::HGEJSON(JSONValue& json, HGEToolbox * toolbox)
: HGEJSONRef(0)
{
	this->mimic(json, toolbox);
}

HGEJSON::~HGEJSON() {
	delete this->stored.value;  this->stored.value = 0;
}

HGEJSON& HGEJSON::mimic(JSONValue& json, HGEToolbox * toolbox) {
	return this->mimic(json);
}

HGEJSON& HGEJSON::mimic(JSONValue& json) {
	if (!this->stored.value) {
		this->stored.value = new JSONValue();
	}
	JSONDoc doc;
	
	doc.Reproduce(json, *this->stored.value);
	
	this->hold(this->stored.value);
	
	return (*this);
}

NS_HGE_END
