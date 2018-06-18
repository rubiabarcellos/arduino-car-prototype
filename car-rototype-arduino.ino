/* 
   Arduino MEGA
   Data final: 10/06/2016
   Função: Carro robô
*/

#include "Ultrasonic.h"
 
Ultrasonic ultrasonic(6,7); //Trigger - 6 - e Echo - 7

/* Variáveis para o sensor ultrasonico */
long microsec = 0;
float distanciaCM = 0;

/*Variável para o buzzer*/
int pinBuzzer = 8;

/*Variável para o acelerometro*/
int accelero;

/*Variável para ler o valor enviado do celular por Bluetooth*/
char readBluetooth;

/*Variáveis para o módulo de motor ponte H*/
int EnA = 2; 
int In1 = 40; 
int In2 = 38;

int EnB = 3; 
int In3 = 36;
int In4 = 34;

int velocidade = 150;

/* Variáveis para o sensor de luminosidade */
int sensorLDR = 0; // pino Analógico A0
int valorLDR = 0; //lê o valor em tempo real

/* Variáveis para os LEDs */
int ledBranco1 = 32;
int ledBranco2 = 42;
int ledVermelho1 = 22;
int ledVermelho2 = 52;


void setup(){
  Serial.begin(9600);//Inicia Comunicação Serial
  motor.attach(13);
  
  //SETA os In como saída 
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  pinMode(In3,OUTPUT);
  pinMode(In4,OUTPUT); 
  pinMode(EnA,OUTPUT);
  pinMode(EnB,OUTPUT); 

  pinMode(pinBuzzer,OUTPUT);

  pinMode(ledBranco1, OUTPUT);
  pinMode(ledBranco2, OUTPUT);
  pinMode(ledVermelho1, OUTPUT);
  pinMode(ledVermelho2, OUTPUT);  
}

void loop(){
  valorLDR = analogRead(sensorLDR); //Leitura do valor do LDR
  acendeFarolLDR();
 
  microsec = ultrasonic.timing(); //Lendo o sensor
  distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM); //Convertendo a distância em CM
  distanciaObstaculo(); 
  
  if(Serial.available()){
    readBluetooth = Serial.read();//Valor lido na Serial será guardado na variável readBluetooth
    
    analogWrite(EnA, velocidade); //Velocidade do motor A
    analogWrite(EnB, velocidade); //Velocidade do motor B 
         
    /*Chamada dos métodos baseados no valor recebido*/
    switch (readBluetooth){
      case 'a':
        goFoward();
      break;
  
      case 'b':
        goBack();
      break;
  
      case 'c':
        goRight(); 
        delay(400);
        stopCar();   
      break;
  
      case 'd':
        goLeft(); 
        delay(400);
        stopCar();
        break;
  
      case 'e':
        stopCar();
      break;
  
      case 'f':
        buzina();
      break;    
  
      case 'g':
       acendeFarol();
      break;
  
      case 'h': //Velocidade baixa
        velocidade = 80; 
      break;
  
      case 'i': //Velocidade média
        velocidade = 150;
      break;
        
      case 'j': //Velocidade alta
        velocidade = 230;
      break;
        
      default:
        stopCar();
    } 
  }  
}
/*--------- MÉTODOS ---------*/
/* Método: andar para frente */
void goFoward(){ 
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);  
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  apagaRe(); 
}
/* fim - Método: andar para frente */
//////////////////////////////////////////////////////
/* Método: andar para trás */
void goBack(){
  digitalWrite(In1, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In2, LOW);  
  digitalWrite(In4, LOW); 
  acendeRe(); 
}
/* fim - Método: andar para trás */
//////////////////////////////////////////////////////
/* Método: andar para a esquerda */
void goLeft(){
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);  
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW); 
  apagaRe(); 
}
/* fim - Método: andar para a esquerda */
//////////////////////////////////////////////////////
/*Método: andar para a direita*/
void goRight(){
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);  
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  apagaRe();
}
/*fim - Método: andar para a direita*/
//////////////////////////////////////////////////////
/*Método: parar o carro*/
void stopCar(){
  digitalWrite(In1, LOW); 
  digitalWrite(In2, LOW); 
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
  apagaRe();
}
/*fim - Método: parar o carro*/
//////////////////////////////////////////////////////
/*Medição da distância com o Sensor Untrassonico*/
void distanciaObstaculo(){ 
  if(distanciaCM <= 10 && (readBluetooth == 'a' || readBluetooth == 'b' 
                      || readBluetooth == 'c' || readBluetooth == 'd')){   
      stopCar(); 
      delay(500);  
      goBack();
      buzzer();
      delay(500);
      stopCar();
    }
}
/* fim - Medição da distância com o Sensor Untrassonico*/
//////////////////////////////////////////////////////
/*Auto-falante de frequência*/
void buzzer(){
  tone(pinBuzzer,2000,50); // porta, frequencia, duração
  delay(250);
  tone(pinBuzzer,2000,50);
}

void buzina(){
  tone(pinBuzzer,1000,120);
}
/*fim - Auto-falante de frequência*/
//////////////////////////////////////////////////////
boolean farolAceso = false;
/* Acender e apagar LED do farol*/ 
void acendeFarolLDR(){
  if(valorLDR <= 40){
     digitalWrite(ledBranco1, HIGH);
     digitalWrite(ledBranco2, HIGH);
  }

  else{
    if(farolAceso != true){
      digitalWrite(ledBranco1, LOW);
      digitalWrite(ledBranco2, LOW);
    }  
  }
} 
/* fim - Acender e apagar LED do farol */
//////////////////////////////////////////////////////
/* Acender e apagar LED do farol com botão*/
int valorLED = digitalRead(ledBranco1);

void acendeFarol(){
  if(farolAceso == false){
     digitalWrite(ledBranco1, HIGH);
     digitalWrite(ledBranco2, HIGH);
     farolAceso = true;
      Serial.println(valorLED); 
  }

  else{
    if(farolAceso == true){
      digitalWrite(ledBranco1, LOW);
      digitalWrite(ledBranco2, LOW);
      farolAceso = false;
    }
  }  
} 
/* fim - Acender e apagar LED do farol com botão*/
//////////////////////////////////////////////////////
/* Acender e apagar LED traseiro */
void acendeRe(){
  digitalWrite(ledVermelho1, HIGH);
  digitalWrite(ledVermelho2, HIGH);
}
 
void apagaRe(){ 
  digitalWrite(ledVermelho1, LOW);
  digitalWrite(ledVermelho2, LOW);
}
/* fim - Acender e apagar LED traseiro */
//////////////////////////////////////////////////////
/*--------- FIM MÉTODOS ---------*/
