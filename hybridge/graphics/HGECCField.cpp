//
//  HGECCField.cpp
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#include "graphics/HGECCField.h"

//#include "service/HGERouter.h"

USING_NS_CC;

NS_HGE_BEGIN

template <typename Simple>
class CC_DLL CCPrimitive : public CCObject
{
public:
    CCPrimitive(Simple v)
	: value(v) {}
    Simple getValue() const {return value;}
    bool setValue(Simple v) { value = v; return !0; }
	
    static CCPrimitive* create(Simple v)
    {
        CCPrimitive* pRet = new CCPrimitive(v);
        pRet->autorelease();
        return pRet;
    }
private:
    Simple value;
};

class FieldInputLayer : public CCLayer {
public:
	FieldInputLayer(HGECCField * f)
	: CCLayer()
	, field(f) {
	}
	
	typedef CCPrimitive<timeSD_hge> PrimitiveTime;
	
    virtual void ccTouchesBegan(CCSet * touches, CCEvent * event) {
		
		timeSD_hge currentTime = HGE_TIME_SD();
		
		JSONValue list(vapidjson::kArrayType);
		
		CCSetIterator it;
		CCTouch* touch;
		
		int32_t count = 0;
		
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			
			if(!touch)
			{
				break;
			}
			
			CCPoint locationNow = touch->getLocation();
			
			timeSD_hge previousTime = this->timeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", 1);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - previousTime);
			value.AddMember("xx", (int)(0));
			value.AddMember("yy", (int)(0));
			
			list.PushBack(value);
			
			count++;
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember("event", "input");
		message.AddMember("data", list);
		message.AddMember("meta", 1 * count);
		
		this->field->produce(message, 0);
	}
	
    virtual void ccTouchesMoved(CCSet * touches, CCEvent * event) {
		
		timeSD_hge currentTime = HGE_TIME_SD();
		
		JSONValue list(vapidjson::kArrayType);
		
		CCSetIterator it;
		CCTouch* touch;
		
		int32_t count = 0;
		
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			
			if(!touch)
			{
				break;
			}
			
			CCPoint locationNow = touch->getLocation();
			CCPoint locationThen = touch->getPreviousLocation();
			
			timeSD_hge previousTime = this->timeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", 1);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - previousTime);
			value.AddMember("xx", (int)(locationNow.x - locationThen.x));
			value.AddMember("yy", (int)(locationNow.y - locationThen.y));
			
			list.PushBack(value);
			
			count++;
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember("event", "input");
		message.AddMember("data", list);
		message.AddMember("meta", 0 * count);
		
		this->field->produce(message, 0);
	}
	
    virtual void ccTouchesEnded(CCSet * touches, CCEvent * event) {
		
		timeSD_hge currentTime = HGE_TIME_SD();
		
		JSONValue list(vapidjson::kArrayType);
		
		CCSetIterator it;
		CCTouch* touch;
		
		int32_t count = 0;
		
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			
			if(!touch)
			{
				break;
			}
			
			CCPoint locationNow = touch->getLocation();
			CCPoint locationThen = touch->getPreviousLocation();
			
			timeSD_hge previousTime = this->timeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", 1);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - previousTime);
			value.AddMember("xx", (int)(locationNow.x - locationThen.x));
			value.AddMember("yy", (int)(locationNow.y - locationThen.y));
			
			list.PushBack(value);
			
			count++;
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember("event", "input");
		message.AddMember("data", list);
		message.AddMember("meta", -1 * count);
		
		this->field->produce(message, 0);
	}
	
    virtual void ccTouchesCancelled(CCSet * touches, CCEvent * event) {
		
		timeSD_hge currentTime = HGE_TIME_SD();
		
		JSONValue list(vapidjson::kArrayType);
		
		CCSetIterator it;
		CCTouch* touch;
		
		int32_t count = 0;
		
		for( it = touches->begin(); it != touches->end(); it++)
		{
			touch = (CCTouch*)(*it);
			
			if(!touch)
			{
				break;
			}
			
			CCPoint locationNow = touch->getLocation();
			CCPoint locationThen = touch->getPreviousLocation();
			
			timeSD_hge previousTime = this->timeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", 1);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - previousTime);
			value.AddMember("xx", (int)(locationNow.x - locationThen.x));
			value.AddMember("yy", (int)(locationNow.y - locationThen.y));
			
			list.PushBack(value);
			
			count++;
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember("event", "input");
		message.AddMember("data", list);
		message.AddMember("meta", -1 * count);
		
		this->field->produce(message, 0);
	}
	
	timeSD_hge timeForTouch(int id, timeSD_hge fallback) {
		
		PrimitiveTime * t = (PrimitiveTime *)this->inputTimes.objectForKey(id);
		
		if (t) {
			return t->getValue();
		}
		
		t = new PrimitiveTime(fallback);
		this->inputTimes.setObject(t, id);
		t->release();
		
		return t->getValue();
	}
	
	void zeroTimeForTouch(int id) {
		
		PrimitiveTime * t = (PrimitiveTime *)this->inputTimes.objectForKey(id);
		
		if (t) {
			t->setValue(0);
		}
		
		t = new PrimitiveTime(0);
		this->inputTimes.setObject(t, id);
		t->release();
	}
	
	HGECCField * field;
	
	CCDictionary inputTimes;
};

bool HGECCField::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		HGEDeleteNull(this->cc.layer);
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didDestroy;
}

bool HGECCField::createJSON(JSONValue& json, bool firstResponder)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.layer, "leaking");
			this->cc.layer = new FieldInputLayer(this);
			this->cc.layer->init();
			this->cc.layer->setTouchEnabled(!0);
			didCreate = !0;
			firstResponder = 0;
		}
		
		
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didCreate;
}

bool HGECCField::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	return didEnact || HGECCNexus::enactJSON(task, json, firstResponder);
}

NS_HGE_END
