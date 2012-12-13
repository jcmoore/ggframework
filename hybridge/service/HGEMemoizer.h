//
//  HGEMemoizer.h
//  hybridge
//
//  Created by The Narrator on 12/12/12.
//
//

#ifndef __HGEMEMOIZER_H__
#define __HGEMEMOIZER_H__

#include "service/HGEWorker.h"

#include <map>



NS_HGE_BEGIN

class HGEMemoizer : public HGEWorker {
	
	typedef enum {
		MaskDisableDefaultOutputStream,
		MaskDisableDefaultInputStream,
	} Mask;
	
public:
	
	typedef enum {
		FlagEnableDefaultIOStreams,
		FlagDisableDefaultOutputStream = (1 << MaskDisableDefaultOutputStream),
		FlagDisableDefaultInputStream = (1 << MaskDisableDefaultInputStream),
		FlagDisableDefaultIOStreams = FlagDisableDefaultOutputStream | FlagDisableDefaultInputStream,
	} Flag;
	
	struct StreamInterface {
		
		virtual void repath(const char * path) = 0;
		virtual void stream(const char * data) = 0;
		virtual void erase() = 0;
		
	};
	
	struct FactoryInterface {
		virtual StreamInterface * make(const char * format, const char * path) = 0;
	};

private:
	
	struct StreamConfig {
		StreamConfig()
		: stream(0)
		, disabled(0)
		{}
		
		StreamInterface * stream;
		unsigned char disabled;
	};
	
	typedef std::map<const char *, StreamConfig, cmpstr> ConifgMap;
	typedef ConifgMap::iterator ConfigIterator;
	
public:
	HGEMemoizer(const char * a,
				FactoryInterface * f,
				Flag settings = FlagEnableDefaultIOStreams,
				StreamInterface * ostream = 0,
				StreamInterface * istream = 0);
	virtual ~HGEMemoizer();
	
protected:
	virtual bool digestJSON(JSONValue& json);
	
private:
	
	FactoryInterface * factory;
	ConifgMap inputConfigs;
	ConifgMap outputConfigs;
	
	bool clear(bool input, const char * stream);
	bool enable(bool input, const char * stream, const char * format);
	bool disable(bool input, const char * stream);
	bool rename(bool input, const char * from, const char * to);
	
public:
	
	void istream(const char * data);
	void ostream(const char * data);
	
};

NS_HGE_END

#endif
