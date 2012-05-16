#include "ax12.h"
#include "Arduino.h"

#define NOUVELLE_ID 8

void setup()
{ 
  // on détecte un ax12
  AX12::init(1000000);
  int detect[1];
  AX12::autoDetect (detect, 1);

  // on l'initialise
  if (detect[0] != -1) {
    AX12 motor = AX12(detect[0]);
    motor.writeInfo(ID, NOUVELLE_ID);
  }
}

void loop()
{

}
