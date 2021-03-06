
 /* combate.ino
 * Código para controle de robô beetleweight
 * Autores: Isabella Galvão e Marcus Vinícius
 * 05/09/2016
 */

//Inclui biblioteca para acesso a funções matemáticas avançadas
#include <math.h>
//#include <Arduino.h>

//Define os pinos que os motores estarão conectados
#define MOTOR_E1 11
#define MOTOR_E2 10
#define MOTOR_D1 5
#define MOTOR_D2 6


//Define as entradas de sinal do controle
#define AILE A0
#define ELE A1

//Constantes para leitura do controle (Devem ser calibradas)
//Intensidade máxima do sinal permitida como velocidade máxima
#define MAX 1900
#define MIN 1200
//#define TOLERANCIA 30
#define VELOCIDADE_MAXIMA 250
#define VELOCIDADE_MINIMA 10    //Mudar de acordo com o robô
#define DIV  2                  //divisor de potência para diagonal,valores de teste , amanhã mudamos

//Vão guardar as leituras vindas do controle
int aile = 0;
int ele = 0;
int ele_potencia = 0;
int aile_potencia = 0;
int potencia = 0;
//limiar aile ele
#define LIMIAR_MAX_AILE 100
#define LIMIAR_MIN_AILE -100
#define LIMIAR_MAX_ELE  100 
#define LIMIAR_MIN_ELE  -100 

void setup() {
  // Define entradas e saídas
  pinMode(AILE, INPUT);
  pinMode(ELE, INPUT);

  pinMode(MOTOR_E1, OUTPUT);
  pinMode(MOTOR_E2, OUTPUT);
  pinMode(MOTOR_D1, OUTPUT);
  pinMode(MOTOR_D2, OUTPUT);
  Serial.begin(9600);
  Serial.flush();               //limpa o lixo eletrônico
}

