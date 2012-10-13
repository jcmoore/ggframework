//
//  HGEModel.cpp
//  hybridge
//
//  Created by The Narrator on 8/21/12.
//  Copyright (c) 2012 Starduu. All rights reserved.
//

#include "core/HGEModel.h"

NS_HGE_BEGIN

HGEModel::HGEModel()
: version(HGE_VERSION_ZERO())
{
	
}

HGEModel::~HGEModel() {
	
}

version_hge HGEModel::hgerevise() {
	HGE_VERSION_UP(this->version);
	
	return this->version;
}

NS_HGE_END
