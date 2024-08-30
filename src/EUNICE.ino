//LIBRERIAS
  #include <SoftwareSerial.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <MPU6050_light.h>
  #include <Servo.h>
  #include "HUSKYLENS.h" 
  #include "I2Cdev.h"

//PINES
  #define B_TOUCH 12
  #define TRIG_1 11
  #define ECO_1 10
  #define ECO_2 9
  #define TRIG_2 8
  #define TRIG_3 7
  #define ECO_3 6
  #define IN1 5
  #define IN2 4
  #define ENB 3
  // SDA-A4  SCL-A5  SERVO-13  TOUCH-12  I_TRIG-11  I_ECO-10  F_TRIG-8  F_ECO-9  D_TRIG-7  D_ECO-6  IN1-5  IN2-4  ENB-3  A4-VERDE_1  A5-AZUL_0  

//OBJETOS
  HUSKYLENS huskylens;
  MPU6050 mpu(Wire);
  Servo GIRO;

//PUENTE H
  int PWM = 0;
  int VELOCIDAD = 0;

//GIROSCOPIO
  unsigned long timer = 0;
  int INICIAL;
  int CONTINUO;
  float ANGULO;
  int ZONA = 0;
  int VUELTA = 1;

//SERVO
  int L_DERECHA = 170;
  int L_IZQUIERDA = 50;
  int L_CENTRO_I = 160;
  int L_CENTRO_D = 60;
  int L_CENTRO = 100;

//ULTRASONICOS
  int DURACION_1 = 0;
  int DURACION_2 = 0;
  int DURACION_3 = 0;
  int DIST_FRENTE = 0;
  int DIST_IZQUIERDA = 0;
  int DIST_DERECHA = 0;
  int time = 58.2;

//TACTIL
  int TACTIL;
  int ESTADO_BOTON = 0;
  int ACTIVO = 0;

//CAMARA
  int X = 0;
  int VERDE;
  int Y = 0;
  int ROJO = 0;
  int X_CENTRO = 160;
  int Y_CENTRO = 120;

//DIRECCIÓN
  int IZQUIERDA = 0;
  int DERECHA = 0;
  int PARED = 0;
  int EXTRA = 0;

