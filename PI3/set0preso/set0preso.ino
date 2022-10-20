/*
* main.cpp
*
* Created: 09/05/2022
* Author : Dudu
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <HX711.h>


//Sensor Pressao
HX711 sensor_pressao;


//Funcoes
float aquisicao_pressao(void);
float aquisicao_pressao2(void);

int main(void){
   Serial.begin(115200);

   //INICIALIZA SENSOR PRESSAO
   sensor_pressao.begin(A1, A2);
   sensor_pressao.set_scale(1);
   sensor_pressao.set_offset(-882917.0);
   //sensor_pressao.tare(10);
   float currentOffset = sensor_pressao.get_offset();

   _delay_ms(1000);

   while(1){
    //Serial.println("AVERAGE / GET_UNITS");
    //Serial.println(aquisicao_pressao());
    Serial.println(aquisicao_pressao2());
    //Serial.println(currentOffset);
    _delay_ms(500);
   }

}

float aquisicao_pressao(void){
  float valor=sensor_pressao.read();
  sensor_pressao.power_down();
  _delay_ms(20);
  sensor_pressao.power_up();
  return(valor);
}


float aquisicao_pressao2(void){
  float valor=sensor_pressao.get_value();
  sensor_pressao.power_down();
  _delay_ms(20);
  sensor_pressao.power_up();
  return(valor);
}