void loop() {
  
  //leitura dos canais do controle
  aile = pulseIn(AILE, HIGH);  
  ele = pulseIn(ELE,HIGH);
  
  //debug para leitura do controle   
  Serial.print("AILE : ");
  Serial.println(aile);
  Serial.print("ELE : ");
  Serial.println(ele);
  delay(500);

 //condição que impede que o robô se movimente quando o CONTROLE estiver DESLIGADO
   if(aile != 0 && ele !=0)             
{ 
  //debug para sinais mapeados
   ele_potencia = potenciaPwmEle(ele);
   aile_potencia = potenciaPwmAile(aile);
   Serial.print(" Ele mapiado = ");
   Serial.println(ele_potencia);
   Serial.print(" Aile mapiado = ");
   Serial.println(aile_potencia);
 /* Serial.print("AILE : ");
  Serial.println(aile_potencia);
  Serial.print("ELE : ");
  Serial.println(ele_potencia);
  delay(500); */
     

//condição parado
    if(((aile_potencia >= LIMIAR_MIN_AILE)  && (aile_potencia <= LIMIAR_MAX_AILE))  && ((ele_potencia >= LIMIAR_MIN_ELE) && (ele_potencia <= LIMIAR_MAX_ELE))){
   // potencia = map(ele,MIN,MAX,-255,255);
   // if(potencia>245) {potencia=255;}
      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,LOW);
      //DEBUG
      Serial.println("parado");
      delay(500);
  }
  //condição para frente
    else if(((aile_potencia >= LIMIAR_MIN_AILE-60)  && (aile_potencia <= LIMIAR_MAX_AILE-60)) && (ele_potencia > LIMIAR_MAX_ELE-30)){
   // potencia = ele_potencia;//map(ele,MIN,MAX,-255,255);  
   // if(potencia>245) {potencia=255;} 
      analogWrite(MOTOR_E1,ele_potencia);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,ele_potencia);
      digitalWrite(MOTOR_D2,LOW);
      //DEBUG
      Serial.print("potencia: "); 
      Serial.println(ele_potencia);
      Serial.println("FRENTE");
      delay(500);
    }
     //condição para trás
  else if(((aile_potencia >= LIMIAR_MIN_AILE+40)  && (aile_potencia <= LIMIAR_MAX_AILE-60)) && (ele_potencia < LIMIAR_MIN_ELE)){
  //potencia = map(ele,MIN,MAX,-255,255);
  //if(potencia>245) {potencia=255;}   
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,ele_potencia);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,ele_potencia); 
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("TRÁS");
      delay(500);
    } 
    //condição para direita
  else if(((ele_potencia >= LIMIAR_MIN_ELE)  && (ele_potencia <= LIMIAR_MAX_ELE)) && (aile_potencia > LIMIAR_MAX_AILE)){
   //potencia = map(aile,MIN,MAX,-255,255); 
  // if(potencia>245) {potencia=255;}  
      analogWrite(MOTOR_E1,aile_potencia);
      digitalWrite(MOTOR_E2,LOW);
      digitalWrite(MOTOR_D1,LOW);
      digitalWrite(MOTOR_D2,LOW);
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(aile_potencia);
      Serial.println("DIREITA");
      delay(500); 
    }
   //condição para esquerda
  else if(((ele_potencia >= LIMIAR_MIN_ELE)  && (ele_potencia <= LIMIAR_MAX_ELE)) && (aile_potencia < LIMIAR_MIN_AILE)){
   //potencia = map(aile,MIN,MAX,-255,255);   
   // if(potencia>245) {potencia=255;}
      digitalWrite(MOTOR_E1,LOW);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,aile_potencia);
      digitalWrite(MOTOR_D2,LOW);
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(aile_potencia);
      Serial.println("ESQUERDA");
      delay(500); 
    }
    //diagonal frente direita
    else if((ele_potencia > LIMIAR_MAX_ELE) && (aile_potencia > LIMIAR_MAX_AILE)){
    //potencia = map(ele,MIN,MAX,-255,255);  
   // if(potencia>245) {potencia=255;} 
      analogWrite(MOTOR_E1,ele_potencia);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,ele_potencia/DIV);
      digitalWrite(MOTOR_D2,LOW); 
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL FRENTE DIREITA");
      delay(500);
    }
   //diagonal frente esquerda
    else if((ele_potencia > LIMIAR_MAX_ELE) && (aile_potencia < LIMIAR_MIN_AILE)){
   //potencia = map(ele,MIN,MAX,-255,255);
   //if(potencia>245) {potencia=255;}   
      analogWrite(MOTOR_E1,ele_potencia/DIV);
      digitalWrite(MOTOR_E2,LOW);
      analogWrite(MOTOR_D1,ele_potencia);
      digitalWrite(MOTOR_D2,LOW);
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL FRENTE ESQUERDA");
      delay(500); 
    }
   //diagonal tras direita
    else if((ele_potencia < LIMIAR_MIN_ELE) && (aile_potencia > LIMIAR_MAX_AILE)){
    //potencia = map(ele,MIN,MAX,-255,255);  
    //if(potencia>245) {potencia=255;} 
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,ele_potencia);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,ele_potencia/DIV);
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL TRAS DIREITA");
      delay(500); 
    }
   //diagonal tras esquerda
    else if((ele_potencia < LIMIAR_MIN_ELE) && (aile_potencia < LIMIAR_MIN_AILE)){
    //potencia = map(ele,MIN,MAX,-255,255);
    //if(potencia>245) {potencia=255;}   
      digitalWrite(MOTOR_E1,LOW);
      analogWrite(MOTOR_E2,ele_potencia/DIV);
      digitalWrite(MOTOR_D1,LOW);
      analogWrite(MOTOR_D2,ele_potencia);
      //DEBUG
      Serial.print("potencia: ");
      Serial.println(ele_potencia);
      Serial.println("DIAGONAL TRAS ESQUERDA");
      delay(500); 
    }
}
}
/** 
  //funções que mapeiam - somente - os valores de aile e ele 
int mapPwmAile(int value) {
   return map(value, MIN, MAX, -255, 255);
}
int mapPwmEle(int value) {
   return map(value, MIN, MAX, -255, 255);
}
**/

//Função que mapeia o sinal aile do controle e limita a potência MÁXIMA e MÍNÍMA
int potenciaPwmAile(int sinal) {
    int potencia = map(sinal, MIN, MAX, -255, 255);               //mapeando aile
    if(abs(potencia) > VELOCIDADE_MAXIMA) {                       //limitando velocidade máxima
      return (potencia/abs(potencia)) * VELOCIDADE_MAXIMA;
    }
      else if(abs(potencia) < VELOCIDADE_MINIMA){                 //limitando velocidade mínima (para não forçar o motor)
        return (potencia/abs(potencia)) * VELOCIDADE_MINIMA;  
      }
        else{
          return potencia;
        }
  }
//Função que mapeia os sinais de ele do controle e limita a potência MÁXIMA E MÍNIMA
int potenciaPwmEle(int sinal) {
    int potencia = map(sinal, MIN, MAX, -255, 255);             //mapeando a ele
    if(abs(potencia) > VELOCIDADE_MAXIMA) {                     //limitando a velocidade máxima 
     return (potencia/abs(potencia)) * VELOCIDADE_MAXIMA;
    }
      else if(abs(potencia) < VELOCIDADE_MINIMA){               //limitando velocidade mínima (para não forçar o motor)
        return (potencia/abs(potencia)) * VELOCIDADE_MINIMA;    
      }
      else {
        return potencia;
    }
  }
