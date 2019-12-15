
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

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

int botao1 = 0;

float temp = 0;
float umid = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "GtbDs089gvqc_UGjE3rQeCWzppwOkIIj";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DELTA-28750";
char pass[] = "06128938";

#define DHTPIN 12          // What digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

BLYNK_WRITE(V7)
{
  botao1 = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
}

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  umid = h;
  temp = t;
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  
  tft.fillRect(78, 30, 46, 14, ST7735_GREEN);
  tft.setFont();
  tft.setTextSize(2);
  tft.setCursor(78, 30);
  tft.print(temp, 1);

  tft.fillRect(78, 50, 46, 14, ST7735_GREEN);
  tft.setFont();
  tft.setTextSize(2);
  tft.setCursor(78, 50);
  tft.print(umid, 1);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

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

  tft.setFont(&FreeSansBold9pt7b); // Seleciona a fonte
  tft.setCursor(3, 42); // Move o cursor x y 
  tft.print("TEMP"); // Escreve a palavra Vida Com a cor que foi setada na linha acima
  tft.setCursor(3, 62); // Move o cursor x y 
  tft.print("UMID"); // Escreve a palavra Vida Com a cor que foi setada na linha acima

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(5000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  if (botao1 == 0){
    tft.fillRoundRect(104, 13, 18, 10, 2, ST7735_RED);
  }
  else{
    tft.fillRoundRect(104, 13, 18, 10, 2, ST7735_GREEN);
  }


}
