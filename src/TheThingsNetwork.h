// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSNETWORK_H_
#define _THETHINGSNETWORK_H_

#include <Arduino.h>
#include <Stream.h>

#define TTN_DEFAULT_SF 7
#define TTN_DEFAULT_FSB 2
#define TTN_RETX 7

#define TTN_PWRIDX_868 1
#define TTN_PWRIDX_915 5

enum ttn_fp_t {
  TTN_FP_EU868,
  TTN_FP_US915
};

typedef struct  airtime_s
{
  uint8_t sf;
  uint8_t de;
  uint8_t ps;
  uint16_t band;
  uint8_t header;
  uint8_t cr;
} airtime_t;

class TheThingsNetwork
{
  private:
    Stream* modemStream;
    Stream* debugStream;
    String model;
    airtime_t info;
    float airtime;
    ttn_fp_t fp;
    uint8_t sf;
    uint8_t fsb;
    void (* messageCallback)(const byte* payload, size_t length, uint8_t port);

    String readLine();
    void fillAirtimeInfo();
    void trackAirtime(size_t payloadSize);
    String readValue(String key);
    bool sendCommand(String cmd);
    bool sendCommand(String cmd, String value);
    bool sendCommand(String cmd, const byte* buf, size_t length);
    void reset(bool adr = true);
    void configureEU868(uint8_t sf);
    void configureUS915(uint8_t sf, uint8_t fsb);
    void configureChannels(uint8_t sf, uint8_t fsb);

  public:
    TheThingsNetwork(Stream& modemStream, Stream& debugStream, ttn_fp_t fp, uint8_t sf = TTN_DEFAULT_SF, uint8_t fsb = TTN_DEFAULT_FSB);
    void showStatus();
    void onMessage(void (*cb)(const byte* payload, size_t length, uint8_t port));
    bool provision(const byte appEui[8], const byte appKey[16]);
    bool join(const byte appEui[8], const byte appKey[16], int8_t retries = -1, uint32_t retryDelay = 10000);
    bool join(int8_t retries = -1, uint32_t retryDelay = 10000);
    bool personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]);
    bool personalize();
    int sendBytes(const byte* payload, size_t length, uint8_t port = 1, bool confirm = false);
    int poll(uint8_t port = 1, bool confirm = false);
};

#endif
