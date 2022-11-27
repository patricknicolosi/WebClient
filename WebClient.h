
#include <UIPEthernet.h>
#include <ArduinoHttpClient.h>

class WebClient {
private:
  EthernetClient ethernetClient;
  byte staticIP[4] = { 0, 0, 0, 0 };

  bool staticIPSetted() {
    for (int i = 0; i < 4; i++)
      if (staticIP[i] != 0) return true;
    return false;
  }
  printEthernetConnectionInfo() {
    Serial.println("Ethernet initiliazed");
    Serial.print(("localIP: "));
    Serial.println(Ethernet.localIP());
    Serial.print(("subnetMask: "));
    Serial.println(Ethernet.subnetMask());
    Serial.print(("gatewayIP: "));
    Serial.println(Ethernet.gatewayIP());
    Serial.print(("dnsServerIP: "));
    Serial.println(Ethernet.dnsServerIP());
  }
  printGetRequestResponseStatusCode(HttpClient httpClient) {
    Serial.print("Staus code: ");
    Serial.print(httpClient.responseStatusCode());
  }

public:
  uint8_t mac[6] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
  int digitalPin = 10;
  bool DHCPMode = false;
  bool autoMode = false;

  WebClient() {}
  WebClient(byte* _staticIP) {
    setStaticIP(_staticIP);
  }
  void setStaticIP(byte* _staticIP) {
    for (int i = 0; i < 4; i++)
      staticIP[i] = _staticIP[i];
  }
  void init() {
    Ethernet.init(digitalPin);
    Serial.println("Ethernet init on pin 10");
    if (!staticIPSetted() || DHCPMode) {
      Serial.println("Init with DHCP");
      while (!Ethernet.begin(mac)) {
        Serial.println("Init error in DHCP mode. Try again");
      }
    } else {
      if (autoMode) {
        Serial.println("Init auto mode");
        while (!Ethernet.begin(mac)) {
          Serial.println("Init error in DHCP mode. Try without DHCP");
          Ethernet.begin(mac, staticIP);
        }
      } else {
        Serial.println("Init without DHCP");
        Ethernet.begin(mac, staticIP);
      }
    }
    printEthernetConnectionInfo();
  }
  String makeGetRequest(String serverAddress, String request, int port) {
    HttpClient httpClient = HttpClient(ethernetClient, serverAddress, port);
    Serial.println("making GET request");
    httpClient.get(request);
    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    return response;
  }
};
