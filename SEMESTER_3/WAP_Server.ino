#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "SazServer"; // SSID로 자신의 팀번호와 학번을 사용하여 변경
char pass[] = "mypass";       // pass로 해당 번호 그대로 사용
char my_name[] = "sazaam";     // 자신의 닉네임으로 변경

int AP_status = WL_IDLE_STATUS;
WiFiServer local_server(80);          // create Web server, HTTP port is 80

void setup() {  
  Serial.begin(9600); //Initialize serial and wait for port to open:  
  while (!Serial);    // wait for serial port to connect. Needed for native USB port only
    
  createAP(); // create access point
  local_server.begin();   // start the web server on port 80
  printWiFiStatus(); // print out the status
}

void loop() {
  checkAPConn();
  responseClientConn();
}

void createAP() {
  Serial.println("Create Access Point Web Server");
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
  // by default the local IP address of will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));  
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  AP_status = WiFi.beginAP(ssid, pass);
  if (AP_status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");    
    while (true);   // don't continue
  }  
  delay(10000);   // wait 10 seconds for connection:    
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void checkAPConn() {
  // compare the previous status to the current status
  if (AP_status != WiFi.status()) {
    // it has changed update the variable
    AP_status = WiFi.status();
    if (AP_status == WL_AP_CONNECTED) {
      // a device has connected to this AP
      Serial.println("Some device connected to this AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
}  

void responseClientConn() {  
  WiFiClient client = local_server.available();   // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("New client");           // print a message out the serial port
    String currentLine = "";                
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // 클라이언트로부터 byte를 수신
        Serial.print(c);
        if (c == '\n') {                    // if the byte is a newline character
          // 클라이언트의 request가 끝나는 지점은 \n\n, 즉 newline이 두번 연속 들어오는 시점임
          // 따라서 \n이 들어왔고 currentLine이 0이라면 직전 라인에서도 \n이 들어왔다는 뜻임!
          if (currentLine.length() == 0) {
            // 클라이언트의 request가 끝났으므로 서버가 response를 보냄
            sendHttpResponse(client);  
            break;      // while 루프에서 빠져나와서 clinet를 close함
          } else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {    // 캐리지 리턴이 아니라면 라인의 끝이 아니므로 계속 추가함
          currentLine += c;      // 한 라인씩 저장함
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void sendHttpResponse(WiFiClient& client) {    
  Serial.println("Send HTML to client");            
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  // HTTP data
  client.println("<html>");
  client.println((String)"<h1> Welcome " + my_name + " to IT proj 3! </h1>");
  client.println("<h2> Hope you enjoy this class :-) </h2>");
  client.println("</html>");

  // The HTTP response ends with another blank line:
  client.println();  
}
