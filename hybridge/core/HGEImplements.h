//
//  HGEImplements.h
//  hybridge
//
//  Created by The Narrator on 11/3/12.
//
//

#ifndef __HGEIMPLEMENTS_H__
#define __HGEIMPLEMENTS_H__

#include "dev/HGEMacros.h"

#include "core/HGEKind.h"
#include "core/HGELike.h"


NS_HGE_BEGIN

#define HGE_IMPOF(i)		HGE_KINDOF(i<>)

#define HGEFunction			boost::function



class HGEPure {
public:
	virtual ~HGEPure() {}
};

#define HGE_IMP_TYPEDEF							MagicDerived

class HGEPrototype {
public:
	virtual ~HGEPrototype() {}
};

template < typename BaseType = void >
class HGEProtoImp
: public BaseType {
public:
	virtual bool does(kind_hge isolatedInterface) {
		return 0;
	}
	
	//typedef DerivedType HGE_IMP_TYPEDEF;
};

template <>
class HGEProtoImp< void > : public HGEProtoImp< HGEPrototype > {};



#define HGE_BOILERPLATE_TYPE_ORIGIN						HGEProtoImp< HGEPrototype >
#define HGE_BOILERPLATE_TYPE_EMPTY						HGENone

#define HGE_BOILERPLATE_TYPENAME_BASE					BaseType
#define HGE_BOILERPLATE_TYPENAME_DERIVED				DerivedType



/**
 type definer and inheritance modifier
 */
template < typename Derived, typename Parent = Derived >
class HGEPublic
: public Parent {
public:
	typedef HGEPublic MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	typedef HGEPublic RealSelf;
};

/**
 type definer and inheritance modifier
 */
template < typename Derived, typename Parent = Derived >
class HGEProtected
: protected Parent {
public:
	typedef HGEProtected MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	typedef HGEProtected RealSelf;
};

/**
 type definer and inheritance modifier
 */
template < typename Derived, typename Parent = Derived >
class HGEPrivate
: private Parent {
public:
	typedef HGEPrivate MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	typedef HGEPrivate RealSelf;
};





/**
 type definer and inheritance modifier
 */
template < typename Derived, typename Parent = Derived >
class HGEVirtualPublic
: virtual public Parent {
public:
	typedef HGEVirtualPublic MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	typedef HGEVirtualPublic RealSelf;
};

/**
 type definer and inheritance modifier
 */
template < typename Derived, typename Parent = Derived >
class HGEVirtualProtected
: virtual protected Parent {
public:
	typedef HGEVirtualProtected MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	typedef HGEVirtualProtected RealSelf;
};

/**
 type definer and inheritance modifier
 */
template < typename Derived, typename Parent = Derived >
class HGEVirtualPrivate
: virtual private Parent {
public:
	typedef HGEVirtualPrivate MagicParent;
	typedef Parent RealParent;
	typedef Derived MagicDerived;
	typedef HGEVirtualPrivate RealSelf;
};





/**
 The following style of imp (and similar macros) are useful when
 defining a "defspace" on which a set of more functional imps rely
 */
template < typename DefType, typename Base = DefType >
class HGETypename_MagicDerived : public Base {
public:
	typedef DefType	MagicDerived;
};

#define HGE_IMP_TYPENAME(tn)	HGE_IMP_TYPENAME_PREFIXED(, tn)

#define HGE_IMP_TYPENAME_PREFIXED(prefix, tn)				\
template < typename DefType, typename Base = DefType >		\
class prefix##tn : public Base {							\
public:														\
	typedef DefType	tn;										\
};










/**
 HGEDeriver enables escaping somewhat limiting conventional sugar that permits (and requires)
 that interfaces only take a single base-type template argument
 */
template < typename BaseType, typename DerivedType >
class HGEDeriver : public BaseType {
public:
	
	typedef DerivedType HGE_IMP_TYPEDEF;
	
};





/**
 use the following examples to create more new interfaces
 */

