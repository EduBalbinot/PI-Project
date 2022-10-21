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


//Configuracoes Medicoes&Pwm
const float FREQUENCIA_PWM=0.1;
const long int CLOCK_ARDUINO=16000000;
const long int MAXPULSOS_TIMER_PSEGUNDO=CLOCK_ARDUINO/(256*2);
const long int PULSOS_TIMER0=(1/FREQUENCIA_PWM)*MAXPULSOS_TIMER_PSEGUNDO;
const long int MINPULSOS_VSOLENOIDE=MAXPULSOS_TIMER_PSEGUNDO*0.008;
long int cont_timer1=0;
long int PULSOS_CONTROLE;
float NOVO_DUTY_CYCLE=0.0;


//Variaveis Globais
bool LER_MEDICOES=true;
bool rodando=false;
bool estado_compressor=false;
bool primeiro_armazenamento=true;
float ultima_temp1=0, ultima_temp2=0, ultima_pressao=0, DutyCycle=0, PRESSAO_MIN=1000, PRESSAO_MAX=2000;
unsigned long int ID_DADO=0; //contador banco de banco_dados


//Sensores Temperatura
#define SENSORES_TEMP 6
OneWire oneWire(SENSORES_TEMP);
DallasTemperature sensors(&oneWire);


//Sensor Pressao
HX711 sensor_pressao;


//Funcoes
float aquisicao_temperatura(int sensor);
float aquisicao_pressao(void);
void aciona_compressor(void);
void desliga_compressor(void);
void aciona_Vsolenoide(void);
void desliga_Vsolenoide(void);
void controle_solenoide(float sinal_controle);
double controladorPI(float referencia, float saida);

//interrupt PWM
ISR(TIMER1_COMPA_vect){
  cont_timer1++;

  if(cont_timer1<PULSOS_CONTROLE)
    aciona_Vsolenoide();
  else{
    desliga_Vsolenoide();
    if(LER_MEDICOES && rodando){
      LER_MEDICOES=false;
      efetua_medicoes();
      armazena_dados();
    }
  }

  if(cont_timer1>=PULSOS_TIMER0){
    cont_timer1=0;
    LER_MEDICOES=true;
    if(!estado_compressor)
      controle_solenoide(NOVO_DUTY_CYCLE); //adicionar o novo DutyCycle
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

   //Inicia Serial
   Serial.begin(115200);

   //INICIALIZA SENSORES TEMPERATURA
   sensors.begin();

   //INICIALIZA SENSOR PRESSAO
   sensor_pressao.begin(A1, A2);
   sensor_pressao.set_scale(2280.f);
   sensor_pressao.tare();

   // sensor_pressao.set_offset(1351599.00);

   //Configuracoes Ensaio
   aciona_compressor();
   controle_solenoide(0.0);
   _delay_ms(1000); //delay para estabilizar o sistema

   rodando=true;
   while (rodando){ //loop principal
    if(ultima_pressao>=PRESSAO_MAX)
      estado_compressor=false;
    if(ultima_pressao<=PRESSAO_MIN)
      estado_compressor=true;


     _delay_ms(100);
   }

   desliga_compressor();
   controle_solenoide(0.0);
}

void aciona_Vsolenoide(){
  PORTB|=(0x02);
}

void desliga_Vsolenoide(){
  PORTB&=~(0x02);
}

float aquisicao_temperatura(int sensor){
  float temperatura;
  sensors.requestTemperatures();
  temperatura=sensors.getTempCByIndex(sensor);
  return(temperatura);
}

float aquisicao_pressao(void){
  float valor=sensor_pressao.get_units();
  sensor_pressao.power_down();
  _delay_ms(20);
  sensor_pressao.power_up();
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
  cli();

  //Ajuste ciclo de trabalho
  if(sinal_controle>=0.8){
    PULSOS_CONTROLE=PULSOS_TIMER0*0.8;
    DutyCycle=0.8;
  } else if(sinal_controle<=0){
    PULSOS_CONTROLE=0;
    DutyCycle=0;
  } else{
    PULSOS_CONTROLE=PULSOS_TIMER0*sinal_controle;
    DutyCycle=sinal_controle;
  }


  sei();
}

void efetua_medicoes(){
  // ultima_temp1=aquisicao_temperatura(0);
  // ultima_temp2=aquisicao_temperatura(1);
  ultima_pressao=aquisicao_pressao();
}

void armazena_dados(){
  
}
