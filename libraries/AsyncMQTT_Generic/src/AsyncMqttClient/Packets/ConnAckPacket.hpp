/****************************************************************************************************************************
  ConnAckPacket.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/
 
#pragma once

#ifndef CONN_ACK_PACKET_HPP
#define CONN_ACK_PACKET_HPP

#include "Arduino.h"
#include "Packet.hpp"
#include "../ParsingInformation.hpp"
#include "../Callbacks.hpp"

namespace AsyncMqttClientInternals 
{
  class ConnAckPacket : public Packet 
  {
   public:
    explicit ConnAckPacket(ParsingInformation* parsingInformation, OnConnAckInternalCallback callback);
    ~ConnAckPacket();

    void parseVariableHeader(char* data, size_t len, size_t* currentBytePosition);
    void parsePayload(char* data, size_t len, size_t* currentBytePosition);

   private:
    ParsingInformation* _parsingInformation;
    OnConnAckInternalCallback _callback;

    uint8_t _bytePosition;
    bool _sessionPresent;
    uint8_t _connectReturnCode;
  };
}  // namespace AsyncMqttClientInternals

#endif    // CONN_ACK_PACKET_HPP
