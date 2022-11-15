/*
  main.cpp

  Created: 09/05/2022
  Author : Dudu
*/
#include <OneWire.h>
#include <DallasTemperature.h>
// #include <HX711.h>
#include <EspInfos_lib.hpp>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Pinos
#define PIN_VALVULA_SOLENOIDE 12
#define PIN_A_SENSOR_PRESSAO 34
#define PIN_B_SENSOR_PRESSAO 39
#define PIN_SENSOR_TEMPERATURA 25
#define PIN_COMPRESSOR 13

//OLED Display
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//LoRa pins
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//Parametros
#define BAND 941654258
const int FREQUENCIA_PWM = 10;
const int RESOLUCAO_PWM = 10;
const int GPIO_0 = 0;

//Sensores Temperatura
OneWire oneWire(PIN_SENSOR_TEMPERATURA);
DallasTemperature sensors(&oneWire);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// //Sensor Pressao
// HX711 sensor_pressao;

//Variaveis Globais
bool ESTADO_COMPRESSOR = false;
float CICLO_PWM = 0;
EspInfos e(BAND);

void altera_ciclo_pwm_Vsolenoide(float novoCiclo);
float aquisicao_temperatura(int sensor);
// float aquisicao_pressao(void);
void aciona_compressor(void);
void desliga_compressor(void);
void comunicacao_EM(String p);
void oled_write(float t, int r, float dc);

void setup() {
  //INICIA SERIAL
  Serial.begin(115200);

  //PORTAS
  pinMode(PIN_VALVULA_SOLENOIDE, OUTPUT);
  pinMode(PIN_COMPRESSOR, OUTPUT);

  //CONFIGS PWM VALVULA SOLENOIDE
  ledcAttachPin(PIN_VALVULA_SOLENOIDE, GPIO_0);
  ledcSetup(GPIO_0, FREQUENCIA_PWM, RESOLUCAO_PWM);
  altera_ciclo_pwm_Vsolenoide(0.5); //inicia com pwm desligado

  //INICIALIZA SENSORES TEMPERATURA
  sensors.begin();

  //INICIA RADIO
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
    while (1);
  //   //INICIALIZA SENSOR PRESSAO
  //   sensor_pressao.begin(PIN_A_SENSOR_PRESSAO, PIN_B_SENSOR_PRESSAO);
  //   sensor_pressao.set_scale(2280.f);
  //   sensor_pressao.tare();

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
}

void loop() {
  e.set_temperatura(aquisicao_temperatura(0));
  oled_write(e.get_temperatura(), e.get_rssI(), e.get_novoDutyCycle());
  comunicacao_EM(e.get_pacote());
  altera_ciclo_pwm_Vsolenoide(e.get_novoDutyCycle());
  if (e.get_estadoAerador() == true)
    aciona_compressor();
  else
    desliga_compressor();
  delay(100);
}

void oled_write(float t, int r, float dc) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("TEMP:");
  display.setCursor(30, 0);
  display.print(t);
  display.setCursor(0, 10);
  display.print("rssI:");
  display.setCursor(30, 10);
  display.print(r);
  display.setCursor(0, 20);
  display.print("DC:");
  display.setCursor(30, 20);
  display.print(dc);
  display.display();
  return;
}

void altera_ciclo_pwm_Vsolenoide(float novoCiclo) {
  if (novoCiclo > 1)
    novoCiclo = 1;
  if (novoCiclo < 0)
    novoCiclo = 0;
  ledcWrite(GPIO_0, novoCiclo * 1023);
}


float aquisicao_temperatura(int sensor) {
  float temperatura;
  sensors.requestTemperatures();
  temperatura = sensors.getTempCByIndex(sensor);
  Serial.print(temperatura);
  Serial.println("C");
  return (temperatura);
}

// float aquisicao_pressao(void){
//   altera_ciclo_pwm_Vsolenoide(0);
//
//   float valor=sensor_pressao.get_units();
//   sensor_pressao.power_down();
//   delay(20);
//   sensor_pressao.power_up();
//
//   altera_ciclo_pwm_Vsolenoide(CICLO_PWM);
//
//   return(valor);
// }

void aciona_compressor(void) {
  digitalWrite(PIN_COMPRESSOR, LOW);
  ESTADO_COMPRESSOR = true;
}

void desliga_compressor(void) {
  digitalWrite(PIN_COMPRESSOR, HIGH);
  ESTADO_COMPRESSOR = false;
}

void comunicacao_EM(String p) { //4
  String LoRaData;

  //Espera Receber algo
  while (!LoRa.parsePacket());

  //Le pacote
  while (LoRa.available())
    LoRaData = LoRa.readString();
  e.set_rssI(LoRa.packetRssi());
  //Armazena informacoes
  String s1 = LoRaData.substring(0, LoRaData.indexOf("|"));
  LoRaData.remove(0, LoRaData.indexOf("|") + 1);
  e.set_novoDutyCycle((LoRaData.substring(0, LoRaData.indexOf("|"))).toFloat());
  LoRaData.remove(0, LoRaData.indexOf("|") + 1);
  e.set_estadoAerador(LoRaData.substring(0, LoRaData.indexOf("|")).toInt());
  LoRaData.remove(0, LoRaData.indexOf("|"));

  //envia informacoes caso:
  if (s1 == "me_envia") {
    LoRa.beginPacket();
    LoRa.print(p);
    while (!LoRa.endPacket()); //espera enviar
  }
}
