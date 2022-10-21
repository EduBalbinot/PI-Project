/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
*********/
#include <HX711.h>


//Sensor Pressao
HX711 sensor_pressao;


//Funcoes
float aquisicao_pressao(void);
float aquisicao_pressao2(void);


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  //INICIALIZA SENSOR PRESSAO
   sensor_pressao.begin(4, 5);
   sensor_pressao.set_scale(1);
   sensor_pressao.set_offset(-882917.0);
   //sensor_pressao.tare(10);
   float currentOffset = sensor_pressao.get_offset();

   delay(1000);A
}

void loop() {
    //Serial.println("AVERAGE / GET_UNITS");

    //Serial.println(aquisicao_pressao());

    Serial.println(aquisicao_pressao2());

    //Serial.println(currentOffset);

    delay(500);
}

float aquisicao_pressao(void){
  float valor=sensor_pressao.read();
  sensor_pressao.power_down();
  delay(20);
  sensor_pressao.power_up();
  return(valor);
}


float aquisicao_pressao2(void){
  float valor=sensor_pressao.get_value();
  sensor_pressao.power_down();
  delay(20);
  sensor_pressao.power_up();
  return(valor);
}
