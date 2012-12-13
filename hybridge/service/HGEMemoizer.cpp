//
//  HGEMemoizer.cpp
//  hybridge
//
//  Created by The Narrator on 12/12/12.
//
//

#include "service/HGEMemoizer.h"

#include <sstream>



NS_HGE_BEGIN

HGEMemoizer::~HGEMemoizer() {
	{
		ConifgMap& map = this->outputConfigs;
		for (ConfigIterator iter = map.begin(); iter != map.end(); iter++) {
			
			StreamConfig& config = iter->second;
			StreamInterface *& s = config.stream;
			
			HGEDeleteNull(s);
		}
	}
	{
		ConifgMap& map = this->inputConfigs;
		for (ConfigIterator iter = map.begin(); iter != map.end(); iter++) {
			
			StreamConfig& config = iter->second;
			StreamInterface *& s = config.stream;
			
			HGEDeleteNull(s);
		}
	}
}

HGEMemoizer::HGEMemoizer(const char * a,
						 FactoryInterface * f,
						 Flag settings,
						 StreamInterface * ostream,
						 StreamInterface * istream)
: HGEWorker(a)
, factory(f) {
	
	time_t t;
	time(&t);
	
	if (ostream) {
		StreamConfig& config = this->outputConfigs[HGE_KEYTEXT_SERVICE_MEMOIZER_STREAM_DEFAULT];
		config.stream = ostream;
		config.disabled = 0;
	} else if (!(FlagDisableDefaultOutputStream & settings) && this->factory) {
		std::ostringstream s;
		s << "dump_" << t << "_o.txt";
		StreamConfig& config = this->outputConfigs[HGE_KEYTEXT_SERVICE_MEMOIZER_STREAM_DEFAULT];
		config.stream = this->factory->make(0, s.str().c_str());
		config.disabled = 0;
	}
	
	if (istream) {
		StreamConfig& config = this->inputConfigs[HGE_KEYTEXT_SERVICE_MEMOIZER_STREAM_DEFAULT];
		config.stream = istream;
		config.disabled = 0;
	} else if (!(FlagDisableDefaultInputStream & settings) && this->factory) {
		std::ostringstream s;
		s << "dump_" << t << "_i.txt";
		StreamConfig& config = this->inputConfigs[HGE_KEYTEXT_SERVICE_MEMOIZER_STREAM_DEFAULT];
		config.stream = this->factory->make(0, s.str().c_str());
		config.disabled = 0;
	}
}

void HGEMemoizer::istream(const char * data) {
	ConifgMap& map = this->inputConfigs;
	for (ConfigIterator iter = map.begin(); iter != map.end(); iter++) {
		
		StreamConfig& config = iter->second;
		
		if (config.disabled) {
			continue;
		}
		
		StreamInterface *& s = config.stream;
		
		if (s) {
			s->stream(data);
		}
	}
}

void HGEMemoizer::ostream(const char * data) {
	ConifgMap& map = this->outputConfigs;
	for (ConfigIterator iter = map.begin(); iter != map.end(); iter++) {
		
		StreamConfig& config = iter->second;
		
		if (config.disabled) {
			continue;
		}
		
		StreamInterface *& s = config.stream;
		
		if (s) {
			s->stream(data);
		}
	}
}

