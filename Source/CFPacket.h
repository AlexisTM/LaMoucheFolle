/*
  ==============================================================================

	CFPacket.h
	Created: 19 Jun 2018 1:55:30pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "crtp.h"

class CFDrone;

class CFPacket
{
public:
	enum Type { CONSOLE, LOG_INFO, LOG_ITEM, LOG_CONTROL, LOG_DATA, PARAM_TOC_INFO, PARAM_TOC_ITEM, MEMORY_NUMBER, MEMORY_INFO, PARAM_VALUE, RSSI_ACK };
	
	CFPacket(CFDrone * drone, const ITransport::Ack &ack); 
	~CFPacket();

	Type type;
	var data; 
};