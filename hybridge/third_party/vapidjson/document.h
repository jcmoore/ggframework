#ifndef VAPIDJSON_DOCUMENT_H_
#define VAPIDJSON_DOCUMENT_H_

#include "reader.h"
#include "internal/strfunc.h"

#include <string>
#include <list>
#include <map>

#include "third_party/vallocator/vallocator.h"

namespace vapidjson {

///////////////////////////////////////////////////////////////////////////////
// GenericValue

//! Represents a JSON value. Use Value for UTF8 encoding and default allocator.
/*!
	A JSON value can be one of 7 types. This class is a variant type supporting
	these types.

	Use the Value if UTF8 and default allocator

	\tparam Encoding	Encoding of the value. (Even non-string values need to have the same encoding in a document)
	\tparam Allocator	Allocator type for allocating memory of object, array and string.
*/
#pragma pack (push, 4)
template <typename Encoding> 
class GenericValue {
public:
	//! Name-value pair in an object.
	//struct Member { 
	//	GenericValue<Encoding, Allocator> name;		//!< name of member (must be a string)
	//	GenericValue<Encoding, Allocator> value;	//!< value of member.
	//};
	
	typedef GenericValue Member;
	
	typedef std::string Text;
	typedef std::list<GenericValue *> VList;
	typedef std::map<Text, GenericValue *> VMap;
	typedef typename VMap::value_type VMapPair;
	
	typedef Encoding EncodingType;						//!< Encoding type from template parameter.
	typedef typename Encoding::Ch Ch;					//!< Character type derived from Encoding.
	//typedef Member* MemberIterator;						//!< Member iterator for iterating in object.
	typedef typename VMap::iterator MemberIterator;		//!< Member iterator for iterating in object.
	typedef const MemberIterator ConstMemberIterator;	//!< Constant member iterator for iterating in object.
	//typedef GenericValue* ValueIterator;				//!< Value iterator for iterating in array.
	typedef typename VList::iterator ValueIterator;		//!< Value iterator for iterating in array.
	typedef const ValueIterator ConstValueIterator;		//!< Constant value iterator for iterating in array.
	typedef VList VStack;
	
	//!@name Constructors and destructor.
	//@{

	//! Default constructor creates a null value.
	GenericValue() : flags_(kNullFlag) {}

	//! Copy constructor is not permitted.
private:
	GenericValue(const GenericValue& rhs);
	
	static MemberIterator const& UndefinedMemberIterator() {
		static MemberIterator UndefinedIterator;
		return UndefinedIterator;
	}
	static ValueIterator const& UndefinedValueIterator() {
		static ValueIterator UndefinedIterator;
		return UndefinedIterator;
	}
	static GenericValue & UndefinedValue() {
		static GenericValue Undefined(kUndefinedType);
		return Undefined;
	}
public:

	//! Constructor with JSON value type.
	/*! This creates a Value of specified type with default content.
		\param type	Type of the value.
		\note Default content for number is zero.
	*/
	GenericValue(Type type) {
		static const unsigned defaultFlags[7] = {
			kNullFlag, kFalseFlag, kTrueFlag, kObjectFlag, kArrayFlag, kCopyStringFlag, //kConstStringFlag, // TODO: not so sure about swapping kCopyStringFlag for kConstStringFlag...
			kNumberFlag | kIntFlag | kUintFlag | kInt64Flag | kUint64Flag | kDoubleFlag
		};
		//VAPIDJSON_ASSERT(type <= kNumberType);
		//flags_ = defaultFlags[type];
		VAPIDJSON_ASSERT(type <= kNumberType || (type == kUndefinedType));
		flags_ = (type != kUndefinedType) ? defaultFlags[type] : kUndefinedFlag;
		memset(&data_, 0, sizeof(data_));
	}

	//! Constructor for boolean value.
	GenericValue(bool b) : flags_(b ? kTrueFlag : kFalseFlag) {}

	//! Constructor for int value.
	GenericValue(int i) : flags_(kNumberIntFlag) { 
		data_.n.i64 = i;
		if (i >= 0)
			flags_ |= kUintFlag | kUint64Flag;
	}

	//! Constructor for unsigned value.
	GenericValue(unsigned u) : flags_(kNumberUintFlag) {
		data_.n.u64 = u; 
		if (!(u & 0x80000000))
			flags_ |= kIntFlag | kInt64Flag;
	}

