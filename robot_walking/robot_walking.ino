/*-----------------------------------------------------------------------------------------------------
  ----------------------------------------------------------------------------------------------------
  ---------------------------------------------------------------------------------------------------

            _____   ____   _____ ____ ____  __  ___
           |  __ \ / __ \ / ____/ __ \___ \/_ |/ _ \
           | |__) | |  | | |   | |  | |__) || | (_) |
           |  _  /| |  | | |   | |  | |__ < | |> _ <
           | | \ \| |__| | |___| |__| |__) || | (_) |
           |_|  \_\\____/ \_____\____/____/ |_|\___/
                    SIMPLE SERVO CONTROLLER

  This is some super simple code for controlling servos via id and angle values.
  This has been made for the Arduino Nano with a PCA9685 16 channel i2c PWM breakout board.

                         -:Wiring:-
                ARDUINO NANO --->    PCA9685
                     A5      --->      SCL
                     A4      --->      SDA
                    GND      --->      GND
                     5V      --->      VCC

  THIS CODE UTILISES THE ADAFRUIT PWM SERVO DRIVER LIBRARY, TO ADD IT TO YOUR ARDUINO IDE:
  - Click on; Sketch
  - Click on; Include Library
  - Click on; Manage Libraries...
  - In the search bar that appears, type in "Adafruit PWM Servo Driver" and click install on the one that appears with the same name.
  Now you have the neccessary libraries to run the following code.

                     -:USING THE CODE:-
                      ----------------
  When uploaded you can then send simple commands to the board to control your servos.
  It must be in the following format:
    ServoNumber,ServoAngle

    So for example:

    UpdateServo(6, 90);

    This will move servo number 6 to an angle of 90 degrees.

  That is everything you need to know to use the board

  Debugging:

  If you send your servo an angle value and it does not line up, then you may need to calibrate the servo minimum and maximum pulse lengths
  Currently they have been calibrated for a range of 0 to 180 degrees for the Towerpro MG996R
  #define SERVOMIN  135 // This is the 'minimum' pulse length count (out of 4096) For the MG996R this wants to be 135
  #define SERVOMAX  482 // this is the 'maximum' pulse length count (out of 4096) For the MG996R this wants to be 482

  ------------------------------------------------------------------------------------------------
  -------------------------------------------------------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  135 // This is the 'minimum' pulse length count (out of 4096) For the MG996R this wants to be 135
#define SERVOMAX  482 // this is the 'maximum' pulse length count (out of 4096) For the MG996R this wants to be 482

Adafruit_PWMServoDriver PCA9685 = Adafruit_PWMServoDriver();

int ServoPositions[16][5]; //you can use this to make a 2D array of values for your 16 servos positions for each point of your walking gait

//Macros
#define FRH1  0
#define FRH2  1
#define FRK   2
#define FRA   3

#define FLH1  4
#define FLH2  5
#define FLK   6
#define FLA   7

#define BRH1  8
#define BRH2  9
#define BRK   11
#define BRA   10

#define BLH1  12
#define BLH2  13
#define BLK   15
#define BLA   14

int leg0[4] = {FRH1, FRH2, FRK, FRA};
int leg1[4] = {FLH1, FLH2, FLK, FLA};
int leg2[4] = {BRH1, BRH2, BRK, BRA};
int leg3[4] = {BLH1, BLH2, BLK, BLA};

enum legside {
  LEFT,
  RIGHT
};

enum legGroup {
  ALPHA,
  BRAVO
};





//----------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); //Nice faster speed
  PCA9685.begin();
  PCA9685.setPWMFreq(50); //Maximum speed this can be set to is 1600
  Wire.setClock(400000); // Fast 400khz I2C Mode
  Serial.println("PCA9685 Servo Controller Connected & Ready");

  home();

  delay(1000);

  // step 1
  leanLeg(leg0);
  delay(1000);
  legStep(leg3);
  delay(1000);
  footDown(0);
  delay(1000);

  // step 2
  leanLeg(leg2);
  delay(1000);
  legStep(leg1);
  delay(1000);
  footDown(2);
  delay(1000);

  //step 3
  leanLeg(leg3);
  delay(1000);
  legStep(leg0);
  delay(1000);
  footDown(3);
  delay(1000);

  // step 4
  //step 3
  leanLeg(leg1);
  delay(1000);
  legStep(leg2);
  delay(1000);
  footDown(1);
  delay(1000);

  shuffleForwards();


}


/*----------------------------------------------------------------------------
   __  __          _____ _   _
  |  \/  |   /\   |_   _| \ | |
  | \  / |  /  \    | | |  \| |
  | |\/| | / /\ \   | | | . ` |
  | |  | |/ ____ \ _| |_| |\  |
  |_|  |_/_/    \_\_____|_| \_|
  ----------------------------------------------------------------------------*/
