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


//Configuracoes Medicoes&Pwm
const float FREQUENCIA_PWM=0.1;
const long int CLOCK_ARDUINO=16000000;
const long int MAXPULSOS_TIMER_PSEGUNDO=CLOCK_ARDUINO/(256*2);
const long int PULSOS_TIMER0=(1/FREQUENCIA_PWM)*MAXPULSOS_TIMER_PSEGUNDO;
const long int MINPULSOS_VSOLENOIDE=MAXPULSOS_TIMER_PSEGUNDO*0.008;
long int cont_timer1=0;
long int PULSOS_CONTROLE;


/*ControladorPID*/
//parametros controlador
float Kp;
float Kd;
float Ti;
float Ta;
//variaveis controlador
float Ek[3]={0, 0, 0}; //erro
float Uk[2]={0, 0}; //acao de controle
float Yk[2]={0, 0}; //Saida
float Uk_max=1; //linearizar acao de controle 0.00-1.00


//Variaveis Globais
bool LER_MEDICOES=true;
bool rodando=false;
bool estado_compressor=false;
bool primeiro_armazenamento=true;
float ultima_temp1=NULL, ultima_temp2=NULL, ultima_pressao=NULL, DutyCycle=0;
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


//interrupt PWM
ISR(TIMER1_COMPA_vect){
  cont_timer1++;

  if(cont_timer1<PULSOS_CONTROLE)
    PORTB|=(0x02);
  else{
    PORTB&=~(0x02);
    if(LER_MEDICOES && rodando){
      LER_MEDICOES=false;
      efetua_medicoes();
      armazena_dados();
      //controle_solenoide(controladorPID( , ultima_pressao)); //referencia, saida
    }
  }

  if(cont_timer1>=PULSOS_TIMER0){
    cont_timer1=0;
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

   //Inicia Serial
   Serial.begin(115200);

   //INICIALIZA SENSORES TEMPERATURA
   sensors.begin();

   //INICIALIZA SENSOR PRESSAO
   sensor_pressao.begin(A1, A2);
   sensor_pressao.set_scale(2280.f);
   sensor_pressao.tare();
   Serial.println("Coloque Sensor!");
   _delay_ms(2000);

   //Configuracoes Ensaio
   aciona_compressor();
   controle_solenoide(0); //estado inicial PWM
   _delay_ms(500); //delay para estabilizar o sistema


   Serial.println("Começo do loop");
   rodando=true;
   while (rodando){ //loop principal
    if(ultima_pressao>=2500)
      controle_solenoide(0.2); //estado inicial PWM

     _delay_ms(100);
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
  DutyCycle=sinal_controle;
  //Ajuste ciclo de trabalho
  if(sinal_controle>=1)
    PULSOS_CONTROLE=PULSOS_TIMER0;
  else if(sinal_controle<=0)
    PULSOS_CONTROLE=0;
  else
    PULSOS_CONTROLE=PULSOS_TIMER0*sinal_controle;

  // //verifica caso valor de acionamento minimo/max atingido
  // if(PULSOS_CONTROLE<=MINPULSOS_VSOLENOIDE)
  //   PULSOS_CONTROLE=0;
  // if(PULSOS_CONTROLE>=(PULSOS_TIMER0-MINPULSOS_VSOLENOIDE))
  //   PULSOS_CONTROLE=PULSOS_TIMER0;
  sei();
}

float controladorPID(float referencia, float saida){
  float a, b ,c, d;

  Yk[0]=saida;
  Ek[0]=saida-referencia;

  a=Uk[1];
  b=Ek[0]*(Kp+(Ta/(2*Ti))+(Kd/Ta));
  c=Ek[1]*(Ta/(2*Ti)-Kp-((2*Kd)/Ta));
  d=Ek[2]*(Kd/Ta);

  Uk[0]=a+b+c+d;

  //Atualiza Erros
  Ek[2]=Ek[1];
  Ek[1]=Ek[0];

  //Atualiza acoes de controle
  Uk[1]=Uk[0];

  //Atualiza saida
  Yk[1]=Yk[0];

  return (Uk[0]/Uk_max);
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
  Serial.print(ultima_temp2);
  Serial.print(" ");
  Serial.print(ultima_pressao);
  Serial.print(" ");
  Serial.print(DutyCycle);
  Serial.print(" ");
  Serial.println(estado_compressor);
  ID_DADO++;
}