	//! Constructor for int64_t value.
	GenericValue(int64_t i64) : flags_(kNumberInt64Flag) {
		data_.n.i64 = i64;
		if (i64 >= 0) {
			flags_ |= kNumberUint64Flag;
			if (!(i64 & 0xFFFFFFFF00000000LL))
				flags_ |= kUintFlag;
			if (!(i64 & 0xFFFFFFFF80000000LL))
				flags_ |= kIntFlag;
		}
		else if (i64 >= -2147483648LL)
			flags_ |= kIntFlag;
	}

	//! Constructor for uint64_t value.
	GenericValue(uint64_t u64) : flags_(kNumberUint64Flag) {
		data_.n.u64 = u64;
		if (!(u64 & 0x8000000000000000ULL))
			flags_ |= kInt64Flag;
		if (!(u64 & 0xFFFFFFFF00000000ULL))
			flags_ |= kUintFlag;
		if (!(u64 & 0xFFFFFFFF80000000ULL))
			flags_ |= kIntFlag;
	}

	//! Constructor for double value.
	GenericValue(double d) : flags_(kNumberDoubleFlag) { data_.n.d = d; }

	//! Constructor for copy-string (i.e. do make a copy of string)
	GenericValue(const Ch* s) { 
		VAPIDJSON_ASSERT(s != NULL);
		flags_ = kCopyStringFlag;
		data_.s.str = new std::string(s);
	}

	//! Destructor.
	/*! Need to destruct elements of array, members of object, or copy-string.
	*/
	~GenericValue() {
		switch(flags_) {
			case kArrayFlag:
				Clear();
				delete data_.a.elements;
				break;
				
			case kObjectFlag:
				for (MemberIterator i = MemberBegin(); i != MemberEnd(); i++) {
					delete (*i).second;
					VAPIDJSON_ASSERT(data_.o.size--);
				}
				VAPIDJSON_ASSERT(data_.o.size == 0);
				delete data_.o.members;
				VAPIDJSON_ASSERT(0 == data_.o.size);
				break;
				
			case kCopyStringFlag:
				delete data_.s.str;
				break;
		}
		flags_ = kNullFlag;
	}

	//@}

	//!@name Assignment operators
	//@{

	//! Assignment with move semantics.
	/*! \param rhs Source of the assignment. It will become a null value after assignment.
	*/
	GenericValue& operator=(GenericValue& rhs) {
		VAPIDJSON_ASSERT(this != &rhs);
		this->~GenericValue();
		memcpy(this, &rhs, sizeof(GenericValue));
		rhs.flags_ = kNullFlag;
		return *this;
	}

	//! Assignment with primitive types.
	/*! \tparam T Either Type, int, unsigned, int64_t, uint64_t, const Ch*
		\param value The value to be assigned.
	*/
	template <typename T>
	GenericValue& operator=(T value) {
		this->~GenericValue();
		::new (this) GenericValue(value);
		return *this;
	}
	//@}

	//!@name Type
	//@{

	Type GetType()	const { return static_cast<Type>(flags_ & kTypeMask); }
	bool IsTruthy()	const { return (flags_ & (kUndefinedFlag | kNullFlag | kFalseFlag)) != 0; }
	bool IsUndefined()	const { return flags_ == kUndefinedFlag; }
	bool IsNull()	const { return flags_ == kNullFlag; }
	bool IsFalse()	const { return flags_ == kFalseFlag; }
	bool IsTrue()	const { return flags_ == kTrueFlag; }
	bool IsBool()	const { return (flags_ & kBoolFlag) != 0; }
	bool IsObject()	const { return flags_ == kObjectFlag; }
	bool IsArray()	const { return flags_ == kArrayFlag; }
	bool IsNumber() const { return (flags_ & kNumberFlag) != 0; }
	bool IsInt()	const { return (flags_ & kIntFlag) != 0; }
	bool IsUint()	const { return (flags_ & kUintFlag) != 0; }
	bool IsInt64()	const { return (flags_ & kInt64Flag) != 0; }
	bool IsUint64()	const { return (flags_ & kUint64Flag) != 0; }
	bool IsDouble() const { return (flags_ & kDoubleFlag) != 0; }
	bool IsString() const { return (flags_ & kStringFlag) != 0; }

	//@}

	//!@name Null
	//@{

	GenericValue& SetNull() { this->~GenericValue(); ::new (this) GenericValue(); return *this; }

	//@}

	//!@name Bool
	//@{

	bool GetBool() const { VAPIDJSON_ASSERT(IsBool()); return flags_ == kTrueFlag; }
	GenericValue& SetBool(bool b) { this->~GenericValue(); ::new (this) GenericValue(b); return *this; }

