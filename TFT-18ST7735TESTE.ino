#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>

#include <CayenneMQTTESP8266.h>
#define CAYENNE_PRINT Serial
#define CAYENNE_DEBUG
char ssid[] = "DELTA-28750";
char wifiPassword[] = "06128938";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "e324a620-d208-11e6-97cc-8758d0339dd8";
char password[] = "bc71f20f8d6dd82acca0273c82c4fd7b1acc571a";
char clientID[] = "78fb8070-7aad-11e9-be3b-372b0d2759ae";

#define TEMP_CHANNEL 0
#define UMID_CHANNEL 1

#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define D6 12
uint8_t DHTPin = D6;
DHT dht(DHTPin, DHTTYPE);
int Temperatura;
int Umidade;
float TempFloat;
float UmidFloat;

#define D3 0
#define D4 2 
#define D5 14 
#define D7 13 
#define D8 15 
#define TFT_CS     D8
#define TFT_RST    D4  
#define TFT_DC     D3
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST); // Instância do objeto tipo Adafruit_ST7735 que recebe como argumentos os pinos de controle
#define TFT_SCLK D5   
#define TFT_MOSI D7
 
void setup(void){
  pinMode(DHTPin, INPUT);
  dht.begin();

  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  
  tft.initR(INITR_BLACKTAB); // Inicializa a tela com um fundo preto
  
  tft.fillScreen(ST7735_WHITE); // Preenche a tela com a cor branca
  tft.drawRect(1, 1, 126, 25, ST7735_BLACK); // x0, y0, x, y, cor
  
  
  tft.setTextColor(ST7735_BLACK); // Seta a cor do texto
  tft.setFont(&FreeSansBold12pt7b); // Seleciona a fonte
  tft.setCursor(3, 21); // Move o cursor x y 
  tft.print("O-BOT"); // Escreve a palavra Vida Com a cor que foi setada na linha acima

  tft.setFont(); // Seleciona a fonte
  tft.setTextSize(1); // Seta o tamanho da fonte de texto para o tamanho
  tft.setCursor(83, 4); // Move o cursor x y 
  tft.print("ORION3D"); // Escreve a palavra Vida Com a cor que foi setada na linha acima
  tft.setCursor(83, 14);
  tft.print("IoT");

  //WIFI CONECTADO
  tft.fillRoundRect(104, 13, 18, 10, 2, ST7735_GREEN);

  tft.setFont();
  tft.setTextSize(2);
  tft.setCursor(3, 28);
  tft.print("AR CONTROL");

}
   
void loop(){
  Cayenne.loop();
  tela_fundo();
  leitura_dht();
}

CAYENNE_OUT(TEMP_CHANNEL)
{
  TempFloat = dht.readTemperature();
  CAYENNE_LOG("Channel %d, value %d", DHTTEMP, Temperature);
  Cayenne.celsiusWrite(TEMP_CHANNEL, TempFloat);
  Temperatura = (int) TempFloat;
  // Write the value received to the digital pin.
  //digitalWrite(ACTUATOR_PIN, value);
}
CAYENNE_OUT(UMID_CHANNEL)
{
  UmidFloat = dht.readHumidity();
  CAYENNE_LOG("Channel %d, value %d", DHTHUM, Humidity);
  Cayenne.virtualWrite(UMID_CHANNEL, UmidFloat, "rel_hum", "p");
  // Write the value received to the digital pin.
  //digitalWrite(ACTUATOR_PIN, value);
}

void tela_fundo(){

  //tft.fillScreen(ST7735_WHITE);
  tft.drawLine(0, 45, 128, 45, ST7735_BLACK); //PRIMEIRA LINHA HORIZONTAL

  tft.setFont();
  tft.setTextSize(1);
  tft.setCursor(3, 49);
  tft.print("TEMPERATURA");
  tft.setCursor(3, 59);
  tft.print("DO AMBIENTE");

  //ESCREVE VALOR DA TEMPERATURA DO AMBIENTE
  //tft.setTextSize(2);
  //tft.setCursor(77, 50);
  //tft.print("0");

  tft.drawRect(104, 50, 3, 3, ST7735_BLACK); // SIMBOLO GRAUS x0, y0, x, y, cor
  
  tft.setCursor(111, 50);
  tft.print("C");

  tft.drawLine(0, 69, 128, 69, ST7735_BLACK); //SEGUNDA LINHA HORIZONTAL
  tft.drawLine(64, 69, 64, 160, ST7735_BLACK); //PRIMEIRA LINHA VERTICAL
  tft.drawLine(0, 114, 128, 114, ST7735_BLACK); //TERCEIRA LINHA HORIZONTAL

  //ESCREVE UNIDADES A B C e D
  tft.setTextSize(1);
  //A
  tft.setCursor(3, 72);
  tft.print("A");
  //C
  tft.setCursor(3, 117);
  tft.print("C");
  //B
  tft.setCursor(67, 72);
  tft.print("B");
  //D
  tft.setCursor(67, 117);
  tft.print("D");

  //ESCREVE TEMPERATURAS A B C e D
  tft.setTextSize(2);
  
  //TEMP A
  tft.setCursor(20, 83);
  tft.print("22");
  //TEMP C
  tft.setCursor(20, 128);
  tft.print("22");
  //TEMP B
  tft.setCursor(83, 83);
  tft.print("22");
  //TEMP D
  tft.setCursor(83, 128);
  tft.print("22");

  //ESCREVE STATUS DE COMUNICAÇÃO DO ESCRAVO
  //A
  tft.fillCircle(53, 90, 5, ST7735_GREEN); //x0, y0, raio, cor
  //C
  tft.fillCircle(53, 135, 5, ST7735_GREEN);
  //B
  tft.fillCircle(116, 90, 5, ST7735_GREEN);
  //D
  tft.fillCircle(116, 135, 5, ST7735_GREEN); 

}

void leitura_dht (){
  //Temperatura = dht.readTemperature(); // Gets the values of the temperature
  //Umidade = dht.readHumidity(); // Gets the values of the humidity
  //ESCREVE VALOR DA TEMPERATURA DO AMBIENTE
  tft.fillRect(77, 50, 26, 16, ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(77, 50);
  tft.print(Temperatura);
  tela_fundo();
  delay(500);  
}
