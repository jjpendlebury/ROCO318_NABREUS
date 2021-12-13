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




//----------------------------------------------------------------------------//
void setup() {
  Serial.begin(115200); //Nice faster speed
  PCA9685.begin();
  PCA9685.setPWMFreq(50); //Maximum speed this can be set to is 1600
  Wire.setClock(400000); // Fast 400khz I2C Mode
  Serial.println("PCA9685 Servo Controller Connected & Ready");

  home();

  delay(1000);

  oneLegLift();

  delay(1000);

  updateServo(12, 100);
  updateServo(13, 100);
  updateServo(14, 95);
  updateServo(15, 100);

  delay(500);

  updateServo(12, 100);
  updateServo(13, 140);
  updateServo(14, 95);
  updateServo(15, 60);

  delay(500);

  updateServo(5, 100);
  updateServo(6, 95);
  delay(500);
  updateServo(4, 150);
  delay(500);
  updateServo(5, 130);
  updateServo(6, 75);
  delay(500);
//  updateServo(8, 125);
  updateServo(1, 100);
  updateServo(2,95);
  delay(500);
  updateServo(0, 40);
  delay(500);
  updateServo(1, 125);
  updateServo(2, 75);
  delay(500);
    updateServo(12, 65);
  delay(500);
  updateServo(BRH2, 100);
  updateServo(BRH1, 100);
  delay(100);
  updateServo(BRH2, 130);
  delay(500);
 
//  forward sweep??
  updateServo(0, 125);
  updateServo(4, 70);
//beggining of step 2
  delay(500);
  updateServo(FRH2, 100);
  updateServo(FRH1, 70);
  delay(100);
  updateServo(FRH2, 125);
  delay(500);
  updateServo(FLH2, 100);
  updateServo(FLH1, 120);
  updateServo(FLK, 95);
  delay(100);
  updateServo(FLK, 60);
  updateServo(FLH2, 130);
  
  
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

void oneLegLift() {
  updateServo(0, 70);
  updateServo(1, 130);
  updateServo(2, 75);
  updateServo(3, 87);
  delay(500);
  updateServo(4, 90);
  updateServo(5, 130);
  updateServo(6, 75);
  updateServo(7, 110);
  delay(500);
  updateServo(8, 135);
  updateServo(9, 145);
  updateServo(10, 90);
  updateServo(11, 70);
  delay(500);
  updateServo(12, 80);
  updateServo(13, 140);
  updateServo(14, 95);
  updateServo(15, 100);
}

void home() {
  updateServo(0, 70);
  updateServo(1, 130);
  updateServo(2, 75);
  updateServo(3, 87);

  updateServo(4, 125);
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