	//@}

	//!@name Object
	//@{

	//! Set this value as an empty object.
	GenericValue& SetObject() { this->~GenericValue(); ::new (this) GenericValue(kObjectType); return *this; }
	
	//! Get the number of kvps in object.
	SizeType Count() const { VAPIDJSON_ASSERT(IsObject()); return data_.o.size; }

	//! Get the value associated with the object's name.
	GenericValue& operator[](const Ch* name) {
		if (Member* member = FindMember(name)) {
			return *member;
		} else {
			return UndefinedValue();
			static GenericValue NullValue;
			return NullValue;
		}
	}
	const GenericValue& operator[](const Ch* name) const { return const_cast<GenericValue&>(*this)[name]; }
	
	////! Get the value associated with the object's name.
	//GenericValue& operator()() {
	//	VAPIDJSON_ASSERT(IsObject());
	//	if (data_.o.size > 0)
	//		return (data_.o.members + data_.o.size - 1)->value;
	//	else {
	//		static GenericValue UndefinedValue(kUndefinedType);
	//		return UndefinedValue;
	//		static GenericValue NullValue;
	//		return NullValue;
	//	}
	//}
	//const GenericValue& operator()() const { return const_cast<GenericValue&>(*this)(); }

	//! Member iterators.
	MemberIterator MemberBegin()			{ VAPIDJSON_ASSERT(IsObject()); return data_.o.members ? data_.o.members->begin() : UndefinedMemberIterator(); }
	MemberIterator MemberEnd()				{ VAPIDJSON_ASSERT(IsObject()); return data_.o.members ? data_.o.members->end() : UndefinedMemberIterator(); }
	ConstMemberIterator MemberBegin() const	{ return const_cast<GenericValue&>(*this).MemberBegin(); }
	ConstMemberIterator MemberEnd()	const	{ return const_cast<GenericValue&>(*this).MemberEnd(); }

	//! Check whether a member exists in the object.
	bool HasMember(const Ch* name) const { return FindMember(name) != 0; }

	//! Add a member (name-value pair) to the object.
	/*! \param name A string value as name of member.
		\param value Value of any type.
	    \param allocator Allocator for reallocating memory.
	    \return The value itself for fluent API.
	    \note The ownership of name and value will be transfered to this object if success.
	*/
	GenericValue& AddMember(const Ch* name, GenericValue& value) {
		VAPIDJSON_ASSERT(IsObject());
		Object& o = data_.o;
		if (!o.members) {
			o.members = new VMap();
		}
		GenericValue *& e = (*o.members)[name];
		if (e) {
			delete e;
		}
		
		GenericValue * m = new GenericValue();
		(*m)=value;
		e = m;
		
		o.size++;
		return (*m);
		//return *this;
	}

	template <typename T>
	GenericValue& AddMember(const Ch* name, T value) {
		GenericValue v(value);
		return AddMember(name, v);
	}

	//! Remove a member in object by its name.
	/*! \param name Name of member to be removed.
	    \return Whether the member existed.
	    \note Removing member is implemented by moving the last member. So the ordering of members is changed.
	*/
	bool RemoveMember(const Ch* name) {
		VAPIDJSON_ASSERT(IsObject());
		if (!data_.o.members) {
			return false;
		}
		MemberIterator iter = data_.o.members->find(name);
		if (iter != MemberEnd()) {
			VAPIDJSON_ASSERT(data_.o.size > 0);
			
			delete (*iter).second;
			
			data_.o.members->erase(iter);
			
			--data_.o.size;
			return true;
		}
		return false;
	}

	//@}

	//!@name Array
	//@{

	//! Set this value as an empty array.
	GenericValue& SetArray() {	this->~GenericValue(); ::new (this) GenericValue(kArrayType); return *this; }

	//! Get the number of elements in array.
	SizeType Size() const { VAPIDJSON_ASSERT(IsArray()); return data_.a.size; }

	////! Get the capacity of array.
	//SizeType Capacity() const { VAPIDJSON_ASSERT(IsArray()); return data_.a.capacity; }

	//! Check whether the array is empty.
	//bool Empty() const { VAPIDJSON_ASSERT(IsArray()); return data_.a.size == 0; }
	bool Empty() const { VAPIDJSON_ASSERT(IsArray()); return !data_.a.elements || data_.a.elements->empty(); }

