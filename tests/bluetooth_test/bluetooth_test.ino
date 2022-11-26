#include <SoftwareSerial.h>
#include <AFMotor.h>

SoftwareSerial BT(A0,A1); // Definimos los pines RX y TX del Arduino conectados al Bluetooth


AF_DCMotor motor(3);
AF_DCMotor motor4(4);

/*
void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

long int ROBOT_ON = 1;
long int ROBOT_OFF = 0;
long int TURN_LEFT = 0;
long int TURN_RIGHT = 0;

void loop() {
  if (BT.available()) {
    long data = BT.parseInt();
    if (data == ROBOT_ON) {
      digitalWrite(LED_BUILTIN, HIGH);
      //delay(1000);
      //Serial.print("Dato recivido: ");
      //Serial.println(data);
    } else {
      //Serial.print("nada");
      
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
*/



void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  
  motor.setSpeed(0);
  motor4.setSpeed(0);
 
  motor.run(RELEASE);
  motor4.run(RELEASE);
}

void loop() {
  if(BT.available() > 0) {
    String value = BT.readStringUntil('#');
    if(value.length() == 7) {
      String angle = value.substring(0, 3);
      String strength = value.substring(3, 6);
      String button = value.substring(6, 8);
      
      Serial.print("angle: ");
      Serial.print(angle);
      Serial.print('\t');
      
      Serial.print("strength: ");
      Serial.print(strength);
      Serial.print('\t');
      
      Serial.print("button: ");
      Serial.print(button);
      Serial.println("");
      
      Serial.flush();
      value="";

      if (button == "1") { //parao
        motor.run(FORWARD);
        motor4.run(FORWARD);
        motor.setSpeed(0);
        motor4.setSpeed(0);
      } else if (button == "4") { //movio
        motor.run(FORWARD);
        motor4.run(FORWARD);
        motor.setSpeed(220);
        motor4.setSpeed(220);
      }
    }
  }
}
