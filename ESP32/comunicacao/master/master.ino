//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6

String LoRaData;

struct EspInfos{
  //atributos -recebidos
  String mac_adress;
  long int bandwidth;
  float temperatura;
  String tempo_leitura;
  bool estado_comunicacao;
  int rssI;
  //atributos -enviados
  int novo_duty_cicle; //xxxx/1000
  //atributos -recebidos/enviados
  bool estado_aerador;
  //pacote
  String pacote;

  //construtores
  EspInfos(){

  }

  //metodos
  void armazena(String dados){
    int i=0;
    while(dados.length()>0){
      switch (i) {
        case 0:
        mac_adress=dados.substring(0, dados.indexOf("|"));
        break;
        case 1:
        bandwidth=(dados.substring(0, dados.indexOf("|"))).toInt();
        break;
        case 2:
        temperatura=(dados.substring(0, dados.indexOf("|"))).toFloat();
        break;
        case 3:
        tempo_leitura=dados.substring(0, dados.indexOf("|"));
        break;
        case 4:
        estado_comunicacao=(dados.substring(0, dados.indexOf("|"))).toInt();
        break;
        case 5:
        rssI=(dados.substring(0, dados.indexOf("|"))).toInt();
        break;
        case 6:
        estado_aerador=(dados.substring(0, dados.indexOf("|"))).toInt();
        break;
      }

      if(dados.indexOf("|")!=-1)
        dados.remove(0, dados.indexOf("|")+1);
      else
        dados.remove(0, dados.indexOf("|"));
      i++;
    }
    para_string();
  }
  void para_string(){
    pacote=mac_adress+String("|")+String(bandwidth)+String("|")+String(temperatura)+String("|")+tempo_leitura+String("|")+String(estado_comunicacao)+String("|")+String(rssI)+String("|")+String(estado_aerador);
  }
};

EspInfos e;

void setup() {
  //INICIA SERIAL
  Serial.begin(115200);

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
    while (1);
}

void loop() {
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    Serial.print("Received packet ");

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData);
      }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    //Serial.print(" with RSSI ");
    //Serial.println(rssi);
  }
   // Dsiplay information
   e.armazena(LoRaData);

   delay(10);
}