void loop() {
}
//End of Main
//----------------------------------------------------------------------------//






/*----------------------------------------------------------------------------
   ______ _    _ _   _  _____ _______ _____ ____  _   _  _____
  |  ____| |  | | \ | |/ ____|__   __|_   _/ __ \| \ | |/ ____|
  | |__  | |  | |  \| | |       | |    | || |  | |  \| | (___
  |  __| | |  | | . ` | |       | |    | || |  | | . ` |\___ \
  | |    | |__| | |\  | |____   | |   _| || |__| | |\  |____) |
  |_|     \____/|_| \_|\_____|  |_|  |_____\____/|_| \_|_____/
  ----------------------------------------------------------------------------*/

void updateServo (int Id, int Angle) {

  double pulselength = map(Angle, 0, 180, SERVOMIN, SERVOMAX); //map the degree value to pulselength value
  PCA9685.setPWM(Id, 0, pulselength); //Send the angle to the appropiate servo

}

void footDown(int leg) {
  if (leg == 0) {
    updateServo(1, 130);
    updateServo(2, 75);
  }
  else if (leg == 1) {
    updateServo(5, 130);
    updateServo(6, 60);
  }
  else if (leg == 2) {
    updateServo(9, 140);
    updateServo(11, 90);
  }
  else if (leg == 3) {
    updateServo(13, 140);
    updateServo(15, 60);
  }
}


void home(int leg) {
  if (leg == 0) {
    updateServo(0, 70);
    updateServo(1, 130);
    updateServo(2, 75);
    updateServo(3, 87);
  }
  else if (leg == 1) {
    updateServo(4, 125);
    updateServo(5, 130);
    updateServo(6, 60);
    updateServo(7, 110);
  }
  else if (leg == 2) {
    updateServo(8, 130);
    updateServo(9, 140);
    updateServo(10, 90);
    updateServo(11, 70);
  }
  else {
    updateServo(12, 70);
    updateServo(13, 140);
    updateServo(14, 95);
    updateServo(15, 60);
  }
}

void home() {
  updateServo(0, 70);
  updateServo(1, 130);
  updateServo(2, 75);
  updateServo(3, 87);

  updateServo(4, 130);
  updateServo(5, 130);
  updateServo(6, 60);
  updateServo(7, 110);

  updateServo(8, 130);
  updateServo(9, 140);
  updateServo(10, 90);
  updateServo(11, 70);

  updateServo(12, 70);
  updateServo(13, 140);
  updateServo(14, 95);
  updateServo(15, 60);
}

void leanLeg(int leg[]) {
  updateServo(leg[1], 90);
  updateServo(leg[2], 110);
}

void shuffleForwards(){
  updateServo(FRH1, 70);
  updateServo(FLH1, 130);
  updateServo(BRH1, 130);
  updateServo(BLH1, 70);
}

void legStep(int leg[]) {
  int movement[3] = {0, 0, 0};
  movement[1] = 100;
  movement[2] = 100;

  if (leg[0] == 0) {
    movement[0] = 40;
  } else if (leg[0] == 4) {
    movement[0] = 160;
  } else if (leg[0] == 8){
    movement[0] = 90;
  } else if (leg[0] == 12){
    movement[0] = 110;
  }

  // lift foot
  updateServo(leg[1], movement[1]);
  updateServo(leg[2], movement[2]);
  delay(500);

  // take step here
  updateServo(leg[0], movement[0]);

  delay(500);

  if (leg[0] == 0) {
    footDown(0);
  }
  else if (leg[0] == 4) {
    footDown(1);
  }
  else if (leg[0] == 8) {
    footDown(2);
  } else if (leg[0] == 12) {
    footDown(3);
  }


}

void moveServo(int servoNum, int startingPos, int endPos) {
  if (startingPos < endPos) {
    for (int i = startingPos; i <= endPos; i++) {
      updateServo(servoNum, i);
    }
  }
  else {
    for (int i = startingPos; i >= endPos; i--) {
      updateServo(servoNum, i);
    }
  }
}