	//! Remove all elements in the array.
	/*! This function do not deallocate memory in the array, i.e. the capacity is unchanged.
	*/
	void Clear() {
		VAPIDJSON_ASSERT(IsArray()); 
		for (ValueIterator i = Begin(); i != End(); i++) {
			VAPIDJSON_ASSERT(data_.a.size--);
			delete (*i);
		}
		VAPIDJSON_ASSERT(data_.a.size == 0);
		if (data_.a.elements) {
			data_.a.elements->clear();
		}
	}

	//! Get an element from array by index.
	/*! \param index Zero-based index of element.
		\note
\code
Value a(kArrayType);
a.PushBack(123);
int x = a[0].GetInt();				// Error: operator[ is ambiguous, as 0 also mean a null pointer of const char* type.
int y = a[SizeType(0)].GetInt();	// Cast to SizeType will work.
int z = a[0u].GetInt();				// This works too.
\endcode
	*/
	GenericValue& operator[](SizeType index) {
		VAPIDJSON_ASSERT(IsArray());
		// developer modified
		//VAPIDJSON_ASSERT(index < data_.a.size);
		if (index >= data_.a.size) {
			return UndefinedValue();
		}
		VAPIDJSON_ASSERT(data_.a.elements);
		for (ValueIterator i = Begin(); i != End(); i++) {
			if (index-- == 0) {
				return *(*i);
			}
		}
		VAPIDJSON_ASSERT(0); // size mismatch
		return UndefinedValue();
	}
	const GenericValue& operator[](SizeType index) const { return const_cast<GenericValue&>(*this)[index]; }

	//! Element iterator
	ValueIterator Begin() { VAPIDJSON_ASSERT(IsArray()); return data_.a.elements ? data_.a.elements->begin() : UndefinedValueIterator(); }
	ValueIterator End() { VAPIDJSON_ASSERT(IsArray()); return data_.a.elements ? data_.a.elements->end() : UndefinedValueIterator(); }
	ConstValueIterator Begin() const { return const_cast<GenericValue&>(*this).Begin(); }
	ConstValueIterator End() const { return const_cast<GenericValue&>(*this).End(); }
	//! Request the array to have enough capacity to store elements.
	/*! \param newCapacity	The capacity that the array at least need to have.
		\param allocator	The allocator for allocating memory. It must be the same one use previously.
		\return The value itself for fluent API.
	*/
	//GenericValue& Reserve(SizeType newCapacity, Allocator &allocator) {
	//	VAPIDJSON_ASSERT(IsArray());
	//	if (newCapacity > data_.a.capacity) {
	//		data_.a.elements = (GenericValue*)allocator.Realloc(data_.a.elements, data_.a.capacity * sizeof(GenericValue), newCapacity * sizeof(GenericValue));
	//		data_.a.capacity = newCapacity;
	//	}
	//	return *this;
	//}
	
	//! Append a value at the end of the array.
	/*! \param value		The value to be appended.
	 \param allocator	The allocator for allocating memory. It must be the same one use previously.
	 \return The value itself for fluent API.
	 \note The ownership of the value will be transfered to this object if success.
	 \note If the number of elements to be appended is known, calls Reserve() once first may be more efficient.
	 */
	GenericValue& PushFront(GenericValue& value) {
		VAPIDJSON_ASSERT(IsArray());
		if (!data_.a.elements) {
			data_.a.elements = new VList();
		}
		GenericValue * e = new GenericValue();
		(*e) = value;
		data_.a.elements->push_front(e);
		data_.a.size++;
		return (*e);
		//return *this;
	}
	
	template <typename T>
	GenericValue& PushFront(T value) {
		GenericValue v(value);
		return PushFront(v);
	}
	
	//! Remove the last element in the array.
	GenericValue& PopFront() {
		VAPIDJSON_ASSERT(IsArray());
		VAPIDJSON_ASSERT(!Empty());
		if (!data_.a.elements) {
			return UndefinedValue();
		}
		GenericValue * e = data_.a.elements->front();
		delete e;
		data_.a.elements->pop_front();
		data_.a.size--;
		return *this;
	}
	
	//! Append a value at the end of the array.
	/*! \param value		The value to be appended.
	 \param allocator	The allocator for allocating memory. It must be the same one use previously.
	 \return The value itself for fluent API.
	 \note The ownership of the value will be transfered to this object if success.
	 \note If the number of elements to be appended is known, calls Reserve() once first may be more efficient.
	 */
	GenericValue& PushBack(GenericValue& value) {
		VAPIDJSON_ASSERT(IsArray());
		if (!data_.a.elements) {
			data_.a.elements = new VList();
		}
		GenericValue * e = new GenericValue();
		(*e) = value;
		data_.a.elements->push_back(e);
		data_.a.size++;
		return (*e);
		//return *this;
	}
	
