#include "SoftwareSerial.h"
#include "TinyStepper_28BYJ_48.h"

SoftwareSerial BT(2, 3);
char val;

TinyStepper_28BYJ_48 stepper;

int stepsPerRev = 2048;

void setup() {
  //Bluetooth and serial initialization
  Serial.begin(9600);
  Serial.println("BT Module initialized.");

  BT.begin(9600);
  delay(3000);
  BT.print("AT");

  //Stepper initialization
  stepper.connectToPins(8, 9, 10, 12);
  stepper.setSpeedInStepsPerSecond(600);
  stepper.disableMotor();
}
 
void loop() {
  //Sending to laptop
  if (Serial.available()) {
    val = Serial.read();
    BT.print(val);
  }

  //Recieving from laptop
  if (BT.available()) {
    String command = "";

    while (BT.available()) {
      char val = BT.read();
      command += val;
      delay(2);
    }

    if (command != "") {
      String messageStr;

      if (command.startsWith("rhs")) {
        double parameter = extractParameter(command);
        
        sendMessage("Rotating (h) stepper " + String(parameter) + " revolution(s)");

        stepper.moveRelativeInSteps(parameter * stepsPerRev);
        stepper.disableMotor();
        sendMessage("Rotation finished");
      } else {
        sendMessage("Command not recognized");
      }
    }
  }
}

void sendMessage(String messageStr) {
  BT.print(messageStr + "xyz");
}

double extractParameter(const String& command) {
  int spacePosition = command.lastIndexOf(' ');
  if (spacePosition != -1) return command.substring(spacePosition + 1).toDouble();

  return 0.0;
}