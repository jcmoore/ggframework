//
//  HGECCZone.cpp
//  hybridge
//
//  Created by The Narrator on 10/28/12.
//
//

#include "graphics/HGECCZone.h"

//#include "service/HGERouter.h"

USING_NS_CC;

#define NATIVE_ONLY				0

#define HGE_KEYTEXT_TASK_ZONE_INPUT			"input"



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

class CCFieldInputLayer : public CCLayer {
public:
	CCFieldInputLayer(HGECanJott<>::Magic * j)
	: CCLayer()
	, jotter(j) {
	}
	
	typedef CCPrimitive<timeSD_hge> PrimitiveTime;
	
	void passInput(JSONValue& json, bool purify) {
#if !NATIVE_ONLY
		this->jotter->jott(json, purify);
#endif
	}
	
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
			
			timeSD_hge startTime = this->startTimeForTouch(touch->getID(), currentTime);
			timeSD_hge lastTime = this->updateTimeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", 1);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - lastTime);
			value.AddMember("xx", (int)(0));
			value.AddMember("yy", (int)(0));
			value.AddMember("a", currentTime - startTime);
			
			list.PushBack(value);
			
			count++;
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember(HGE_KEYTEXT_TASK, HGE_KEYTEXT_TASK_ZONE_INPUT);
		message.AddMember(HGE_KEYTEXT_ARGS, list);
		message.AddMember("meta", 1 * count);
		
		this->passInput(message, 0);
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
			
			timeSD_hge startTime = this->startTimeForTouch(touch->getID(), currentTime);
			timeSD_hge lastTime = this->updateTimeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", 0);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - lastTime);
			value.AddMember("xx", (int)(locationNow.x - locationThen.x));
			value.AddMember("yy", (int)(locationNow.y - locationThen.y));
			value.AddMember("a", currentTime - startTime);
			
#if NATIVE_ONLY
			CCPoint locationMy = this->getPosition();
			this->setPosition(locationMy.x + (locationNow.x - locationThen.x),
							  locationMy.y + (locationNow.y - locationThen.y));
#endif
			
			list.PushBack(value);
			
			count++;
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember(HGE_KEYTEXT_TASK, HGE_KEYTEXT_TASK_ZONE_INPUT);
		message.AddMember(HGE_KEYTEXT_ARGS, list);
		message.AddMember("meta", 0 * count);
		
		this->passInput(message, 0);
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
			
			timeSD_hge startTime = this->startTimeForTouch(touch->getID(), currentTime);
			timeSD_hge lastTime = this->updateTimeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", -1);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - lastTime);
			value.AddMember("xx", (int)(locationNow.x - locationThen.x));
			value.AddMember("yy", (int)(locationNow.y - locationThen.y));
			value.AddMember("a", currentTime - startTime);
			
			list.PushBack(value);
			
			count++;
			
			this->zeroTimeForTouch(touch->getID());
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember(HGE_KEYTEXT_TASK, HGE_KEYTEXT_TASK_ZONE_INPUT);
		message.AddMember(HGE_KEYTEXT_ARGS, list);
		message.AddMember("meta", -1 * count);
		
		this->passInput(message, 0);
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
			
			timeSD_hge startTime = this->startTimeForTouch(touch->getID(), currentTime);
			timeSD_hge lastTime = this->updateTimeForTouch(touch->getID(), currentTime);
			
			JSONValue value(vapidjson::kObjectType);
			
			value.AddMember("f", vapidjson::kNullType);
			value.AddMember("n", touch->getID());
			value.AddMember("t", currentTime);
			value.AddMember("x", (int)(locationNow.x));
			value.AddMember("y", (int)(locationNow.y));
			value.AddMember("tt", currentTime - lastTime);
			value.AddMember("xx", (int)(locationNow.x - locationThen.x));
			value.AddMember("yy", (int)(locationNow.y - locationThen.y));
			value.AddMember("l", currentTime - startTime);
			
			list.PushBack(value);
			
			count++;
			
			this->zeroTimeForTouch(touch->getID());
		}
		
		JSONValue message(vapidjson::kObjectType);
		message.AddMember(HGE_KEYTEXT_TASK, HGE_KEYTEXT_TASK_ZONE_INPUT);
		message.AddMember(HGE_KEYTEXT_ARGS, list);
		message.AddMember("meta", -1 * count);
		
		this->passInput(message, 0);
	}
	
	timeSD_hge startTimeForTouch(int id, timeSD_hge fallback) {
		
		PrimitiveTime * t = (PrimitiveTime *)this->startTimes.objectForKey(id);
		
		if (t) {
			return t->getValue();
		}
		
		t = new PrimitiveTime(fallback);
		this->startTimes.setObject(t, id);
		t->release();
		
		return t->getValue();
	}
	
	timeSD_hge updateTimeForTouch(int id, timeSD_hge current) {
		
		PrimitiveTime * t = (PrimitiveTime *)this->lastTimes.objectForKey(id);
		
		if (t) {
			timeSD_hge last = t->getValue();
			t->setValue(current);
			return last;
		}
		
		t = new PrimitiveTime(current);
		this->startTimes.setObject(t, id);
		t->release();
		
		return t->getValue();
	}
	
	void zeroTimeForTouch(int id) {
		
		updateTimeForTouch(id, 0);
		
		PrimitiveTime * t = (PrimitiveTime *)this->startTimes.objectForKey(id);
		
		if (t) {
			t->setValue(0);
		}
		
		t = new PrimitiveTime(0);
		this->startTimes.setObject(t, id);
		t->release();
	}
	
	HGECanJott<>::Magic * jotter;
	
	CCDictionary startTimes;
	CCDictionary lastTimes;
};

bool HGECCZone::destroyJSON(JSONValue& json, bool firstResponder)
{
	bool didDestroy = 0;
	
	if (firstResponder) {
		if (this->cc.layer) {
			this->cc.layer->removeFromParentAndCleanup(!0);
			this->cc.layer->release();
			this->cc.layer = 0;
		}
		didDestroy = !0;
		firstResponder = 0;
	}
	
	return HGECCNexus::destroyJSON(json, firstResponder) || didDestroy;
}

bool HGECCZone::createJSON(JSONValue& json, bool firstResponder)
{
	bool didCreate = 0;
	
	if (json.IsObject()) {
		if (firstResponder) {
			HGEAssertC(!this->cc.layer, "leaking");
			this->cc.layer = new CCFieldInputLayer(this->tasker.feat());
			this->cc.layer->init();
			this->cc.layer->setTouchEnabled(!0);
			didCreate = !0;
			firstResponder = 0;
		}
		
		
	}
	
	return HGECCNexus::createJSON(json, firstResponder) || didCreate;
}

bool HGECCZone::enactJSON(JSONValue& task, JSONValue& json, bool firstResponder)
{
	bool didEnact = 0;
	
	return didEnact || HGECCNexus::enactJSON(task, json, firstResponder);
}

NS_HGE_END