	template <typename T>
	GenericValue& PushBack(T value) {
		GenericValue v(value);
		return PushBack(v);
	}
	
	//! Remove the last element in the array.
	GenericValue& PopBack() {
		VAPIDJSON_ASSERT(IsArray());
		VAPIDJSON_ASSERT(!Empty());
		if (!data_.a.elements) {
			return UndefinedValue();
		}
		GenericValue * e = data_.a.elements->back();
		delete e;
		data_.a.elements->pop_back();
		data_.a.size--;
		return *this;
	}
	
#define VAPIDJSON_GET_ELEMENTS(value, ...) (value).GetElementsUnsafe(__VA_ARGS__, 0)
	SizeType GetElementsUnsafe ( GenericValue const ** first, ...) const
	{
		SizeType count = 0;
		
		VAPIDJSON_ASSERT(IsArray());
		
		ValueIterator iter = Begin();
		
		GenericValue const ** e;
		va_list vl;
		
		e = first;
		
		va_start(vl,first);
		
		while (e && iter != End()) {
			*e = (*iter);
			iter++;
			count++;
			e = va_arg(vl, GenericValue const **);
		}
		
		va_end(vl);
		
		return count;
	}
	
	//@}

	//!@name Number
	//@{

	int GetInt() const			{ VAPIDJSON_ASSERT(flags_ & kIntFlag);   return data_.n.i32.i;   }
	unsigned GetUint() const	{ VAPIDJSON_ASSERT(flags_ & kUintFlag);  return data_.n.u32.u;   }
	int64_t GetInt64() const	{ VAPIDJSON_ASSERT(flags_ & kInt64Flag); return data_.n.i64; }
	int64_t GetUint64() const	{ VAPIDJSON_ASSERT(flags_ & kInt64Flag); return data_.n.u64; }

	double GetDouble() const {
		VAPIDJSON_ASSERT(IsNumber());
		if ((flags_ & kDoubleFlag) != 0)				return data_.n.d;	// exact type, no conversion.
		if ((flags_ & kIntFlag) != 0)					return data_.n.i32.i;	// int -> double
		if ((flags_ & kUintFlag) != 0)					return data_.n.u32.u;	// unsigned -> double
		if ((flags_ & kInt64Flag) != 0)					return (double)data_.n.i64; // int64_t -> double (may lose precision)
		VAPIDJSON_ASSERT((flags_ & kUint64Flag) != 0);	return (double)data_.n.u64;	// uint64_t -> double (may lose precision)
	}

	GenericValue& SetInt(int i)				{ this->~GenericValue(); ::new (this) GenericValue(i);	return *this; }
	GenericValue& SetUint(unsigned u)		{ this->~GenericValue(); ::new (this) GenericValue(u);	return *this; }
	GenericValue& SetInt64(int64_t i64)		{ this->~GenericValue(); ::new (this) GenericValue(i64);	return *this; }
	GenericValue& SetUint64(uint64_t u64)	{ this->~GenericValue(); ::new (this) GenericValue(u64);	return *this; }
	GenericValue& SetDouble(double d)		{ this->~GenericValue(); ::new (this) GenericValue(d);	return *this; }

	//@}

	//!@name String
	//@{

	const Ch* GetString() const { VAPIDJSON_ASSERT(IsString()); return data_.s.str->c_str(); }

	//! Get the length of string.
	/*! Since vapidjson permits "\u0000" in the json string, strlen(v.GetString()) may not equal to v.GetStringLength().
	*/
	SizeType GetStringLength() const { VAPIDJSON_ASSERT(IsString()); return data_.s.str->length(); }

	//! Set this value as a string by copying from source string.
	/*!	\param s source string. 
		\return The value itself for fluent API.
	*/
	GenericValue& SetString(const Ch* s) { this->~GenericValue(); SetStringRaw(s); return *this; }

	//@}

