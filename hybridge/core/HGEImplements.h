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

NS_HGE_BEGIN



#define HGE_BOILERPLATE_TYPEDEF_DERIVED		Derived



typedef void HGENothing;

class HGEPure {
public:
	virtual ~HGEPure() {}
};

class HGEBasis {
public:
	virtual ~HGEBasis() {}
	virtual bool does(kind_hge isolatedInterface) {
		// consider instead a parallel system to HGEKind for interface types (HGEImp)
		// during interface list traversal, common interfaces should all assign the same
		// imp_hge as the fully derived individual interface
		// (i.e. imp_hge(HGEImp < HGEChip < HGEEntity > >()) == imp_hge(HGEImp < HGECircuit < HGEChip < HGEEntity > > >())
		return 0;
	}
	
	// TODO: try to avoid this typedef
protected:
	typedef HGEBasis HGE_BOILERPLATE_TYPEDEF_DERIVED;
};

// NOTE: the usual < base, derived > conventional order is reversed here to sweeten syntax
// this to match the inherited class definition convetion of "class subclass : public superclass {}"
template < typename DerivedType, typename BaseType = HGEBasis >
// NOTE: virtual inheritance!
// this to permit the derived type to directly initialize it's non-direct base class of the base type
class HGEImplementer : virtual public BaseType {
	
public:
	virtual ~HGEImplementer() { HGEAssertC(static_cast<HGEBasis *>(this), ""); }
	
	typedef DerivedType HGE_BOILERPLATE_TYPEDEF_DERIVED;
	
};

/**
 HGEDeriver enables escaping somewhat limiting conventional sugar that permits (and requires)
 that interfaces only take a single base-type template argument
 */
template < typename BaseType, typename DerivedType >
class HGEDeriver : public BaseType {
public:
	
	typedef DerivedType HGE_BOILERPLATE_TYPEDEF_DERIVED;
	
};




/**
 use the following examples to create more new interfaces
 */

#define HGE_BOILERPLATE(bpName, bpBase)																	\
;																										\
template < typename BaseType, typename DerivedType = HGENothing >										\
class bpName : public HGEDeriver< bpName< BaseType, HGENothing >, DerivedType > {};						\
;																										\
template < typename bpBase >																			\
class bpName< bpBase, void > : public bpBase



#define HGE_BOILERPLATE_SUGAR_IMP(bpName, bpImp)														\
;																										\
public:																									\
typedef bpName bpImp;



#define HGE_BOILERPLATE_SUGAR_REAL(bpBase, bpReal)														\
;																										\
public:																									\
typedef typename bpBase::HGE_BOILERPLATE_TYPEDEF_DERIVED bpReal;



#define HGE_BOILERPLATE_SUGAR_DOES(bpBase)																\
;																										\
protected:																								\
typedef typename bpBase::HGE_BOILERPLATE_TYPEDEF_DERIVED bpReal;





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



template < typename BaseType, typename DerivedType = HGENothing >
class HGEExplicitBoilerplate : public HGEDeriver< HGEExplicitBoilerplate< BaseType, HGENothing >, DerivedType > {};

template < typename BaseType >
class HGEExplicitBoilerplate< BaseType, void > : public BaseType
// HGE_BOILERPLATE(HGEExplicitBoilerplate, BaseType)
{
	// sugary typedefs are not mandatory but they sure can help
public:
	typedef HGEExplicitBoilerplate ImpExplicitBoilerplate;
	// HGE_BOILERPLATE_SUGAR_IMP(HGEExplicitBoilerplate, ImpExplicitBoilerplate);
public:
	typedef typename BaseType::HGE_BOILERPLATE_TYPEDEF_DERIVED RealExplicitBoilerplate;
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
