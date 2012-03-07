// dinamyxel ax-12 servo test for Arduino 0017

#define NOUSART1
#include <ax12.h>

AX12 motor[2] = {AX12 (), AX12 ()};                 // define 2 motores AX12

void setup()   {                
  
  pinMode(13, OUTPUT);
  Serial.begin (57600);                             // Serial0
  
  AX12::init (1000000);                             // AX12 à 1 Mb/s

  // on détecte automatiquement 2 AX12
  int detect[2];
  AX12::autoDetect (detect, 2);
  for (byte i=0; i<2; i++) {
    motor[i].id = detect[i];
  }
  
  motor_init();
}

void loop()   {
  int res = 0;
  
  for(int i=0;i<2;i++){
    // Affichage du moteur
    Serial.print("== Moteur ");
    Serial.print(i, DEC);
    Serial.println(" ==");
  
    // Donne une consigne aléatoire entre 0 et 1024
    res = motor[0].writeInfo (GOAL_POSITION, random(1024));
    
    // Affichage si erreur
    if(res > 0){
      Serial.print ("internal error ");
      Serial.println(res, DEC);
    }
    
    // Affichage de la rempérature
    Serial.print("temperature ");
    res = motor[0].readInfo(PRESENT_TEMPERATURE);
    Serial.println(motor[0].status_data, DEC);
    
    // Affichage de la position
    Serial.print("position ");
    res = motor[0].readInfo(PRESENT_POSITION);
    Serial.println(motor[0].status_data, DEC);
  }
  
  delay(500);
}


void motor_init () {  
  for (int i=0; i<2; i++) {
    motor[i].writeInfo (TORQUE_ENABLE, 1);               // habilita el torque
    motor[i].setEndlessTurnMode(false);                   // lo pone en modo de rotación continua
   // setear la inercia y todo eso
    motor[i].writeInfo (CW_COMPLIANCE_MARGIN, 0);
    motor[i].writeInfo (CCW_COMPLIANCE_MARGIN, 0);
    motor[i].writeInfo (CW_COMPLIANCE_SLOPE, 95);
    motor[i].writeInfo (CCW_COMPLIANCE_SLOPE, 95);
    motor[i].writeInfo (PUNCH, 150);
    motor[i].writeInfo (MAX_TORQUE, 1023);
    motor[i].writeInfo (LIMIT_TEMPERATURE, 85);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 60);
    motor[i].writeInfo (DOWN_LIMIT_VOLTAGE, 190);
    motor[i].writeInfo (RETURN_DELAY_TIME, 150);
  }
}