bool HGEMemoizer::digestJSON(JSONValue& json) {
	bool result = 0;
	for (JSONMemberIter iter = json.MemberBegin(); iter != json.MemberEnd(); iter++) {
		const char * job = iter->first.c_str();
		JSONValue const * tools = iter->second;
		
		if (0 == strcmp("clear", job)) {
			if (!tools->IsArray()) {
				continue;
			}
			JSONValue const * inputValue = 0;
			JSONValue const * streamValue = 0;
			
			VAPIDJSON_GET_ELEMENTS(*tools, &inputValue, &streamValue);
			
			if (!inputValue->IsBool() ||
				!streamValue->IsString()) {
				continue;
			}
			
			bool input = inputValue->GetBool();
			const char * stream = streamValue->GetString();
			
			result = this->clear(input, stream) && result;
		} else if (0 == strcmp("disable", job)) {
			if (!tools->IsArray()) {
				continue;
			}
			JSONValue const * inputValue = 0;
			JSONValue const * streamValue = 0;
			
			VAPIDJSON_GET_ELEMENTS(*tools, &inputValue, &streamValue);
			
			if (!inputValue->IsBool() ||
				!streamValue->IsString()) {
				continue;
			}
			
			bool input = inputValue->GetBool();
			const char * stream = streamValue->GetString();
			
			result = this->disable(input, stream) && result;
		} else if (0 == strcmp("enable", job)) {
			if (!tools->IsArray()) {
				continue;
			}
			JSONValue const * inputValue = 0;
			JSONValue const * streamValue = 0;
			JSONValue const * formatValue = 0;
			
			VAPIDJSON_GET_ELEMENTS(*tools, &inputValue, &streamValue, &formatValue);
			
			if (!inputValue->IsBool() ||
				!streamValue->IsString()) {
				continue;
			}
			
			bool input = inputValue->GetBool();
			const char * stream = streamValue->GetString();
			const char * format = formatValue->IsString() ? formatValue->GetString() : 0;
			
			result = this->enable(input, stream, format) && result;
		} else if (0 == strcmp("rename", job)) {
			if (!tools->IsArray()) {
				continue;
			}
			JSONValue const * inputValue = 0;
			JSONValue const * streamValue = 0;
			JSONValue const * nameValue = 0;
			
			VAPIDJSON_GET_ELEMENTS(*tools, &inputValue, &streamValue, &nameValue);
			
			if (!inputValue->IsBool() ||
				!streamValue->IsString() ||
				!nameValue->IsString()) {
				continue;
			}
			
			bool input = inputValue->GetBool();
			const char * stream = streamValue->GetString();
			const char * name = nameValue->GetString();
			
			result = this->rename(input, stream, name) && result;
		}
	}
	return result;
}

bool HGEMemoizer::clear(bool input, const char * stream) {
	ConifgMap& map = input ? this->inputConfigs : this->outputConfigs;
	ConfigIterator iter = map.find(stream);
	
	if (iter == map.end()) {
		return 0;
	}
	
	StreamConfig& config = iter->second;
	StreamInterface *& s = config.stream;
	
	if (s) {
		s->erase();
	}
	
	return !0;
}

bool HGEMemoizer::enable(bool input, const char * stream, const char * format) {
	StreamConfig& config = input ? this->inputConfigs[stream] : this->outputConfigs[stream];
	config.disabled = 0;
	if (!config.stream) {
		if (strcmp(stream, HGE_KEYTEXT_SERVICE_MEMOIZER_STREAM_DEFAULT)) {
			HGEAssertC(0, "building default memo streams after initialization is unsupported");
			return 0;
		}
		
		if (this->factory) {
			config.stream = this->factory->make(format, stream);
		}
	} else {
		HGEAssertC(!format, "reformatting an existing stream is not supported");
		return 0;
	}
	
	return !0;
}

bool HGEMemoizer::disable(bool input, const char * stream) {
	ConifgMap& map = input ? this->inputConfigs : this->outputConfigs;
	ConfigIterator iter = map.find(stream);
	
	if (iter == map.end()) {
		return 0;
	}
	
	StreamConfig& config = iter->second;
	config.disabled = !0;
	
	return !0;
}

bool HGEMemoizer::rename(bool input, const char * from, const char * to) {
	if (!from ||
		!to ||
		0 == strcmp(from, to)) {
		return 0;
	}
	
	ConifgMap& map = input ? this->inputConfigs : this->outputConfigs;
	ConfigIterator iter = map.find(from);
	
	if (iter == map.end()) {
		return 0;
	}
	
	StreamConfig& config = map[to];
	
	config.stream = iter->second.stream;
	config.disabled = iter->second.disabled;
	
	map.erase(iter);
	
	if (0 != strcmp(to, HGE_KEYTEXT_SERVICE_MEMOIZER_STREAM_DEFAULT)) {
		// do not repath default memo streams
		if (config.stream) {
			config.stream->repath(to);
		}
	}
	
	return !0;
}

NS_HGE_END
