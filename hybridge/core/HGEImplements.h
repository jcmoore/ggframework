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
template < typename YoungestDef, typename Parent = YoungestDef >
class HGEPublic
: public Parent {
public:
	typedef YoungestDef MagicDerived;
	typedef Parent RealParent;
	typedef Parent MagicConcrete;
	typedef HGEPublic MagicParent;
};

/**
 inheritance modifier only -- youngest definer is the parent
 */
template < typename YoungestDef >
class HGEPublic< YoungestDef, YoungestDef >
: public YoungestDef {};



/**
 type definer and inheritance modifier
 */
template < typename YoungestDef, typename Parent = YoungestDef >
class HGEProtected
: protected Parent {
public:
	typedef YoungestDef MagicDerived;
	typedef Parent RealParent;
	typedef Parent MagicConcrete;
	typedef HGEProtected MagicParent;
};

/**
 inheritance modifier only -- youngest definer is the parent
 */
template < typename YoungestDef >
class HGEProtected< YoungestDef, YoungestDef >
: protected YoungestDef {};



/**
 type definer and inheritance modifier
 */
template < typename YoungestDef, typename Parent = YoungestDef >
class HGEPrivate
: private Parent {
public:
	typedef YoungestDef MagicDerived;
	typedef Parent RealParent;
	typedef Parent MagicConcrete;
	typedef HGEPrivate MagicParent;
};

/**
 inheritance modifier only -- youngest definer is the parent
 */
template < typename YoungestDef >
class HGEPrivate< YoungestDef, YoungestDef >
: private YoungestDef {};





/**
 type definer and inheritance modifier
 */
template < typename YoungestDef, typename Parent = YoungestDef >
class HGEVirtualPublic
: virtual public Parent {
public:
	typedef YoungestDef MagicDerived;
	typedef Parent RealParent;
	typedef Parent MagicConcrete;
	typedef HGEVirtualPublic MagicParent;
};

/**
 inheritance modifier only -- youngest definer is the parent
 */
template < typename YoungestDef >
class HGEVirtualPublic< YoungestDef, YoungestDef >
: virtual public YoungestDef {};



/**
 type definer and inheritance modifier
 */
template < typename YoungestDef, typename Parent = YoungestDef >
class HGEVirtualProtected
: virtual protected Parent {
public:
	typedef YoungestDef MagicDerived;
	typedef Parent RealParent;
	typedef Parent MagicConcrete;
	typedef HGEVirtualProtected MagicParent;
};

/**
 inheritance modifier only -- youngest definer is the parent
 */
template < typename YoungestDef >
class HGEVirtualProtected< YoungestDef, YoungestDef >
: virtual protected YoungestDef {};



/**
 type definer and inheritance modifier
 */
template < typename YoungestDef, typename Parent = YoungestDef >
class HGEVirtualPrivate
: virtual private Parent {
public:
	typedef YoungestDef MagicDerived;
	typedef Parent RealParent;
	typedef Parent MagicConcrete;
	typedef HGEVirtualPrivate MagicParent;
};

/**
 inheritance modifier only -- youngest definer is the parent
 */
template < typename YoungestDef >
class HGEVirtualPrivate< YoungestDef, YoungestDef >
: virtual private YoungestDef {};








template <>
class HGEPublic< void > {
public:
	HGEPublic() {}
	typedef HGEPublic MagicParent;
	typedef HGEPublic MagicConcrete;
};

/**
 type definer only -- no parent (the following templates differ in name only)
 */

template < typename YoungestDef >
class HGEPublic< YoungestDef, HGENone >
: public HGEPublic< void > {
public:
	typedef YoungestDef MagicDerived;
	typedef HGEPublic< void > RealParent;
	typedef typename RealParent::MagicConcrete MagicConcrete;
	typedef HGEPublic MagicParent;
};

template < typename YoungestDef >
class HGEProtected< YoungestDef, HGENone >
: protected HGEPublic< void > {
public:
	typedef YoungestDef MagicDerived;
	typedef HGEPublic< void > RealParent;
	typedef typename RealParent::MagicConcrete MagicConcrete;
	typedef HGEProtected MagicParent;
};

template < typename YoungestDef >
class HGEPrivate< YoungestDef, HGENone >
: private HGEPublic< void > {
public:
	typedef YoungestDef MagicDerived;
	typedef HGEPublic< void > RealParent;
	typedef typename RealParent::MagicConcrete MagicConcrete;
	typedef HGEPrivate MagicParent;
};

template < typename YoungestDef >
class HGEVirtualPublic< YoungestDef, HGENone >
: virtual public HGEPublic< void > {
public:
	typedef YoungestDef MagicDerived;
	typedef HGEPublic< void > RealParent;
	typedef typename RealParent::MagicConcrete MagicConcrete;
	typedef HGEVirtualPublic MagicParent;
};

template < typename YoungestDef >
class HGEVirtualProtected< YoungestDef, HGENone >
: virtual protected HGEPublic< void > {
public:
	typedef YoungestDef MagicDerived;
	typedef HGEPublic< void > RealParent;
	typedef typename RealParent::MagicConcrete MagicConcrete;
	typedef HGEVirtualProtected MagicParent;
};

template < typename YoungestDef >
class HGEVirtualPrivate< YoungestDef, HGENone >
: virtual private HGEPublic< void > {
public:
	typedef YoungestDef MagicDerived;
	typedef HGEPublic< void > RealParent;
	typedef typename RealParent::MagicConcrete MagicConcrete;
	typedef HGEVirtualPrivate MagicParent;
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
