//
//  HGERouter.h
//  hybridge
//
//  Created by The Narrator on 8/14/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#ifndef __HGEROUTER_H__
#define __HGEROUTER_H__

#include "service/HGEWorker.h"
#include "util/HGEUtilString.h"
#include "core/HGEDoer.h"
#include "core/can/HGECanRout.h"

#include <map>
#include <list>

NS_HGE_BEGIN

typedef const char * HGEBottomLevelDomainName;
typedef id_hge HGEPortNumber;
typedef HGECanRout<>::Magic HGEHandler;
typedef std::map< HGEPortNumber, HGEHandler * > HGEPortMap;
// TODO: can I make std::string instead const char * ?
typedef std::map< std::string const, HGEPortMap * > HGEDomainMap;
typedef HGEHandler * (*HGEGenerator)(id_hge);
typedef std::map< const char *, HGEGenerator, cmpstr > HGEGeneratorMap;

/**
 worker in charge of creating, destroying, and passing messages to
 uniquely identifiable doers
 */
class HGERouter : public HGEWorker {
	
public:
	
	class BottomLevelNameServerInterface;
	
	class TableInterface {
	public:
		TableInterface() {}
		~TableInterface() {}
		
		virtual HGEHandler * whois(HGEBottomLevelDomainName bldn, HGEPortNumber port) = 0;
		
		virtual bool assign(HGEHandler * owner, HGEBottomLevelDomainName bldn, HGEPortNumber port) = 0;
		
		virtual bool revoke(HGEHandler * owner, HGEBottomLevelDomainName bldn, HGEPortNumber port) = 0;
		
	protected:
		virtual void flip(HGEWorker * producer, HGERouter::BottomLevelNameServerInterface * bdns) = 0;
		
		virtual void safeDomain(HGEBottomLevelDomainName inputBLDN,
								HGEPortMap ** ouputList,
								HGEBottomLevelDomainName * outputBLDN = 0) = 0;
		
		friend class HGERouter;
	};
	
	class BottomLevelNameServerInterface {
	public:
		BottomLevelNameServerInterface() {}
		~BottomLevelNameServerInterface() {}
		
		virtual HGEHandler * whois(HGEBottomLevelDomainName bldn, HGEPortNumber port) = 0;
	};
	
	class SpawnerInterface {
	public:
		SpawnerInterface() {}
		~SpawnerInterface() {}
		
		virtual HGEHandler * generate(const char* type,
									  JSONValue& json,
									  HGEBottomLevelDomainName bldn,
									  HGEPortNumber port,
									  HGERouter * router,
									  HGERouter::BottomLevelNameServerInterface * dns) = 0;
	};
	
	HGERouter(const char * a,
			  HGERouter::TableInterface * t,
			  HGERouter::BottomLevelNameServerInterface * b,
			  HGERouter::SpawnerInterface * s);
	virtual ~HGERouter();
	
protected:
	/**
	 hge@: ({'hgeid':idNumber, 'task':taskJSON, ...}) passes taskJSON to the doer with id idNumber,
	 *	if no doer exists with the desired id and the taskJSON is JSON String, a new doer is generated
	 *	with the idNumber using the generator specified by the taskJSON String,
	 *	if the taskJSON is a JSON String prefixed with a '~' (tilde), any doer found with the id idNumber
	 *	is destroyed
	 */
	virtual bool digestJSON(JSONValue& json);
	
private:
	
	HGERouter::TableInterface * table;
	
	HGERouter::BottomLevelNameServerInterface * bdns;
	
	HGERouter::SpawnerInterface * spawner;
	
	
	
	
	JSONValue cleanupTasklist;
	
public:
	/**
	 add to a set of tasks the dispatcher should hand out when it is destroyed (a cleanup mechanism) --
	 currently this feature is only used in unit testing to avoid memory leaks
	 */
	void tidyJSON (JSONValue& task);
	
