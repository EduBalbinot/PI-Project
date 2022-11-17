#include "EspInfos_lib.hpp"
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Pinos LoRa
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//Endereço servidor
String server = "http://192.168.137.234/";
String enviaphp =  server + "_php/registraMedicoes.php";
String recebephp = server + "_php/lerConfigAerador.php";

// Setup WiFi
WiFiClient client;
HTTPClient http;

const char* ssid = "Mateus";
const char* password = "Mateus1998";

//const char* ssid = "Oeste Telecom Magaiver";
//const char* password = "magaiver02041971";

//const char* ssid = "DUDU7701";
//const char* password = "12131213";

EspInfos **aeradores;
int ultimo_tamanho = -1;

void comunicacao_inicial_DB() { // 1 e 2
  StaticJsonDocument<1000> doc;
  doc = "";
  Serial.print("1. Enviando MAC Address para DB: ");
  Serial.println("[\"24:0A:C4:FA:7D:60\"]");
  http.begin(client, recebephp);
  http.addHeader("Content-Type", "application/json");
  //  int httpResponseCode = http.POST("[\"" + WiFi.macAddress() + "\"]");
  int httpResponseCode = http.POST("[\"24:0A:C4:FA:7D:60\"]");
  String payload = http.getString();
  Serial.print("   Comunicação Inical HTTP Response code: ");
  Serial.println(httpResponseCode);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  for (int i = 0; i < ultimo_tamanho; i++)
    delete aeradores[i];
  free(aeradores);// libera o array
  ultimo_tamanho = doc.size();
  Serial.printf("2. Recebido %i aeradores\n", ultimo_tamanho);
  aeradores = (EspInfos **)malloc(ultimo_tamanho * sizeof(EspInfos*));
  int i = 0;
  for (JsonObject item : doc.as<JsonArray>()) {
    long BANDD = item["freq"].as<int>();
    aeradores[i] = new EspInfos(BANDD);
    aeradores[i]->set_macAddress(item["Mac"].as<String>());
    aeradores[i]->set_novoDutyCycle(item["DC"].as<float>());
    aeradores[i]->set_estadoAerador(item["Ligado"].as<int>());
    i++;
  }
}

String comunicacao_ME(long _band, float _novoDuty, bool _estadoAerador) { //3
  String LoRaData = "";
  //Inicia banda
  String pkg = "me_envia|" + (String)_novoDuty + "|" + (String)_estadoAerador;
  Serial.print("3. Enviando pacote para escravo freq: ");
  Serial.print(_band);
  Serial.print(" - Pacote: ");
  Serial.println(pkg);
  if (!LoRa.begin(_band))//Inicia comunicação LoRa
    while (1);
  LoRa.beginPacket();
  LoRa.print(pkg);
  while (!LoRa.endPacket());  //Espera enviar
  int i = 0;
  do {
    i++;
  } while (!LoRa.parsePacket() && i < 20000); //Espera receber resposta por no máximo 5s
  if (i < 20000) {
    while (LoRa.available()) {  //Le a resposta
      LoRaData = LoRa.readString();
    }
  }
  Serial.print("4. Pacote recebido do escravo: ");
  Serial.println(LoRaData);
  return LoRaData; //Pacote em Json serializado
}

void comunicacao_final_DB() { //5
  StaticJsonDocument<1000> doc;
  String output;
  //  for (int i = 0; i < ultimo_tamanho; i++) {
  for (int i = 0; i < 1; i++) {
    JsonObject doc_0 = doc.createNestedObject();
    //    doc_0["freq"] = aeradores[i]->get_bandwidth();
    //    doc_0["Pressao"] = aeradores[i]->get_pressao();
    doc_0["DC"] = aeradores[i]->get_novoDutyCycle();
    doc_0["MAC"] = aeradores[i]->get_macAddress();
    doc_0["State"] = aeradores[i]->get_estadoAerador();
    doc_0["rssI"] = aeradores[i]->get_rssI();
    doc_0["temp"] = aeradores[i]->get_temperatura();
  }
  serializeJson(doc, output);
  Serial.print("5. Enviando pacote para DB: ");
  Serial.println(output);
  //  String a = "[{\"vazao\":\"2\",\"temp\":\"35\",\"estadoComp\":\"1\",\"dutyCycle\":\"0.2\",\"IdAerador\":\"3\"},{\"vazao\":\"2\",\"temp\":\"35\",\"estadoComp\":\"1\",\"dutyCycle\":\"0.2\",\"IdAerador\":\"3\"}]";
  http.begin(client, enviaphp);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(output);
  Serial.print("    Pacote enviado HTTP Response code: ");
  Serial.println(httpResponseCode);
  Serial.print("    Resposta do envio do pacote DB:");
  Serial.println(http.getString());
}

void setup() {
  //Inicia serial
  Serial.begin(115200);
  //PINOS SPI LORA
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa
  LoRa.setPins(SS, RST, DIO0);
  //Inicia WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Espera conexão
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address and MAC
  Serial.print("IP mestre: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC mestre: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  Serial.println("-------------------------------------INÍCIO DO CICLO-------------------------------------");
  if (WiFi.status() == WL_CONNECTED) {
    comunicacao_inicial_DB();//1 e 2
    int i = 0;
    //    for (int i = 0; i < ultimo_tamanho; i++)
    aeradores[i]->do_pacote(comunicacao_ME(aeradores[i]->get_bandwidth(), aeradores[i]->get_novoDutyCycle(), aeradores[i]->get_estadoAerador()));
    comunicacao_final_DB();//5
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  Serial.println("---------------------------------------FIM DO CICLO---------------------------------------\n");
  delay(1000);
}
