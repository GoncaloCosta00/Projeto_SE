// Robo India Tutorial
// Simple code upload the tempeature and humidity data using thingspeak.com
// Hardware: NodeMCU,DHT11
#include <MQUnifiedsensor.h>
#include <DHT.h>  // Including library for dht

#include <ESP8266WiFi.h>

String apiKey = "67SVCFG6UDI7VGIJ";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "MEO-COSTA";     // replace with your wifi ssid and wpa2 key
const char *pass =  "c4cddf1f7d";
//const char* server = "api.thingspeak.com";
const char* server = "192.168.1.67"; //ip atual da maquina servidor
//py manage.py runserver 192.168.1.67:80  comando para dar boot no django

/*SENSOR MQ-135*/
#define placa "esp8266"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  


#define DHTPIN 0          //pin where the dht11 is connected

DHT dht(DHTPIN, DHT11);
int pinTring = 4;
int pinEcho = 5;
float distancia = 0;
float percentagem_utilizacao = 0;
WiFiClient client;


MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup()
{
  Serial.begin(115200);
  delay(10);
  dht.begin();
  pinMode(pinTring, OUTPUT);
  pinMode(pinEcho, INPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //sensor MQ-135
  MQ135.setRegressionMethod(1);
  MQ135.init();
  MQ135.setRL(10);
  Serial.print("A proceder à calibração");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i ++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("  done!.");

  if (isinf(calcR0)) {
    Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
    while (1);
  }
  /*****************************  MQ CAlibration ********************************************/


}

void loop()
{

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  digitalWrite(pinTring, LOW);
  delayMicroseconds(250);
  digitalWrite(pinTring, HIGH);
  delayMicroseconds(250);
  digitalWrite(pinTring, LOW);

  distancia = pulseIn(pinEcho, HIGH);
  distancia = distancia / 58;
  Serial.println(distancia);

  if (distancia < 0 || distancia > 50) {
    percentagem_utilizacao = 0;
  }
  else
    percentagem_utilizacao = map(distancia, 0, 50, 100, 0);

  Serial.print(percentagem_utilizacao);
  Serial.println(" %");
  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
  float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
  float Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
  float NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  delay(1500);

  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    /*
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(t);
        postStr += "&field2=";
        postStr += String(h);
        postStr += "\r\n\r\n";*/

    /*String postStr = "&api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "\r\n\r\n";
    */
    String postStr = "&temperatura=" + String(t) + "&humidade=" + String(h) + "&co=" + String(CO) + "&alcohol=" + String(Alcohol) + "&co2=" + String(CO2) + "&nh4=" + String(NH4) + "&perc_utilizacao=" + String(percentagem_utilizacao) + "\r\n";
    //String postStr = "&api_key=" + apiKey + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(percentagem_utilizacao) + "&field4=" + String(CO) + "&field5=" + String(CO2) + "&field6=" + String(Alcohol) + "&field7=" + String(NH4) + "\r\n";

    //client.print("POST /update HTTP/1.1\n");
    client.print("POST /save_info HTTP/1.1\n"); //servidor django
    client.print("Host: ");
    client.print(server);
    client.print("\n");
    client.print("Connection: close\n");
    //client.print("THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);


    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.print("%. Utilizacao: ");
    Serial.print(percentagem_utilizacao);
    Serial.println("%. Send to Thingspeak.");
    Serial.println("|    CO   |  Alcohol |   CO2  |  NH4  |");
    Serial.print("|   "); Serial.print(CO);
    Serial.print("  |   "); Serial.print(Alcohol);
    Serial.print("   |   "); Serial.print(CO2);
    Serial.print(" |  "); Serial.print(NH4);
    Serial.println(" |");
    Serial.println(postStr);

  }
  client.stop();

  Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(15000);
}
