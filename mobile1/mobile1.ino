#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

Servo myServo;
int ledPin = 3; // choose the pin for the LED
int inPin = 4; // choose the input pin (for a pushbutton)
int inSen = 5; // choose the input pin (for a sensor)
int val_button = 0;     // variable for reading the pin status
int val_sensor = 0; //variavel para leitura do estdo do sensor
int i; //contador e angulo do servo
int ms, tempo_inicio_choro, tempo_inicio_mov;

// Utilize o endereço MAC que está na etiqueta branca da
// sua Galielo
byte mac[] = { 0x98, 0x4F, 0xEE, 0x05, 0xA0, 0xBD };

// IP do servidor e porta do servidor.
//IPAddress server(192,168,1,32);
char server[] = "0.tcp.ngrok.io";
int port = 10290;

EthernetClient client;

// Variaveis para delay de envio de informacoes
const long interval = 250;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inPin, INPUT);    // declare pushbutton as input
  pinMode(inSen, INPUT);  //declara sensor como input
  myServo.attach(6);
  system("ifup eth0");
  Serial.println("Tentando obter um IP:");
  while (!Ethernet.begin(mac)) {
    Serial.println("Erro ao conectar");
  }
  Serial.print("Meu endereco:");
  Serial.println(Ethernet.localIP());

}

void loop(){

      /*início código mobile*/
      val_button = digitalRead(inPin);
      delay(100);
      Serial.print("Choro: "); Serial.println(val_button);
      ms = millis();
      Serial.print("Milisegundos: "); Serial.println(ms);

      if (val_button == 1 || (ms - tempo_inicio_choro <= 60000)) {
        if (val_button == 1){
          tempo_inicio_choro = millis();
          Serial.print("Tempo início Choro: "); Serial.println(tempo_inicio_mov);
        };
        digitalWrite(ledPin, HIGH);
        val_sensor = digitalRead(inSen);
        Serial.print("Movimentação: ");Serial.println(val_sensor);
        delay(100);

        while(val_sensor == 1){
          tempo_inicio_mov = millis();
          Serial.print("Tempo início Movimentação: "); Serial.println(tempo_inicio_mov);

          for(i = 0; i <= 180 ; i = i + 20){
            delay (750);
            myServo.write(i);
            //Serial.print("Angulo Servo: ");Serial.println(i);
            val_sensor = digitalRead(inSen);
            //Serial.print("Movimentação dentro do if: "); Serial.println(val_sensor);
            if (val_sensor == 0) {
              if(ms - tempo_inicio_mov <= 20000){
              //Serial.println("Entrou no break");
              break;
              }
            }
            //Serial.println("Passou pelo break");
            }
          }

      }else{
         digitalWrite(ledPin, LOW);  // turn LED OFF
        }
        /*fim código móbile*/

  if (!client.connected()) {
    Serial.println("Conectando ao servidor");
    //delay(250);
    client.connect(server, port);
  } else {
    // // Realiza o envio de informacoes
    // unsigned long currentMillis = millis();
    // if (currentMillis - previousMillis >= interval){
    //   previousMillis = currentMillis;
    //   if (client.connected()) {
    /*    client.print("Choro bebê: ");client.print(val_button); //Choro
        Serial.println();
        delay(100);
        client.print("Movimentação: ");client.print(val_sensor); //Movimentação do bebê
        Serial.println();
        delay(100);*/

        client.print(val_button); //Choro
        delay(100);
        client.println(val_sensor); //Movimentação do bebê
        delay(100);
    
        
        Serial.println("Enviando... ");
        
      // }
    //}

  }

}
