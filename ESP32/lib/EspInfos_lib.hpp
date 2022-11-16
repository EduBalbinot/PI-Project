//
// Created by Edumarek on 10/11/2022.
//

#ifndef ESPINFOS_LIB_HPP
#define ESPINFOS_LIB_HPP

#include <ArduinoJson.h>
#include <WiFi.h>

struct EspInfos{
  //atributos
  String _macAddress;
  long bandwidth;
  float temperatura;
  int rssI;
  float novoDutyCycle;
  bool estadoAerador;
  String pacote;

  //construtores
  EspInfos();
  EspInfos(const EspInfos &e);
  EspInfos(long _bandwidth);

  //sets and gets
  void set_macAddress(String m);
  void set_bandwidth(long b);
  void set_temperatura(float t);
  void set_rssI(int r);
  void set_novoDutyCycle(float n);
  void set_estadoAerador(bool e);
  void set_pacote();

  String get_macAddress();
  long get_bandwidth();
  float get_temperatura();
  int get_rssI();
  float get_novoDutyCycle();
  bool get_estadoAerador();
  String get_pacote();

  //metodos
  void print_infos();
  void do_pacote(String dados);
  JsonObject para_json();
  void do_json(JsonObject j);

};

#endif //ESPINFOS_LIB_HPP
