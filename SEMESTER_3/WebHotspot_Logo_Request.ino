#include <SPI.h>
#include <WiFiNINA.h>
char ssid[] = "Brave";        // 아두이노가 접속할 무선공유기 SSID (name)
char pass[] = "samu6789";         // 아두이노가 접속할 무선공유기 비번

int station_status = WL_IDLE_STATUS;
char server[] =  "arduino.tips";    // name address (using DNS)
WiFiClient client;  // for http web server, port 80

void setup() {
  Serial.begin(9600);      // initialize serial communication  
  while (!Serial);    // wait for serial port to connect
  
  connectToAP();
  printWiFiStatus();  
  httpRequest();
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  } 
}

void connectToAP() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (station_status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    station_status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void httpRequest() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) { // port 80 for http
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /asciilogo.txt HTTP/1.1");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println((String)"Host: " + server);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}
