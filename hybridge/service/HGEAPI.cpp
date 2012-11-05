//
//  HGEAPI.cpp
//  hybridge
//
//  Created by The Narrator on 7/5/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "service/HGEAPI.h"

#include "service/HGELogger.h"
#include "service/HGERouter.h"

#define VAPIDJSON_USE_PARSE_INSITU		0

NS_HGE_BEGIN

void HGEAPIWorker::flushMessage(JSONBuffer& stream) {
	JSONValue json;
	
	this->chief->reportJSON(json);
	
	JSONWriter output(stream);
	
	json.Accept(output);
}

void HGEAPIWorker::handleMessage(const char * data, size_t length) {
	
	JSONDoc doc;
	
#if VAPIDJSON_USE_PARSE_INSITU
	// WARNING: IT IS CRITICAL THAT EVERYTHING THAT NEEDS JSON VALUES FROM A ParseInsitu MAKE A COPY ITSELF (BUFFER FREEING CAN CAUSE SHENANIGANS -- ESP. FOR JSONObjects)
	if (this->capacity <= 0 ||
		this->capacity - 1 < length) {
		if (this->capacity == 0) {
			this->capacity = 1024;
		}
		
		while (this->capacity - 1 < length) {
			this->capacity *= 2;
		}
		
		free(this->buffer);
		this->buffer = (char *)malloc(this->capacity * sizeof(char));
	}
	
	
	memcpy(this->buffer, data, length);
	this->buffer[length] = '\0';
	
	bool blewup = doc.ParseInsitu<0>(buffer).HasParseError(); blewup = blewup;
	// WARNING: IT IS CRITICAL THAT EVERYTHING THAT NEEDS JSON VALUES FROM A ParseInsitu MAKE A COPY ITSELF (BUFFER FREEING CAN CAUSE SHENANIGANS -- ESP. FOR JSONObjects)
#else
	bool blewup = doc.Parse<0>(data).HasParseError(); blewup = blewup;
#endif
	
	this->chief->consumeJSON(doc.GetValue());
}

NS_HGE_END
