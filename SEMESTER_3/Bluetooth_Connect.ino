/*

  -스마트폰에 LightBlue (iOS and Android)설치
  -A0에 가변저향 연결
  -D7에 서보모터 연결 (전원은 VIN, GND 핀에 연결)
  -https://www.uuidgenerator.net/ 에서 필요한 수 만큼 UUID 생성 (예제에서 3개 필요)
  -시리얼 모니터 샐행
  참고  
  https://create.arduino.cc/projecthub/8bitkick/sensor-data-streaming-with-arduino-683a6c
  https://create.arduino.cc/editor/dpajak/e7af8e95-0aff-4ce1-b2f7-4e7b446c2577/preview
*/


/*

// UUIDs

e526c3ad-9591-44e7-bcdc-dfc916f2f192

365f7de1-00dc-4cb4-a026-b1823155d71c

3a6bf87c-4af3-4b5c-a147-acb272c0d957

*/
/*

Blender Crashes Script etc...
https://blender.stackexchange.com/questions/120266/when-blender-crashes-during-rendering
*/



#include <ArduinoBLE.h>
#include <Servo.h>

unsigned long prev_time = 0;
unsigned long curr_time = 0;
const int led_pin = LED_BUILTIN; // pin to use for the LED
const int servo_pin = 7;
const int analog_pin = A0;

// 학번의 뒤 4자리가 8282라면 아래처럼 local_name 설정
char local_name[] = "BLE_5707_Control";

char arduino_service_UUID[] = "e526c3ad-9591-44e7-bcdc-dfc916f2f192";
char servo_char_UUID[] = "365f7de1-00dc-4cb4-a026-b1823155d71c";
char adc_char_UUID[] = "3a6bf87c-4af3-4b5c-a147-acb272c0d957";


BLEService arduinoService(arduino_service_UUID); // BLE control Service
BLEByteCharacteristic controlCharacteristic(servo_char_UUID, BLEWrite);
BLEIntCharacteristic adcCharacteristic(adc_char_UUID, BLERead | BLENotify);
Servo myservo;

void setup() {
  Serial.begin(9600);
  // while (!Serial);

  pinMode(led_pin, OUTPUT);
  myservo.attach(servo_pin);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // local name
  BLE.setLocalName(local_name);  
  BLE.setAdvertisedService(arduinoService);

  // add the characteristic to the service
  arduinoService.addCharacteristic(controlCharacteristic);
  arduinoService.addCharacteristic(adcCharacteristic);
  
  BLE.addService(arduinoService);       // add service
  controlCharacteristic.writeValue(0);  // set the initial value for the characeristic
  BLE.advertise();                      // start advertising
  Serial.println((String)local_name + " Peripheral starts");
}

void loop() {  
  // listen for BLE peripherals to connect:  
  BLEDevice central = BLE.central();    
  // if a central is connected to peripheral:
  if (central) {                        
    Serial.print("Connected to central: ");    
    // print the central's MAC address:
    Serial.println(central.address());  
    
    // while the central is connected to peripheral:
    while (central.connected()) {      
      curr_time = millis();
      if (curr_time - prev_time > 1000) {
        int adc = analogRead(analog_pin);        
        adcCharacteristic.writeValue(adc);                
        Serial.println(adc);
        prev_time = curr_time;        
      }      
      if (controlCharacteristic.written()) {
        char read_inp = controlCharacteristic.value();
        switch(read_inp) {
          case 'A':
            Serial.println("LED on");
            digitalWrite(led_pin, HIGH);        
            break;
          case 'B':
            Serial.println("LED off");
            digitalWrite(led_pin, LOW);
            break;
          case 'C':
            Serial.println("servo 0 deg");
            myservo.write(0);
            break;
          case 'D':
            Serial.println("servo 180 deg");
            myservo.write(180);
            break;
          case 'E':
            Serial.println("servo reset");
            myservo.write(90);
            break;
          defalult:
            Serial.println("Only A,B,C,D, and E are accepted");
            break;
        }
      }
    }
    // when the central disconnects, print it out:
    Serial.print((String)"Disconnected from central: " + local_name);
  }
}