	//! Generate events of this value to a Handler.
	/*! This function adopts the GoF visitor pattern.
		Typical usage is to output this JSON value as JSON text via Writer, which is a Handler.
		It can also be used to deep clone this value via GenericDocument, which is also a Handler.
		\tparam Handler type of handler.
		\param handler An object implementing concept Handler.
	*/
	template <typename Handler>
	GenericValue& Accept(Handler& handler) {
		switch(GetType()) {
		case kUndefinedType:VAPIDJSON_ASSERT(0); handler.Null(); break;
		case kNullType:		handler.Null(); break;
		case kFalseType:	handler.Bool(false); break;
		case kTrueType:		handler.Bool(true); break;

		case kObjectType:
			handler.StartObject();
			if (data_.o.members) {
				for (MemberIterator m = data_.o.members->begin(); m != data_.o.members->end(); ++m) {
					handler.String(m->first.c_str(), m->first.length(), false);
					m->second->Accept(handler);
				}
			}
			handler.EndObject(data_.o.size);
			break;

		case kArrayType:
			handler.StartArray();
			if (data_.a.elements) {
				for (ValueIterator v = data_.a.elements->begin(); v != data_.a.elements->end(); ++v) {
					(*v)->Accept(handler);
				}
			}
			handler.EndArray(data_.a.size);
			break;

		case kStringType:
			handler.String(data_.s.str->c_str(), data_.s.str->length(), false);
			break;

		case kNumberType:
			if (IsInt())			handler.Int(data_.n.i32.i);
			else if (IsUint())		handler.Uint(data_.n.u32.u);
			else if (IsInt64())		handler.Int64(data_.n.i64);
			else if (IsUint64())	handler.Uint64(data_.n.i64);
			else					handler.Double(data_.n.d);
			break;
		}
		return *this;
	}

private:
	template <typename>
	friend class GenericDocument;

	enum {
		kBoolFlag = 0x100,
		kNumberFlag = 0x200,
		kIntFlag = 0x400,
		kUintFlag = 0x800,
		kInt64Flag = 0x1000,
		kUint64Flag = 0x2000,
		kDoubleFlag = 0x4000,
		kStringFlag = 0x100000,
		kCopyFlag = 0x200000,
		kUndefinedFlag = 0x400000,
		kFieldFlag = 0x800000,

		// Initial flags of different types.
		kNullFlag = kNullType,
		kTrueFlag = kTrueType | kBoolFlag,
		kFalseFlag = kFalseType | kBoolFlag,
		kNumberIntFlag = kNumberType | kNumberFlag | kIntFlag | kInt64Flag,
		kNumberUintFlag = kNumberType | kNumberFlag | kUintFlag | kUint64Flag,
		kNumberInt64Flag = kNumberType | kNumberFlag | kInt64Flag,
		kNumberUint64Flag = kNumberType | kNumberFlag | kUint64Flag,
		kNumberDoubleFlag = kNumberType | kNumberFlag | kDoubleFlag,
		kConstStringFlag = kStringType | kStringFlag,
		kCopyStringFlag = kStringType | kStringFlag | kCopyFlag,
		kObjectFlag = kObjectType,
		kArrayFlag = kArrayType,

		kTypeMask = 0xFF	// bitwise-and with mask of 0xFF can be optimized by compiler
	};

	static const SizeType kDefaultArrayCapacity = 16;
	static const SizeType kDefaultObjectCapacity = 16;
	
	// TODO: typedef std::string somewhere in hge:: and make json more string friendly (esp. in terms of copying)
	struct String {
		std::string * str;
		//GenericValue<Encoding, Allocator>* paired; // TODO: use with potential 1-key object optimization
		//const Ch * str;
		//SizeType length;
		//unsigned hashcode;	//!< reserved
	};	// 12 bytes in 32-bit mode, 16 bytes in 64-bit mode

	// By using proper binary layout, retrieval of different integer types do not need conversions.
	union Number {
#if VAPIDJSON_ENDIAN == VAPIDJSON_LITTLEENDIAN
		struct INumber {
			int i;
			char padding[4];
		};
		struct UNumber {
			unsigned u;
			char padding2[4];
		};
#else
		struct INumber {
			char padding[4];
			int i;
		};
		struct UNumber {
			char padding2[4];
			unsigned u;
		};
#endif
		INumber i32;
		UNumber u32;
		int64_t i64;
		uint64_t u64;
		double d;
	};	// 8 bytes

	struct Object {
		//Member* members;
		VMap* members;
		SizeType size;
		//SizeType capacity;
	};	// 12 bytes in 32-bit mode, 16 bytes in 64-bit mode

	struct Array {
		VList* elements;
		SizeType size;
		//SizeType capacity;
	};	// 12 bytes in 32-bit mode, 16 bytes in 64-bit mode

	union Data {
		String s;
		Number n;
		Object o;
		Array a;
	};	// 12 bytes in 32-bit mode, 16 bytes in 64-bit mode