	class BottomLevelNameServer: public HGERouter::BottomLevelNameServerInterface {
	public:
		BottomLevelNameServer(HGERouter::TableInterface * t) : table(t) {
			HGEAssertC(this->table, "this name server has no table . . . probably a mistake");
		}
		~BottomLevelNameServer() {}
		
		virtual HGEHandler * whois(HGEBottomLevelDomainName bldn, HGEPortNumber port) {
			HGEAssertC(this->table, "this name server has no table . . . probably a mistake");
			return this->table->whois(bldn, port);
		}
		
	private:
		HGERouter::TableInterface * table;
	};
	
	
	
	class Table : public HGERouter::TableInterface {
	public:
		Table() {}
		~Table() {
			this->flip(0, 0);
		};
		
		virtual HGEHandler * whois(HGEBottomLevelDomainName bldn, HGEPortNumber port) {
			
			HGEPortMap * listing = 0;
			
			this->safeDomain(bldn, &listing);
			
			HGEPortMap::iterator iter = listing->find(port);
			
			if (iter == listing->end()) {
				return 0;
			} else {
				return iter->second;
			}
		}
		
	protected:
		
		virtual bool assign(HGEHandler * owner, HGEBottomLevelDomainName bldn, HGEPortNumber port) {
			if (owner == 0) {
				HGEAssertC(0, "no valid owner provided for assignment (requests to clear records should invoke revoke() instead)");
				return 0;
			}
			
			HGEPortMap * listing = 0;
			
			this->safeDomain(bldn, &listing);
			
			HGEPortMap::iterator iter = listing->find(port);
			
			if (iter != listing->end()) {
				if (iter->second == owner) {
					return !0;
				}
				HGEAssertC(0, "register .%s:%li is already assigned", bldn, port);
				return 0;
			}
			
			(*listing)[port] = owner;
			
			return !0;
		}
		
		virtual bool revoke(HGEHandler * owner, HGEBottomLevelDomainName bldn, HGEPortNumber port) {
			HGEPortMap * listing = 0;
			
			this->safeDomain(bldn, &listing);
			
			HGEPortMap::iterator iter = listing->find(port);
			
			if (iter != listing->end()) {
				HGEAssertC(0, "register .%s:%li is not assigned", bldn, port);
				return 0;
			} else if (iter->second != owner) {
				HGEAssertC(0, "register .%s:%li is not assigned to the suggested owner", bldn, port);
				return 0;
			}
			
			listing->erase(iter);
			
			return true;
		}
		
		virtual void flip(HGEWorker * producer, HGERouter::BottomLevelNameServerInterface * bdns) {
			bool leaking = 0;
			JSONValue& undefined = HGEJSONRef::Undefined();
			for (HGEDomainMap::iterator iter = this->records.begin();
				 iter != this->records.end(); ) {
				for (HGEPortMap::iterator jter = iter->second->begin();
					 jter != iter->second->end(); ) {
					if (producer &&
						bdns) {
						jter->second->destroyJSON(undefined, !0);
					} else {
						HGEAssertC(!leaking, "probably leaking . . ."); // assert once
						leaking = !0;
					}
					HGEDelete(jter->second);
				}
				HGEDelete(iter->second);
			}
		}
		
		virtual void safeDomain(HGEBottomLevelDomainName inputBLDN,
								HGEPortMap ** ouputList,
								HGEBottomLevelDomainName * outputBLDN = 0) { // TODO: safe?
			inputBLDN = inputBLDN ? inputBLDN : "null";
			HGEDomainMap::iterator iter = (this->records.insert(HGEDomainMap::value_type(inputBLDN, 0))).first;
			
			if (!iter->second) {
				iter->second = new HGEPortMap();
			}
			
			if (outputBLDN) {
				*outputBLDN = iter->first.c_str();
			}
			if (ouputList) {
				*ouputList = iter->second;
			}
		}
		
	private:
		HGEDomainMap records;
	};
	
};



NS_HGE_END

#endif
