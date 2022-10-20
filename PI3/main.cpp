/*
* main.cpp
*
* Created: 09/05/2022
* Author : Dudu
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>

//Pinos
#define PIN_VALVULA_SOLENOIDE 12
#define PIN_A_SENSOR_PRESSAO 9
#define PIN_B_SENSOR_PRESSAO 8
#define PIN_SENSOR_TEMPERATURA 7
#define PIN_COMPRESSOR 6

//Parametros
const int FREQUENCIA_PWM=10;
const int RESOLUCAO_PWM=10;
const int GPIO_0=0;

//Sensores Temperatura
OneWire oneWire(PIN_SENSOR_TEMPERATURA);
DallasTemperature sensors(&oneWire);

//Sensor Pressao
HX711 sensor_pressao;

//Variaveis Globais
bool ESTADO_COMPRESSOR=false;
float CICLO_PWM=0;

void altera_ciclo_pwm_Vsolenoide(float novoCiclo);
float aquisicao_temperatura(int sensor)
float aquisicao_pressao(void);

void setup(){
  //INICIA SERIAL
  Serial.begin(115200);

  //PORTAS
  pinMode(PIN_VALVULA_SOLENOIDE, OUTPUT);
  pinMode(PIN_COMPRESSOR, OUTPUT);

  //CONFIGS PWM VALVULA SOLENOIDE
  ledcAttachPin(PIN_VALVULA_SOLENOIDE, GPIO_0);
  ledcSetup(GPIO_0, FREQUENCIA_PWM, RESOLUCAO_PWM);
  altera_ciclo_pwm_Vsolenoide(0); //inicia com pwm desligado

  //INICIALIZA SENSORES TEMPERATURA
  sensors.begin();

  //INICIALIZA SENSOR PRESSAO
  sensor_pressao.begin(PIN_A_SENSOR_PRESSAO, PIN_B_SENSOR_PRESSAO);
  sensor_pressao.set_scale(2280.f);
  sensor_pressao.tare();
}

void loop(){
  Serial.print("Temperatura: ");
  Serial.print(aquisicao_temperatura(0));
  Serial.print(" | Pressao: ");
  Serial.print(aquisicao_pressao());
  Serial.print(" | Pressao: ");
  Serial.print(aquisicao_pressao());

  delay(3000);
}

void altera_ciclo_pwm_Vsolenoide(float novoCiclo){
  ledcWrite(GPIO_0, novoCiclo*(1023/100));
}

float aquisicao_temperatura(int sensor){
  float temperatura;
  sensors.requestTemperatures();
  temperatura=sensors.getTempCByIndex(sensor);
  return(temperatura);
}

float aquisicao_pressao(void){
  altera_ciclo_pwm_Vsolenoide(0)

  float valor=sensor_pressao.get_units();
  sensor_pressao.power_down();
  _delay_ms(20);
  sensor_pressao.power_up();

  altera_ciclo_pwm_Vsolenoide(CICLO_PWM);

  return(valor);
}

void aciona_compressor(void){
  digitalWrite(PIN_VALVULA_SOLENOIDE, HIGH);
  ESTADO_COMPRESSOR=true;
}

void desliga_compressor(void){
  digitalWrite(PIN_VALVULA_SOLENOIDE, LOW);
  ESTADO_COMPRESSOR=false;
}
