#include <SoftwareSerial.h>
#include <AFMotor.h>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

SoftwareSerial BT(A0,A1); // Definimos los pines RX y TX del Arduino conectados al Bluetooth


AF_DCMotor motor(3);
AF_DCMotor motor4(4);


void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(65);
  
  motor.setSpeed(0);
  motor4.setSpeed(0);
 
  motor.run(RELEASE);
  motor4.run(RELEASE);
}

float d;
float angle_rad;
int servo_angle;
int motor_vel;

void loop() {
  if(BT.available() > 0) {
    String value = BT.readStringUntil('#');
    if(value.length() == 7) {
      int angle = value.substring(0, 3).toInt();
      int strength = value.substring(3, 6).toInt();
      String button = value.substring(6, 8);

      if (angle == 0) {
        servo_angle = 65;
      } else {
        angle_rad = angle * M_PI / 180;

        float min_bbb = min(strength * cos(angle_rad), 100*cos(M_PI / 4));
        servo_angle = (int) map(max(min_bbb, -100*cos(M_PI / 4)), -100*cos(M_PI / 4), 100*cos(M_PI / 4), 40, 90);
      }
      myservo.write(servo_angle);

      float min_aaa = min(strength * sin(angle_rad), 100*sin(M_PI / 4));
      motor_vel = (int) map(max(min_aaa, -100*sin(M_PI / 4)), -100*sin(M_PI / 4), 100*sin(M_PI / 4), -255, 255);
      if (motor_vel < 0) {
        motor_vel *= -1;
        motor.run(BACKWARD);
        motor4.run(BACKWARD);
      } else {
        motor.run(FORWARD);
        motor4.run(FORWARD);
      }
      motor.setSpeed(motor_vel);
      motor4.setSpeed(motor_vel);
     
      
      //Serial.print("servo_angle: ");
      //Serial.print(servo_angle);
      //Serial.println('\t');
      Serial.print("motor_vel:   ");
      Serial.print(motor_vel);
      Serial.println('\t');

      /*
      Serial.print("angle: ");
      Serial.print(angle);
      Serial.print('\t');
      
      Serial.print("strength: ");
      Serial.print(strength);
      Serial.print('\t');
      
      Serial.print("button: ");
      Serial.print(button);
      Serial.println("");
      
      //*/
      Serial.flush();
      
      value="";

      if (button == "1") { //parao
        /*
        motor.run(FORWARD);
        motor4.run(FORWARD);
        motor.setSpeed(0);
        motor4.setSpeed(0);
        */
      } else if (button == "4") { //movio
        /*
        motor.run(FORWARD);
        motor4.run(FORWARD);
        motor.setSpeed(220);
        motor4.setSpeed(220);
        */
      }
    }
  }
}