	//! Find member by name.
	Member* FindMember(const Ch* name) {
		VAPIDJSON_ASSERT(name);
		VAPIDJSON_ASSERT(IsObject());

		Object& o = data_.o;
		MemberIterator iter = o.members->find(name);
		
		if (iter != MemberEnd()) {
			return (*iter).second;
		}
		
		return 0;
	}
	const Member* FindMember(const Ch* name) const { return const_cast<GenericValue&>(*this).FindMember(name); }

	// Initialize this value as array with initial data, without calling destructor.
	void SetArrayRaw(VStack * stack, SizeType count) {
		flags_ = kArrayFlag;
		VAPIDJSON_ASSERT(!data_.a.elements);
		data_.a.elements = new VList();
		for (SizeType i = 0; i < count; i++) {
			GenericValue * e = stack->back();
			data_.a.elements->push_front(e);
			stack->pop_back();
		}
		data_.a.size = count;
	}

	//! Initialize this value as object with initial data, without calling destructor.
	void SetObjectRaw(VStack * stack, SizeType count) {
		flags_ = kObjectFlag;
		VAPIDJSON_ASSERT(!data_.o.members);
		data_.o.members = new VMap();
		for (SizeType i = 0; i < count; i++) {
			GenericValue * v = stack->back();
			stack->pop_back();
			GenericValue * k = stack->back();
			stack->pop_back();
			data_.o.members->insert(VMapPair(*(k->data_.s.str), v));
			delete k;
			k = 0;
		}
		data_.o.size = count;
	}

	//! Initialize this value as copy string with initial data, without calling destructor.
	void SetStringRaw(const Ch* s) {
		VAPIDJSON_ASSERT(s != NULL);
		flags_ = kCopyStringFlag;
		data_.s.str->assign(s);
	}

	//! Assignment without calling destructor
	void RawAssign(GenericValue& rhs) {
		memcpy(this, &rhs, sizeof(GenericValue));
		rhs.flags_ = kNullFlag;
	}
	
	Data data_;
	unsigned flags_;
	
	inline static TypeAllocator<GenericValue>& Mallocator() {
		static TypeAllocator<GenericValue> UberMallocator(0, 64 * 1024);
		return UberMallocator;
	}
	
public:
	void* operator new (size_t) {
		return Mallocator().allocate();
	}
	void operator delete (void* storage) {
		Mallocator().deallocate(storage);
	}
};
#pragma pack (pop)

//! Value with UTF8 encoding.
typedef GenericValue<UTF8<> > Value;

///////////////////////////////////////////////////////////////////////////////
// GenericDocument 

//! A document for parsing JSON text as DOM.
/*!
	\implements Handler
	\tparam Encoding encoding for both parsing and string storage.
	\tparam Alloactor allocator for allocating memory for the DOM, and the stack during parsing.
*/
template <typename Encoding >
class GenericDocument {
public:
	typedef typename Encoding::Ch Ch;						//!< Character type derived from Encoding.
	typedef GenericValue<Encoding> ValueType;				//!< Value type of the document.

	//! Constructor
	/*! \param allocator		Optional allocator for allocating stack memory.
		\param stackCapacity	Initial capacity of stack in bytes.
	*/
	GenericDocument() : stack_(), parseError_(0), errorOffset_(0) , output(0) {}
	
	~GenericDocument() { Cleanup(); }
	
	GenericDocument& Deliver (ValueType& onto) { // developer added
		if (!stack_.empty()) {
			AcquireValue();
		}
		if (this->output) {
			//onto.~GenericValue();
			//onto.data_ = this->output->data_;
			//onto.flags_ = this->output->flags_;
			//this->output->flags_ = ValueType::kNullFlag;
			onto = *(this->output);
			delete this->output;
			this->output = 0;
		}
		return (*this);
	}
	
	GenericDocument& Reproduce (ValueType& original, ValueType& duplicate) { // developer added
		original.Accept(*this);
		AcquireValue();
		Deliver(duplicate);
		return (*this);
	}
	
	ValueType& Imitate (ValueType& original) { // developer added
		original.Accept(*this);
		AcquireValue();
		return (*this->output);
	}
	
	//! Parse JSON text from an input stream.
	/*! \tparam parseFlags Combination of ParseFlag.
		\param stream Input stream to be parsed.
		\return The document itself for fluent API.
	*/
	template <unsigned parseFlags, typename SourceEncoding, typename InputStream>
	GenericDocument& ParseStream(InputStream& is) {
		GenericReader<SourceEncoding, Encoding> reader;
		if (reader.template Parse<parseFlags, InputStream, GenericDocument>(is, *this)) {
			AcquireValue();
		}
		else {
			parseError_ = reader.GetParseError();
			errorOffset_ = reader.GetErrorOffset();
			Cleanup();
		}
		return *this;
	}

