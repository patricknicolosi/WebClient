#include "WebClient.h"

WebClient webClient = WebClient();

void setup() {
  Serial.begin(9600);
}

void loop() {
  String serverResponse = webClient.makeGetRequest("192.168.10.1", "/?code=" , 80);
  Serial.println(serverResponse);
}