//PID
  int ET; 
  int SD;
  int SM;
  int dE;
  int Ep;
  int iE;
  int Eip;
  int Ei;
  int PID;
  float t;
  int ET_2; 
  int SD_2;
  int SM_2;
  int dE_2;
  int Ep_2;
  int iE_2;
  int Eip_2;
  int Ei_2;
  int PID_2;
  float t_2;
  int KP = 1;
  float KI = 0.1;
  float KD = 0.1;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();    
  byte status = mpu.begin();    
  Serial.println(status);
  while(status!=0){ }
  mpu.calcGyroOffsets();
  Serial.println("INICIO");
  while(status!=0){ };

  while (!huskylens.begin(Wire)){
    Serial.println(F("¡COMUNICACIÓN FALLIDA!"));
    Serial.println(F("1.POR FAVOR REVISE EL PROTOCOLO \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    Serial.println(F("2.POR FAVOR REVISE LA CONEXIÓN.")); 
    delay(100);
  }

  pinMode(TRIG_1,OUTPUT);
  pinMode(ECO_1, INPUT);
  pinMode(TRIG_2,OUTPUT);
  pinMode(ECO_2, INPUT);
  pinMode(TRIG_3,OUTPUT);
  pinMode(ECO_3, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(B_TOUCH, INPUT);
  GIRO.attach(13);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, 0);
  delay(2000);
  GIRO.write(L_CENTRO);
  delay(2000);
  Serial.println("LISTO");
 }


void loop() {
  //**************************************INICIO**************************************
    //ARRANQUE
      TACTIL = digitalRead(B_TOUCH);
      if(TACTIL == HIGH){
        delay(20);
        TACTIL = digitalRead(B_TOUCH);
        if(TACTIL == LOW && ESTADO_BOTON == 1){
          ESTADO_BOTON = 0;
          Serial.println("ESTADO = 0");
          delay(1000);
        }
        if(TACTIL == LOW && ESTADO_BOTON == 0){
          ESTADO_BOTON = 1;
          Serial.println("ESTADO = 1");
          delay(1000);
        }
      }

    //SENTIDO
      if(ESTADO_BOTON == 1){
        if(PARED == 0 && ACTIVO == 0){
          if(DIST_FRENTE > 101 || DIST_FRENTE == 0){
            VELOCIDAD = 50; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
          } 
          digitalWrite(TRIG_2, HIGH);     
          delay(1);        
          digitalWrite(TRIG_2, LOW);
          DURACION_2 = pulseIn(ECO_2, HIGH);     
          DIST_FRENTE = DURACION_2 / time;
          Serial.println(DIST_FRENTE);
          delay(10);
          if(DIST_FRENTE <= 100 && DIST_FRENTE > 0){
            VELOCIDAD = 0; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM); ACTIVO = 1;
          }
        }
        if(ACTIVO == 1){
          digitalWrite(TRIG_1, HIGH); 
          delay(1);
          digitalWrite(TRIG_1, LOW);
          DURACION_1 = pulseIn(ECO_1, HIGH);
          DIST_IZQUIERDA = DURACION_1 / time; 
          digitalWrite(TRIG_3, HIGH);     
          delay(1);        
          digitalWrite(TRIG_3, LOW);
          DURACION_3 = pulseIn(ECO_3, HIGH);     
          DIST_DERECHA = DURACION_3 / time;
          if(DIST_DERECHA > 0 && DIST_IZQUIERDA > 0){
            if(DIST_DERECHA > DIST_IZQUIERDA){
              PARED = 1; IZQUIERDA = 0; DERECHA = 1;//GIRO A LA DERECHA
            }
            if(DIST_IZQUIERDA > DIST_DERECHA){
              PARED = 2; IZQUIERDA = 1; DERECHA = 0;//GIRO A LA IZQUIERDA
            }
            if(PARED == 1 && DERECHA == 1){
              GIRO.write(L_DERECHA); delay(500); EXTRA = 0;
            }
            if(PARED == 2 && IZQUIERDA == 1){
              GIRO.write(L_IZQUIERDA); delay(500); EXTRA = 200;
            }
            VELOCIDAD = 50; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
            delay(1300 + EXTRA);
            if(DERECHA == 1){
              GIRO.write(L_IZQUIERDA); delay(100); GIRO.write(L_CENTRO_D); delay(100); ACTIVO = 2;
            }
            if(IZQUIERDA == 1){
              GIRO.write(L_DERECHA); delay(100); GIRO.write(L_CENTRO_I); delay(100); ACTIVO = 2;
            }
            VELOCIDAD = 80; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
          }
        }
      }
  //**************************************SENSORES**************************************
    if(ACTIVO == 2){
      //VERIFICACIÓN
        Serial.print("DISTANCIA IZQUIERDA: "); Serial.print(DIST_IZQUIERDA); Serial.println("cm");
        Serial.print("DISTANCIA DERECHA: "); Serial.print(DIST_DERECHA); Serial.println("cm");
        Serial.print("DISTANCIA FRENTE: "); Serial.print(DIST_FRENTE); Serial.println("cm");
        Serial.print("ANGULO: "); Serial.print(ANGULO); Serial.println("°");
        Serial.print("ZONA: "); Serial.print(ZONA);
        Serial.print("VUELTA: "); Serial.print(VUELTA); Serial.println(" de 3");
        Serial.print("VELOCIDAD: "); Serial.print(VELOCIDAD); Serial.println("%");
        Serial.println(String()+F("VERDE X = ")+X+F(" ")+VERDE);
        Serial.println(String()+F("ROJO X = ")+X+F(" ")+ROJO);

      //GIROSCOPIO
        if(INICIAL == 0){
          mpu.update();  
          if((millis()-timer)>100){
            INICIAL = (mpu.getAngleZ());
            timer = millis(); 
          }
        }
        mpu.update();  
        if((millis()-timer)>10){  
          CONTINUO = (mpu.getAngleZ());
          timer = millis(); 
          delay(50);
          ANGULO = (CONTINUO - INICIAL);
          if(PARED == 2){ //IZQUIERDA
            ANGULO = ANGULO;
          } 
          if(PARED == 1){ //DERECHA
            ANGULO = (-1*(ANGULO));
          }                                       
          if(ANGULO <= 90 && ANGULO >=1){
            ZONA = 1;
          }

          if(ANGULO <= 180 && ANGULO >= 91){
            ZONA = 2;
          }

          if(ANGULO <= 270 && ANGULO >= 181){
            ZONA = 3;
          }

          if(ANGULO <= 360 && ANGULO >= 271){
            ZONA = 4;
          }
          if(ANGULO <= 450 && ANGULO >=361){
            ZONA = 5;
          }

          if(ANGULO <= 540 && ANGULO >= 451){
            ZONA = 6;
          }

          if(ANGULO <= 630 && ANGULO >= 541){
            ZONA = 7;
          }

          if(ANGULO <= 720 && ANGULO >= 631){
            ZONA = 8;
            VUELTA = 2;
          }
          if(ANGULO <= 810 && ANGULO >= 721){
            ZONA = 9;
          }

          if(ANGULO <= 900 && ANGULO >= 811){
            ZONA = 10;
          }

          if(ANGULO <= 990 && ANGULO >= 901){
            ZONA = 11;
          }

          if(ANGULO <= 1080 && ANGULO >= 991){
            ZONA = 12;
            VUELTA = 3;
          }
          if(ANGULO <= 1170 && ANGULO >= 1081){
            ZONA = 13;
            VUELTA = 4;
          }
        }

      //ULTRASONICO
        digitalWrite(TRIG_1, HIGH); 
        delay(1);
        digitalWrite(TRIG_1, LOW);
        DURACION_1 = pulseIn(ECO_1, HIGH);
        DIST_IZQUIERDA = DURACION_1 / time; 
        digitalWrite(TRIG_2, HIGH); 
        delay(1);
        digitalWrite(TRIG_2, LOW); 
        DURACION_2 = pulseIn(ECO_2, HIGH);
        DIST_FRENTE = DURACION_2 / time;
        digitalWrite(TRIG_3, HIGH);     
        delay(1);        
        digitalWrite(TRIG_3, LOW);
        DURACION_3 = pulseIn(ECO_3, HIGH);     
        DIST_DERECHA = DURACION_3 / time;
      //HUSKYLENS
        if (!huskylens.request()){ Serial.println(F("¡Comunicación fallida , revise conexiones fisicas!"));}
        else if(!huskylens.isLearned()){ Serial.println(F("¡No tiene ningun objeto para elegir, elija uno!"));}
        else if(!huskylens.available()){ Serial.println(F("¡No aparecen bloques en la pantalla!")); X = 0; Y = 0; }
        else{
          Serial.println(F("###########")); 
          if (huskylens.available()){
            HUSKYLENSResult result = huskylens.read(); printResult(result);
          }
        }

  //**************************************CONTROL**************************************
      if(ZONA != 13 && VUELTA != 4){
        //PARED 1
          if(PARED == 1){
            //VARIABLES
              SM = (DIST_IZQUIERDA);
              SD = (20);
            //PID
              ET = SD - SM; //ERROR PROPORCIONAL 
              dE = (ET - Ep)/t; //ERROR DIFERENCIAL
              Ep = ET;
              iE = Eip + (t*ET); //ERROR INTEGRAL
              Eip = Ei;
              t = t + (0.053 + (PID/100)); //TIEMPO 
              PID = (KP*ET)+(KI*iE)+(KD*dE); //PID
              if(DIST_IZQUIERDA < 40 || DIST_IZQUIERDA < 0){
                GIRO.write(L_IZQUIERDA);
                delay(PID*10);
              }
                //ULTRASONICO
                  digitalWrite(TRIG_1, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_1, LOW);
                  DURACION_1 = pulseIn(ECO_1, HIGH);
                  DIST_IZQUIERDA = DURACION_1 / time; 
                  digitalWrite(TRIG_2, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_2, LOW); 
                  DURACION_2 = pulseIn(ECO_2, HIGH);
                  DIST_FRENTE = DURACION_2 / time;
                  digitalWrite(TRIG_3, HIGH);     
                  delay(1);        
                  digitalWrite(TRIG_3, LOW);
                  DURACION_3 = pulseIn(ECO_3, HIGH);     
                  DIST_DERECHA = DURACION_3 / time;
              if(DIST_DERECHA < 40  || DIST_DERECHA < 0){
                GIRO.write(L_IZQUIERDA);
                delay(PID*5);
              }
            //VARIABLES
              SM_2 = (ANGULO);
              SD_2 = (90*ZONA);
            //PID
              //GIROSCOPIO
                if(INICIAL == 0){
                  mpu.update();  
                  if((millis()-timer)>100){
                    INICIAL = (mpu.getAngleZ());
                    timer = millis(); 
                  }
                }
                mpu.update();  
                if((millis()-timer)>10){  
                  CONTINUO = (mpu.getAngleZ());
                  timer = millis(); 
                  delay(50);
                  ANGULO = (CONTINUO - INICIAL);
                  if(PARED == 2){ //IZQUIERDA
                    ANGULO = ANGULO;
                  } 
                  if(PARED == 1){ //DERECHA
                    ANGULO = (-1*(ANGULO));
                  }                                       
                  if(ANGULO <= 90 && ANGULO >=1){
                    ZONA = 1;
                  }

                  if(ANGULO <= 180 && ANGULO >= 91){
                    ZONA = 2;
                  }

                  if(ANGULO <= 270 && ANGULO >= 181){
                    ZONA = 3;
                  }

                  if(ANGULO <= 360 && ANGULO >= 271){
                    ZONA = 4;
                  }
                  if(ANGULO <= 450 && ANGULO >=361){
                    ZONA = 5;
                  }

                  if(ANGULO <= 540 && ANGULO >= 451){
                    ZONA = 6;
                  }

                  if(ANGULO <= 630 && ANGULO >= 541){
                    ZONA = 7;
                  }

                  if(ANGULO <= 720 && ANGULO >= 631){
                    ZONA = 8;
                    VUELTA = 2;
                  }
                  if(ANGULO <= 810 && ANGULO >= 721){
                    ZONA = 9;
                  }

                  if(ANGULO <= 900 && ANGULO >= 811){
                    ZONA = 10;
                  }

                  if(ANGULO <= 990 && ANGULO >= 901){
                    ZONA = 11;
                  }

                  if(ANGULO <= 1080 && ANGULO >= 991){
                    ZONA = 12;
                    VUELTA = 3;
                  }
                  if(ANGULO <= 1170 && ANGULO >= 1081){
                    ZONA = 13;
                    VUELTA = 4;
                  }
                }
              
                if(INICIAL == 0){
                  mpu.update();  
                  if((millis()-timer)>100){
                    INICIAL = (mpu.getAngleZ());
                    timer = millis(); 
                  }
                }
                mpu.update();  
                if((millis()-timer)>10){  
                  CONTINUO = (mpu.getAngleZ());
                  timer = millis(); 
                  delay(50);
                  ANGULO = (CONTINUO - INICIAL);
                  if(PARED == 2){ //IZQUIERDA
                    ANGULO = ANGULO;
                  } 
                  if(PARED == 1){ //DERECHA
                    ANGULO = (-1*(ANGULO));
                  }                                       
                  if(ANGULO <= 90 && ANGULO >=1){
                    ZONA = 1;
                  }

                  if(ANGULO <= 180 && ANGULO >= 91){
                    ZONA = 2;
                  }

                  if(ANGULO <= 270 && ANGULO >= 181){
                    ZONA = 3;
                  }

                  if(ANGULO <= 360 && ANGULO >= 271){
                    ZONA = 4;
                  }
                  if(ANGULO <= 450 && ANGULO >=361){
                    ZONA = 5;
                  }

                  if(ANGULO <= 540 && ANGULO >= 451){
                    ZONA = 6;
                  }

                  if(ANGULO <= 630 && ANGULO >= 541){
                    ZONA = 7;
                  }

                  if(ANGULO <= 720 && ANGULO >= 631){
                    ZONA = 8;
                    VUELTA = 2;
                  }
                  if(ANGULO <= 810 && ANGULO >= 721){
                    ZONA = 9;
                  }

                  if(ANGULO <= 900 && ANGULO >= 811){
                    ZONA = 10;
                  }

                  if(ANGULO <= 990 && ANGULO >= 901){
                    ZONA = 11;
                  }

                  if(ANGULO <= 1080 && ANGULO >= 991){
                    ZONA = 12;
                    VUELTA = 3;
                  }
                  if(ANGULO <= 1170 && ANGULO >= 1081){
                    ZONA = 13;
                    VUELTA = 4;
                  }
                }
              ET_2 = SD_2 - SM_2; //ERROR PROPORCIONAL 
              dE_2 = (ET_2 - Ep_2)/t_2; //ERROR DIFERENCIAL
              Ep_2 = ET_2;
              iE_2 = Eip_2 + (t_2*ET_2); //ERROR INTEGRAL
              Eip_2 = Ei_2;
              t_2 = t_2 + (0.053 + (PID_2/100)); //TIEMPO 
              PID_2 = (KP*ET_2)+(KI*iE_2)+(KD*dE_2); //PID
              if(PID > 0){
                GIRO.write(L_IZQUIERDA);
                delay(PID*10);
              }
              if(PID < 0){
                GIRO.write(L_IZQUIERDA);
                delay(PID*5);
              }
                //ULTRASONICO
                  digitalWrite(TRIG_1, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_1, LOW);
                  DURACION_1 = pulseIn(ECO_1, HIGH);
                  DIST_IZQUIERDA = DURACION_1 / time; 
                  digitalWrite(TRIG_2, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_2, LOW); 
                  DURACION_2 = pulseIn(ECO_2, HIGH);
                  DIST_FRENTE = DURACION_2 / time;
                  digitalWrite(TRIG_3, HIGH);     
                  delay(1);        
                  digitalWrite(TRIG_3, LOW);
                  DURACION_3 = pulseIn(ECO_3, HIGH);     
                  DIST_DERECHA = DURACION_3 / time;
              if(DIST_FRENTE < 80 && DIST_FRENTE > 0){ //GIRO
                VELOCIDAD = 50; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
                GIRO.write(L_DERECHA); delay(500);
                VELOCIDAD = 50; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
                delay(1300);
                GIRO.write(L_IZQUIERDA); delay(100); GIRO.write(L_CENTRO_D); delay(100); ACTIVO = 2;
                VELOCIDAD = 80; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
              }
          }
        //PARED 2
          if(PARED == 2){
            //VARIABLES
              SM = (DIST_DERECHA);
              SD = (20);
            //PID
              ET = SD - SM; //ERROR PROPORCIONAL 
              dE = (ET - Ep)/t; //ERROR DIFERENCIAL
              Ep = ET;
              iE = Eip + (t*ET); //ERROR INTEGRAL
              Eip = Ei;
              t = t + (0.053 + (PID/100)); //TIEMPO 
              PID = (KP*ET)+(KI*iE)+(KD*dE); //PID
              if(DIST_DERECHA < 40 || DIST_DERECHA < 0){
                GIRO.write(L_IZQUIERDA);
                delay(PID*10);
              }
                //ULTRASONICO
                  digitalWrite(TRIG_1, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_1, LOW);
                  DURACION_1 = pulseIn(ECO_1, HIGH);
                  DIST_IZQUIERDA = DURACION_1 / time; 
                  digitalWrite(TRIG_2, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_2, LOW); 
                  DURACION_2 = pulseIn(ECO_2, HIGH);
                  DIST_FRENTE = DURACION_2 / time;
                  digitalWrite(TRIG_3, HIGH);     
                  delay(1);        
                  digitalWrite(TRIG_3, LOW);
                  DURACION_3 = pulseIn(ECO_3, HIGH);     
                  DIST_DERECHA = DURACION_3 / time;
              if(DIST_IZQUIERDA < 40  || DIST_IZQUIERDA < 0){
                GIRO.write(L_DERECHA);
                delay(PID*5);
              }
            //VARIABLES
              SM_2 = (ANGULO);
              SD_2 = (90*ZONA);
            //PID
              //GIROSCOPIO
                if(INICIAL == 0){
                  mpu.update();  
                  if((millis()-timer)>100){
                    INICIAL = (mpu.getAngleZ());
                    timer = millis(); 
                  }
                }
                mpu.update();  
                if((millis()-timer)>10){  
                  CONTINUO = (mpu.getAngleZ());
                  timer = millis(); 
                  delay(50);
                  ANGULO = (CONTINUO - INICIAL);
                  if(PARED == 2){ //IZQUIERDA
                    ANGULO = ANGULO;
                  } 
                  if(PARED == 1){ //DERECHA
                    ANGULO = (-1*(ANGULO));
                  }                                       
                  if(ANGULO <= 90 && ANGULO >=1){
                    ZONA = 1;
                  }

                  if(ANGULO <= 180 && ANGULO >= 91){
                    ZONA = 2;
                  }

                  if(ANGULO <= 270 && ANGULO >= 181){
                    ZONA = 3;
                  }

                  if(ANGULO <= 360 && ANGULO >= 271){
                    ZONA = 4;
                  }
                  if(ANGULO <= 450 && ANGULO >=361){
                    ZONA = 5;
                  }

                  if(ANGULO <= 540 && ANGULO >= 451){
                    ZONA = 6;
                  }

                  if(ANGULO <= 630 && ANGULO >= 541){
                    ZONA = 7;
                  }

                  if(ANGULO <= 720 && ANGULO >= 631){
                    ZONA = 8;
                    VUELTA = 2;
                  }
                  if(ANGULO <= 810 && ANGULO >= 721){
                    ZONA = 9;
                  }

                  if(ANGULO <= 900 && ANGULO >= 811){
                    ZONA = 10;
                  }

                  if(ANGULO <= 990 && ANGULO >= 901){
                    ZONA = 11;
                  }

                  if(ANGULO <= 1080 && ANGULO >= 991){
                    ZONA = 12;
                    VUELTA = 3;
                  }
                  if(ANGULO <= 1170 && ANGULO >= 1081){
                    ZONA = 13;
                    VUELTA = 4;
                  }
                }
              
              ET_2 = SD_2 - SM_2; //ERROR PROPORCIONAL 
              dE_2 = (ET_2 - Ep_2)/t_2; //ERROR DIFERENCIAL
              Ep_2 = ET_2;
              iE_2 = Eip_2 + (t_2*ET_2); //ERROR INTEGRAL
              Eip_2 = Ei_2;
              t_2 = t_2 + (0.053 + (PID_2/100)); //TIEMPO 
              PID_2 = (KP*ET_2)+(KI*iE_2)+(KD*dE_2); //PID
              if(PID > 0){
                GIRO.write(L_DERECHA);
                delay(PID*10);
              }
              if(PID < 0){
                GIRO.write(L_IZQUIERDA);
                delay(PID*5);
              }
                //ULTRASONICO
                  digitalWrite(TRIG_1, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_1, LOW);
                  DURACION_1 = pulseIn(ECO_1, HIGH);
                  DIST_IZQUIERDA = DURACION_1 / time; 
                  digitalWrite(TRIG_2, HIGH); 
                  delay(1);
                  digitalWrite(TRIG_2, LOW); 
                  DURACION_2 = pulseIn(ECO_2, HIGH);
                  DIST_FRENTE = DURACION_2 / time;
                  digitalWrite(TRIG_3, HIGH);     
                  delay(1);        
                  digitalWrite(TRIG_3, LOW);
                  DURACION_3 = pulseIn(ECO_3, HIGH);     
                  DIST_DERECHA = DURACION_3 / time;
              if(DIST_FRENTE < 80 && DIST_FRENTE > 0){ //GIRO
                VELOCIDAD = 50; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
                GIRO.write(L_DERECHA); delay(500);
                VELOCIDAD = 50; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
                delay(1300);
                GIRO.write(L_IZQUIERDA); delay(100); GIRO.write(L_CENTRO_D); delay(100); ACTIVO = 2;
                VELOCIDAD = 80; PWM = map(VELOCIDAD, 0, 100, 0, 250); analogWrite(ENB, PWM);
              }
          }
      }
    }
  }
 
void printResult(HUSKYLENSResult result){
  if (result.command == COMMAND_RETURN_BLOCK){
    switch(result.ID){
      case 1:
        if(result.ID == 1){
          //Serial.println(String()+F("ROJO  X= ")+result.xCenter+F(", Y= ")+result.yCenter+F(", ANCHO= ")+result.width+F(", LARGO= ")+result.height+F(", ID= ")+result.ID);
          VERDE = 0;
          ROJO = 1;
          VELOCIDAD = 100;
          X = result.xCenter;
          Y = result.yCenter;
        }
      case 2:
        if(result.ID == 2){
          //Serial.println(String()+F("VERDE X= ")+result.xCenter+F(", Y= ")+result.yCenter+F(", ANCHO= ")+result.width+F(", LARGO= ")+result.height+F(", ID= ")+result.ID);
          VERDE = 1;
          ROJO = 0;
          VELOCIDAD = 100;
          X = result.xCenter;
          Y = result.yCenter;
        }
    }
  }

  else{
    Serial.println("¡NINGUN OBJETO!");
  }
 }

//MATE BY LAB ROBOT