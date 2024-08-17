#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
#include <Servo.h>
#include "HUSKYLENS.h" 

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

HUSKYLENS huskylens;
MPU6050 mpu(Wire);
Servo GIRO;

//PUENTE H
int PWM;
int VELOCIDAD = 50;

//GIROSCOPIO
unsigned long timer = 0;
int time = 58.2;
int ANGULO;
int DIRECCION;
int VUELTA = 1;

//SERVO
int E_GIRO = 1;
int L_DERECHA = 145;
int L_IZQUIERDA = 70;
int L_CENTRO_I = 120;
int L_CENTRO_D = 95;
int L_CENTRO = 110;

//ULTRASONICOS
int DURACION_1;
int DURACION_2;
int DURACION_3;
int DIST_FRENTE;
int DIST_IZQUIERDA;
int DIST_DERECHA;

//TACTIL
int TACTIL;
int ESTADO_BOTON = 0;

//OBJETO
int X_ROJO;
int X_VERDE;
int Y_ROJO;
int Y_VERDE;

//DIRECCIÓN
int X_CENTRO = 160;
int Y_CENTRO = 120;
int IZQUIERDA = 0;
int DERECHA = 0;
int CENTRO = 110;
int RECTO = 1;

//ESTABILIZACIÓN
int DERECHO = 0;
int SEMAFORO = 0;

//######################################################### CONFIGURACION SETUP
void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();        
  mpu.calcGyroOffsets();

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

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, 0);
  GIRO.attach(13);
  delay(2000);
  GIRO.write(L_CENTRO);
  delay(2000);
  Serial.println("LISTO");
}

//######################################################### LOOP
void loop() {
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

  if(ESTADO_BOTON == 1){

    //**************************************SENSORES**************************************
    //VERIFICACIÓN
      Serial.print("DISTANCIA IZQUIERDA: "); Serial.print(DIST_IZQUIERDA); Serial.println("cm");
      Serial.print("DISTANCIA DERECHA: "); Serial.print(DIST_DERECHA); Serial.println("cm");
      Serial.print("DISTANCIA FRENTE: "); Serial.print(DIST_FRENTE); Serial.println("cm");
      Serial.print("ANGULO: "); Serial.print(ANGULO); Serial.println("°");
      Serial.print("DIRECCION: "); Serial.print(DIRECCION); Serial.println(" de 4");
      Serial.print("VUELTA: "); Serial.print(VUELTA); Serial.println(" de 3");
      Serial.print("VELOCIDAD: "); Serial.print(VELOCIDAD); Serial.println("%");

    //GIROSCOPIO
      mpu.update();  
      if((millis()-timer)>100){                                          
        ANGULO = 1 + (mpu.getAngleZ());
        timer = millis(); 
        delay(50);
        if(ANGULO == 0){
          ANGULO = 360;
        }

        if(ANGULO <= 90 && ANGULO >=1){
          DIRECCION = 1;
        }

        if(ANGULO <= 180 && ANGULO >= 91){
          DIRECCION = 2;
        }

        if(ANGULO <= 270 && ANGULO >= 181){
          DIRECCION = 3;
            if(VUELTA == 2){
              VUELTA = 3;
            }
          }

        if(ANGULO <= 330 && ANGULO >= 271){
          DIRECCION = 4;
          if(VUELTA == 1){
            VUELTA = 2;
          }
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
      if (!huskylens.request()) Serial.println(F("¡Comunicación fallida , revise conexiones fisicas!"));
      else if(!huskylens.isLearned()) Serial.println(F("¡No tiene ningun objeto para elegir, elija uno!"));
      else if(!huskylens.available()) Serial.println(F("¡No aparecen bloques en la pantalla!")); 
      else{
        Serial.println(F("###########")); 
        if (huskylens.available()){
          HUSKYLENSResult result = huskylens.read(); printResult(result);
        }
      }

    //**************************************CONTROL**************************************
    if(ANGULO < DERECHO){
      GIRO.write(DERECHA);
      delay(200);
      GIRO.write(L_DERECHA);
    }
    //SEGUIMIENTO
    /*if(X_VERDE >= X_CENTRO){
      GIRO.write(L_IZQUIERDA);
      IZQUIERDA = 0;
      DERECHA = 1;
    }
    if(X_VERDE <= X_CENTRO){
      GIRO.write(L_DERECHA);
      IZQUIERDA = 1;
      DERECHA = 0;
    }
    if(IZQUIERDA == 1){
      CENTRO = L_CENTRO_D;
    }
    if(IZQUIERDA == 1){
      CENTRO = L_CENTRO_I;
    }
    if(RECTO == 1){
      GIRO.write(CENTRO);
    }
    PWM = map(VELOCIDAD, 0, 100, 0, 250);
    analogWrite(ENB, PWM);
    RECTO = 1;*/

    //RADAR
    if(DIST_FRENTE >= 10){
      if(DERECHA == 1){
        GIRO.write(L_CENTRO_D);
        delay(100);
      }
      if(IZQUIERDA == 1){
        GIRO.write(L_CENTRO_I);
        delay(100);
      }
      GIRO.write(L_CENTRO);
      delay(100);
    }
    if(DIST_IZQUIERDA <= 10 && DIST_IZQUIERDA >= 1){
      GIRO.write(L_IZQUIERDA);
      DERECHA = 0;
      IZQUIERDA = 1;
      delay(100);
    }
    if(DIST_DERECHA <= 10 && DIST_DERECHA >= 1){
      GIRO.write(L_DERECHA);
      DERECHA = 1;
      IZQUIERDA = 0;
      delay(100);
    }
  }    
}
 
void printResult(HUSKYLENSResult result){
  if (result.command == COMMAND_RETURN_BLOCK){
    if(result.ID == 1){
    Serial.println(String()+F("VERDE X= ")+result.xCenter+F(", Y= ")+result.yCenter+F(", ANCHO= ")+result.width+F(", LARGO= ")+result.height+F(", ID= ")+result.ID);
    VELOCIDAD = 0;
    X_ROJO = result.xCenter;
    Y_ROJO = result.yCenter;
    RECTO = 0;
    }
    if(result.ID == 2){
    Serial.println(String()+F("ROJO X= ")+result.xCenter+F(", Y= ")+result.yCenter+F(", ANCHO= ")+result.width+F(", LARGO= ")+result.height+F(", ID= ")+result.ID);
    VELOCIDAD = 100;
    X_VERDE = result.xCenter;
    Y_VERDE = result.yCenter;
    RECTO = 0;
    }
  }

  else{
    Serial.println("¡NINGUN OBJETO!");
  }
}