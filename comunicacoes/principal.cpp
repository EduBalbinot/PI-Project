StaticJsonDocument<1000> PACOTE;



struct EspInfos{
  //atributos -recebidos
  String macAdress;
  long bandwidth;
  float temperatura;
  bool estadoComunicacao;
  int rssI;
  //atributos -enviados
  int novoDutyCycle; //xxxx/1000
  //atributos -recebidos/enviados
  bool estadoAerador;
  //pacote

  //construtores
  EspInfos(){

  }

  //sets and gets
  void set_macAdress(){}
  void set_bandwidth(){}
  void set_temperatura(){}
  void set_estadoComunicacao(){}
  void set_rssI(){}
  void set_novoDutyCycle(){}
  void set_estadoAerador(){}

  String get_macAdress(){}
  Long get_bandwidth(){}
  float get_temperatura(){}
  bool get_estadoComunicacao(){}
  int get_rssI(){}
  int get_novoDutyCycle(){}
  bool get_estadoAerador(){}

  //metodos
  void para_json(){

  }

  void do_jason(){

  }



};


void comunicacao_inicial_DB(){ //1 e 2
  EspInfos[10] arrayInfos;
  http.begin(client, recebephp);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("[\"00-0C-23-D3-89-A1\"]");
  String payload = http.getString();
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  //deserializa e verifica erro
  DeserializationError error = deserializeJson(PACOTE, payload);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  for(int i=0;i<adsd;dadsda) //descobrir desserializacao
    //comuta as infos do json para o arrayInfos
    arrayInfos[i].do_json();

}

void comunicacao_ME(){ //3

}

void comunicacao_EM(){ //4

}

void comunicacao_final_DB(){ //5
  
}
