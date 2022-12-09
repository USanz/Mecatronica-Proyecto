#include <SoftwareSerial.h>
#include <AFMotor.h>

#include <Servo.h>



#define SERVO_CENTER 65
#define MAX_VEL 255
#define MAX_STRENGTH 100



Servo servo;  // Create servo object to control a servo.
              // twelve servo objects can be created on most boards.

SoftwareSerial BT(A0,A1); // We define Arduino RX and TX pins connected to Bluetooth.

AF_DCMotor left_motor(3);
AF_DCMotor right_motor(4);

int ang_range[2] = {50, 80};
int vel_range[2] = {-MAX_VEL, MAX_VEL};
int strength_range[2] = {0, MAX_STRENGTH};



void setup() {
  Serial.begin(9600);
  BT.begin(9600); //bluetooth attached to new pins (A0, A1) different from (RX=0, TX=1).

  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  servo.write(SERVO_CENTER);
  
  left_motor.setSpeed(0);
  right_motor.setSpeed(0);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);
}



void loop() {
  if(BT.available() > 0) {
    String value = BT.readStringUntil('#');
    if(value.length() == 7) {

      //get the information from bluetooth joystick app (Arduino Joystick / Arduino Bluetooth Controlled J in Play Store).
      int angle = value.substring(0, 3).toInt();
      int strength = value.substring(3, 6).toInt();
      String button = value.substring(6, 8);

      
      float angle_rad = angle * M_PI / 180;
      
      float right_cap = min(strength * cos(angle_rad), MAX_STRENGTH*cos(M_PI / 4)); // right virtual border.
      float joystick_x = max(right_cap, -MAX_STRENGTH*cos(M_PI / 4));               // left virtual border (negative).
      int servo_angle = (int) map(joystick_x, -MAX_STRENGTH*cos(M_PI / 4), MAX_STRENGTH*cos(M_PI / 4), ang_range[0], ang_range[1]);
      
      servo.write(servo_angle);


      float top_cap = min(strength * sin(angle_rad), MAX_STRENGTH*sin(M_PI / 4)); // top virtual border.
      float joystick_y = max(top_cap, -MAX_STRENGTH*sin(M_PI / 4));               // bottom virtual border (negative).
      int motor_vel = (int) map(joystick_y, -MAX_STRENGTH*sin(M_PI / 4), MAX_STRENGTH*sin(M_PI / 4), vel_range[0], vel_range[1]);
      
      if (motor_vel < 0) {
        left_motor.run(BACKWARD);
        right_motor.run(BACKWARD);
      } else {
        left_motor.run(FORWARD);
        right_motor.run(FORWARD);
      }
      
      left_motor.setSpeed(abs(motor_vel));
      right_motor.setSpeed(abs(motor_vel));


      ///DEBUG///
      /*
      Serial.print("servo_angle: ");
      Serial.print(servo_angle);
      Serial.println('\t');
      Serial.print("motor_vel:   ");
      Serial.print(motor_vel);
      Serial.println('\t');

      Serial.flush();
      //*/
      
      value = "";

    }
  }
}
