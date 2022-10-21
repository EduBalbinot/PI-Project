
//Pinos
#define PIN_VALVULA_SOLENOIDE 12
#define PIN_A_SENSOR_PRESSAO 4
#define PIN_B_SENSOR_PRESSAO 5
#define PIN_SENSOR_TEMPERATURA 6

//Parametros
const int FREQUENCIA_PWM=10;
const int RESOLUCAO_PWM=10;
const int GPIO_0=0;

//Variaveis Globais
bool LER_MEDICOES=true;
float ciclo=0;

void aciona_Vsolenoide();
void desliga_Vsolenoide();


void setup(){
  //INICIA SERIAL
  Serial.begin(115200);

  //PORTAS
  pinMode(PIN_VALVULA_SOLENOIDE, OUTPUT);

  //CONFIGS PWM VALVULA SOLENOIDE
  ledcAttachPin(PIN_VALVULA_SOLENOIDE, GPIO_0);
  ledcSetup(GPIO_0, FREQUENCIA_PWM, RESOLUCAO_PWM);

}


void loop(){
  ciclo+=5;
  altera_ciclo_pwm_Vsolenoide(ciclo);
 
  if(ciclo>100)
    ciclo=0;

  delay(3000);
}

void aciona_Vsolenoide(){
  digitalWrite(PIN_VALVULA_SOLENOIDE, HIGH);
}

void desliga_Vsolenoide(){
  digitalWrite(PIN_VALVULA_SOLENOIDE, LOW);
}

void altera_ciclo_pwm_Vsolenoide(float novoCiclo){
  ledcWrite(GPIO_0, novoCiclo*(1023/100));
}