	//! Parse JSON text from a mutable string.
	/*! \tparam parseFlags Combination of ParseFlag.
		\param str Mutable zero-terminated string to be parsed.
		\return The document itself for fluent API.
	*/
	template <unsigned parseFlags, typename SourceEncoding>
	GenericDocument& ParseInsitu(Ch* str) {
		GenericInsituStringStream<Encoding> s(str);
		return ParseStream<parseFlags | kParseInsituFlag, SourceEncoding>(s);
	}

	template <unsigned parseFlags>
	GenericDocument& ParseInsitu(Ch* str) {
		return ParseInsitu<parseFlags, Encoding>(str);
	}

	//! Parse JSON text from a read-only string.
	/*! \tparam parseFlags Combination of ParseFlag (must not contain kParseInsituFlag).
		\param str Read-only zero-terminated string to be parsed.
	*/
	template <unsigned parseFlags, typename SourceEncoding>
	GenericDocument& Parse(const Ch* str) {
		VAPIDJSON_ASSERT(!(parseFlags & kParseInsituFlag));
		GenericStringStream<SourceEncoding> s(str);
		return ParseStream<parseFlags, SourceEncoding>(s);
	}

	template <unsigned parseFlags>
	GenericDocument& Parse(const Ch* str) {
		return Parse<parseFlags, Encoding>(str);
	}

	//! Whether a parse error was occured in the last parsing.
	bool HasParseError() const { 
		return parseError_ != 0;
	}

	//! Get the message of parsing error.
	const char* GetParseError() const { return parseError_; }

	//! Get the offset in character of the parsing error.
	size_t GetErrorOffset() const { return errorOffset_; }
	
	ValueType& GetValue() { GuardOutput(); return (*this->output); }

//private:
	//friend class GenericReader<Encoding>;	// for Reader to call the following private handler functions

	// Implementation of Handler
	void Null()	{ stack_.push_back(new ValueType()); }
	void Bool(bool b) { stack_.push_back(new ValueType(b)); }
	void Int(int i) { stack_.push_back(new ValueType(i)); }
	void Uint(unsigned i) { stack_.push_back(new ValueType(i)); }
	void Int64(int64_t i) { stack_.push_back(new ValueType(i)); }
	void Uint64(uint64_t i) { stack_.push_back(new ValueType(i)); }
	void Double(double d) { stack_.push_back(new ValueType(d)); }

	void String(const Ch* str, SizeType length, bool copy) { 
		if (copy) 
			stack_.push_back(new ValueType(str));
		else
			stack_.push_back(new ValueType(str));
	}

	void StartObject() {  }
	
	void EndObject(SizeType memberCount) {
		ValueType * obj = new  ValueType(kObjectType);
		obj->SetObjectRaw(&stack_, memberCount);
		stack_.push_back(obj);
	}

	void StartArray() {  }
	
	void EndArray(SizeType elementCount) {
		ValueType * arr = new ValueType(kArrayType);
		arr->SetArrayRaw(&stack_, elementCount);
		stack_.push_back(arr);
	}
private:
	void ClearStack() {
		;
		for (typename ValueType::VStack::iterator iter = stack_.begin(); iter != stack_.end(); iter++) {
			delete (*iter);
		}
	}
	
	void Cleanup() {
		ClearStack();
		delete this->output;
		this->output = 0;
	}
	
	inline void AcquireValue() {
		VAPIDJSON_ASSERT(stack_.size()); // Got one and only one root object
		VAPIDJSON_ASSERT(stack_.size() == 1); // Got one and only one root object
		delete this->output;
		this->output = stack_.back();
		stack_.pop_back();
		parseError_ = 0;
		errorOffset_ = 0;
		VAPIDJSON_ASSERT(this->output);
	}
	inline void GuardOutput() { 
		if (!stack_.empty()) {
			AcquireValue();
		}
		if (!this->output) { 
			this->output = new ValueType(kUndefinedType);
		}
	}

	//static const size_t kDefaultStackCapacity = 1024;
	typename ValueType::VStack stack_;
	const char* parseError_;
	size_t errorOffset_;
	ValueType * output;
};

typedef GenericDocument<UTF8<> > Document;

} // namespace vapidjson

#endif // VAPIDJSON_DOCUMENT_H_
