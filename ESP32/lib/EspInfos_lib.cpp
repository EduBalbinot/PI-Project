//
// Created by Edumarek on 10/11/2021.
//
#include "EspInfos_lib.hpp"

//------------------CONSTRUTORES&DESTRUTORES-------------------//
EspInfos::EspInfos(){
  set_macAddress(WiFi.macAddress());
  set_bandwidth(0);
  set_temperatura(-125);
  set_rssI(-1);
  set_novoDutyCycle(0.0);
  set_estadoAerador(false);
  set_pacote();
}

EspInfos::EspInfos(const EspInfos &e){
  _macAddress=e._macAddress;
  bandwidth=e.bandwidth;
  temperatura=e.temperatura;
  rssI=e.rssI;
  novoDutyCycle=e.novoDutyCycle;
  estadoAerador=e.estadoAerador;
  pacote=e.pacote;
}

EspInfos::EspInfos(long _bandwidth){
  set_macAddress(WiFi.macAddress());
  set_bandwidth(_bandwidth);
  set_temperatura(-125);
  set_rssI(-1);
  set_novoDutyCycle(-1);
  set_estadoAerador(false);
  set_pacote();
}
//--------------------------GETS&SETS--------------------------//
void EspInfos::set_macAddress(String m){
  _macAddress=m;
}

void EspInfos::set_bandwidth(long b){
  bandwidth=b;
}

void EspInfos::set_temperatura(float t){
  temperatura=t;
}

void EspInfos::set_rssI(int r){
  rssI=r;
}

void EspInfos::set_novoDutyCycle(float n){
  novoDutyCycle=n;
}

void EspInfos::set_estadoAerador(bool e){
  estadoAerador=e;
}

void EspInfos::set_pacote(){
  pacote=get_macAddress()+String("|")+String(get_bandwidth())+String("|")+String(get_temperatura())+String("|")+String(get_rssI())+String("|")+String(get_novoDutyCycle())+String("|")+String(get_estadoAerador());
}

String EspInfos::get_macAddress(){
  return _macAddress;
}

long EspInfos::get_bandwidth(){
  return bandwidth;
}

float EspInfos::get_temperatura(){
  return temperatura;
}

int EspInfos::get_rssI(){
  return rssI;
}

float EspInfos::get_novoDutyCycle(){
  return novoDutyCycle;
}

bool EspInfos::get_estadoAerador(){
  return estadoAerador;
}

String EspInfos::get_pacote(){
  set_pacote();
  return pacote;
}
//---------------------------METODOS---------------------------//
void EspInfos::print_infos(){
  set_pacote();
  Serial.println(get_pacote());
}

void EspInfos::do_pacote(String dados){
  int i=0;
  while(dados.length()>0){
    switch (i) {
      case 0:
      set_macAddress(dados.substring(0, dados.indexOf("|")));
      break;
      case 1:
      set_bandwidth((dados.substring(0, dados.indexOf("|"))).toInt());
      break;
      case 2:
      set_temperatura((dados.substring(0, dados.indexOf("|"))).toFloat());
      break;
      case 3:
      set_rssI((dados.substring(0, dados.indexOf("|"))).toInt());
      break;
      case 4:
      set_novoDutyCycle(dados.substring(0, dados.indexOf("|")).toFloat());
      break;
      case 5:
      set_estadoAerador((dados.substring(0, dados.indexOf("|"))).toInt());
      break;
    }

    if(dados.indexOf("|")!=-1)
      dados.remove(0, dados.indexOf("|")+1);
    else
      dados.remove(0, dados.indexOf("|"));
    i++;
  }
  set_pacote();
}

JsonObject EspInfos::para_json(){
  JsonObject resposta;

  return resposta;
}

void EspInfos::do_json(JsonObject j){
  String s;

  do_pacote(s);
}
//---------------------------FUNCOES---------------------------//

//----------------------FUNCOES AUXILIARES---------------------//
