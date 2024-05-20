// PICO_Wifi.cpp

/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>


#include "PICO_Wifi.h"
#include "PICO_Serial.h"



 // WiFi network name and password:
const char* networkName = "RTAX999";
const char* networkPswd = "LiDoDa#959285$";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char* udpAddress = "192.168.2.101";
const int udpPort = 8080;

//Are we currently connected?
boolean connected = false;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

//The udp library class
WiFiUDP udp;

void ConnectToWiFi(const char* ssid, const char* pwd);

void WifiSetup() {
    // Initilize hardware serial:
    Serial.print("Connect Wifi start: ");
    Serial.println(networkName);
    //Connect to the WiFi network
    ConnectToWiFi(networkName, networkPswd);
}

void WifiLoop() {
    //only send data when connected
    if (connected) {
        //Send a packet
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("Seconds since boot: %lu", millis() / 1000);
        udp.endPacket();
    }
    //Wait for 1 second
    delay(1000);
}

void ConnectToWiFi(const char* ssid, const char* pwd) 
{
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);

    //Initiate connection
    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

}


void WifiScan()
{
    Serial.printf("Beginning scan at %lu\n", millis());
    auto cnt = WiFi.scanNetworks();
    if (!cnt) {
        Serial.printf("No networks found\n");
    }
    else {
        Serial.printf("Found %d networks\n\n", cnt);
        Serial.printf("%32s %5s %17s %2s %4s\n", "SSID", "ENC", "BSSID        ", "CH", "RSSI");
        for (auto i = 0; i < cnt; i++) {
            uint8_t bssid[6];
            WiFi.BSSID(i, bssid);
//            Serial.printf("%32s %5s %17s %2d %4ld\n", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid), WiFi.channel(i), WiFi.RSSI(i));
            Serial.printf("%32s %2d %4ld\n", WiFi.SSID(i), WiFi.channel(i), WiFi.RSSI(i));
        }
    }
    Serial.printf("\n--- Sleeping ---\n\n\n");

}


WiFiUDP Udp;

void UdpSetup(unsigned int localPort) 
{
    if (WiFi.status() == WL_CONNECTED) {
        Udp.begin(localPort);
        connected = true;
    }
}

void UdpLoop()
{
    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort());

        // read the packet into packetBufffer
        int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        packetBuffer[n] = 0;
        Serial.println("Contents:");
        Serial.println(packetBuffer);

        // send a reply, to the IP address and port that sent us the packet we received
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(ReplyBuffer);
        Udp.endPacket();
    }
}