#define HGE_BOILERPLATE(bpName, bpBase)																				\
;																													\
template < typename BaseType = HGE_BOILERPLATE_TYPE_EMPTY, typename DerivedType = HGE_BOILERPLATE_TYPE_EMPTY >		\
class bpName : public HGEDeriver< bpName< BaseType, HGE_BOILERPLATE_TYPE_EMPTY >, DerivedType > {};					\
;																													\
template < typename bpBase >																						\
class bpName< bpBase, void > : public bpBase



#define HGE_BOILERPLATE_SUGAR_IMP(bpName, bpImp)																	\
;																													\
public:																												\
typedef bpName bpImp;



#define HGE_BOILERPLATE_SUGAR_REAL(bpBase, bpReal)																	\
;																													\
public:																												\
typedef typename bpBase::HGE_IMP_TYPEDEF bpReal;



#define HGE_BOILERPLATE_DEFAULT(bpName)																				\
template <>																											\
class bpName< void, void > : public bpName< HGE_BOILERPLATE_TYPE_ORIGIN, HGE_BOILERPLATE_TYPE_EMPTY > {};			\



#define HGE_BOILERPLATE_SUGAR_DOES(bpBase)																			\
;																													\
protected:																											\
typedef typename bpBase::HGE_IMP_TYPEDEF bpReal;





HGE_BOILERPLATE(HGEImplicitBoilerplate, ArbitraryTemplateParameter)
{
	// sugary typedefs are not mandatory but they sure can help
	HGE_BOILERPLATE_SUGAR_IMP(HGEImplicitBoilerplate, ImpImplicitBoilerplate);
	HGE_BOILERPLATE_SUGAR_REAL(ArbitraryTemplateParameter, RealImplicitBoilerplate);
	
private: // be sure to set access modifiers as necessary
	void saveTimeWithMacros () { HGEPrintln("HGEImplicitBoilerplate!"); }
	
protected:
	RealImplicitBoilerplate * reasoning() {
		ImpImplicitBoilerplate * boilerplate = this;
		boilerplate->saveTimeWithMacros();
		return static_cast<RealImplicitBoilerplate *>(boilerplate);
	}
};



template < typename BaseType = HGE_BOILERPLATE_TYPE_EMPTY, typename DerivedType = HGE_BOILERPLATE_TYPE_EMPTY >
class HGEExplicitBoilerplate
: public HGEDeriver< HGEExplicitBoilerplate< BaseType, HGE_BOILERPLATE_TYPE_EMPTY >, DerivedType > {};

template < typename BaseType >
class HGEExplicitBoilerplate< BaseType, void > : public BaseType
// HGE_BOILERPLATE(HGEExplicitBoilerplate, BaseType)
{
	// sugary typedefs are not mandatory but they sure can help
public:
	typedef HGEExplicitBoilerplate ImpExplicitBoilerplate;
	// HGE_BOILERPLATE_SUGAR_IMP(HGEExplicitBoilerplate, ImpExplicitBoilerplate);
public:
	typedef typename BaseType::HGE_IMP_TYPEDEF
	RealExplicitBoilerplate;
	// HGE_BOILERPLATE_SUGAR_REAL(BaseType, RealDemoBoilerplate);
	
private: // be sure to set access modifiers as necessary
	void saveTimeWithTemplates () { HGEPrintln("HGEExplicitBoilerplate!"); }
	
protected:
	RealExplicitBoilerplate * reasoning() {
		ImpExplicitBoilerplate * boilerplate = this;
		boilerplate->saveTimeWithTemplates();
		return static_cast<RealExplicitBoilerplate *>(boilerplate);
	}
};



#if 0 // demo
class HGEDemoBoilerplate : public
HGEExplicitBoilerplate<
HGEImplicitBoilerplate<
HGEImplementer<
HGEDemoBoilerplate> > > {
	
public:
	HGEDemoBoilerplate() {
		ImpExplicitBoilerplate::reasoning();
		ImpImplicitBoilerplate::reasoning();
		return;
	}
};

static HGEDemoBoilerplate demo;
#endif



NS_HGE_END

#endif
