/*
* main.cpp
*
* Created: 09/05/2022
* Author : Dudu
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>
#include <SPI.h>
#include <SD.h>

//Configuracoes Medicoes
const float FREQUENCIA_PWM=0.1;
const long int CLOCK_ARDUINO=16000000;
const long int MAXPULSOS_TIMER_PSEGUNDO=CLOCK_ARDUINO/(256*2);
const long int PULSOS_TIMER0=(1/FREQUENCIA_PWM)*MAXPULSOS_TIMER_PSEGUNDO;
const long int MINPULSOS_VSOLENOIDE=MAXPULSOS_TIMER_PSEGUNDO*0.008;

long int cont_timer0=0;
long int PULSOS_CONTROLE;

//Configuracoes PWM
float DUTY_CYCLE=0;

//Sensores Temperatura
#define SENSORES_TEMP 6
OneWire oneWire(SENSORES_TEMP);
DallasTemperature sensors(&oneWire);

//Sensor Pressao
HX711 scale;

//Funcoes
float aquisicao_temperatura(int sensor);
float aquisicao_pressao(void);
void aciona_compressor(void);
void desliga_compressor(void);
void aciona_Vsolenoide(void);
void desliga_Vsolenoide(void);
void controle_solenoide(float sinal_controle);

//Variaveis Globais
bool LER_MEDICOES=true;
bool rodando=false;
bool estado_compressor=false;
bool primeiro_armazenamento=true;
float ultima_temp1=NULL, ultima_temp2=NULL, ultima_pressao=NULL;
unsigned long int ID_DADO=0; //contador banco de banco_dados

//interrupt PWM
ISR(TIMER1_COMPA_vect){
  cont_timer0++;

  if(cont_timer0<PULSOS_CONTROLE)
    PORTB|=(0x02);
  else{
    PORTB&=~(0x02);
    if(LER_MEDICOES && rodando){
      LER_MEDICOES=false;
      efetua_medicoes();
      armazena_dados();
    }
  }

  if(cont_timer0>=PULSOS_TIMER0){
    cont_timer0=0;
    LER_MEDICOES=true;
  }
}

int main(void){
   //PORTAS
   DDRB|=(0x03); //define PB0(Compressor) e PB1(Valvula PWM) como saidas
   PORTB|=(0x01); //PULLUP PB0(compressor começa desligado)

   //INTERRUPT MEDICOES
   TCCR1A=0;
   TCCR1B=0;
   TCCR1B|=(0x0C); //prescaler CLK/256
   TIMSK1=(0x02); //define a interrupcao OCIE1A
   OCR1A=1; //calculos no inicio nao mudar!!!
   //INICIALIZA SENSORES TEMPERATURA
   sensors.begin();

   //INICIALIZA SENSOR PRESSAO
   scale.begin(A1, A2);
   scale.set_scale(2280.f);
   scale.tare();

   //variaveis
   Serial.begin(115200);

   //Configuracoes Ensaio
   aciona_compressor();
   DUTY_CYCLE=0.5;
   controle_solenoide(DUTY_CYCLE);
   _delay_ms(500); //delay para estabilizar o sistema

   Serial.println("Começo do loop");
   rodando=true;
   while (rodando){ //loop principal


     _delay_ms(20);
   }

   desliga_compressor();
   controle_solenoide(0.0);
}

float aquisicao_temperatura(int sensor){
  float temperatura;
  sensors.requestTemperatures();
  temperatura=sensors.getTempCByIndex(sensor);
  return(temperatura);
}

float aquisicao_pressao(void){
  float valor=scale.get_units();
  scale.power_down();
  _delay_ms(20);
  scale.power_up();
  return(valor);
}

void aciona_compressor(void){
 PORTB&=~(0x01);
 estado_compressor=true;
}

void desliga_compressor(void){
  PORTB|=(0x01);
  estado_compressor=false;
}

void controle_solenoide(float sinal_controle){
  PULSOS_CONTROLE=PULSOS_TIMER0*sinal_controle;
}

void efetua_medicoes(){
  // ultima_temp1=aquisicao_temperatura(0);
  // ultima_temp2=aquisicao_temperatura(1);
  ultima_pressao=aquisicao_pressao();
}

void armazena_dados(){
  if(primeiro_armazenamento){
    Serial.println("i) Temp1: ## | Temp2: ## | Pressao: ## | DutyCycle: ## | Estado Compressor: ##");
    primeiro_armazenamento=false;
  }

  Serial.print(ID_DADO);
  Serial.print(" ");
  Serial.print(ultima_temp1);
  Serial.print(" ");
  // Serial.print(ultima_temp2);
  // Serial.print(" ");
  Serial.print(ultima_pressao);
  Serial.print(" ");
  Serial.println(estado_compressor);
  ID_DADO++;

}
