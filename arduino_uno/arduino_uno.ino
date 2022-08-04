#include <OneWire.h>
#include <DallasTemperature.h> //For Temperature Sensor
#include <EEPROM.h> //For TDS Sensor
#include "GravityTDS.h" //For TDS Sensor

#include<SoftwareSerial.h>
SoftwareSerial SUART(5, 6);//SRX = 5, STX = 6

//     For Temperature Sensor -- Start --
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);
float Celcius = 0;
float Fahrenheit = 0;
////For Temperature Sensor -- END --


//     For TDS Sensor -- START --
#define TdsSensorPin A1
GravityTDS gravityTds;

float temperature , tdsValue = 0;

//    For TDS Sensor -- END --


// For Ph Sensor -- Start --
#define phSensorPin A0          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;
// For Ph Sensor -- Start --


void setup(void)
{

  Serial.begin(9600);
  SUART.begin(9600);
  tempSensors.begin(); //For Temperature Sensor

  //    For TDS Sensor -- Start --
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  //    For TDS Sensor -- END --

  SUART.begin(9600);


}

void loop(void)
{
  //For Temperature Sensor -- Start--
  tempSensors.requestTemperatures();
  Celcius = tempSensors.getTempCByIndex(0);
  //  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.println(Celcius);
  //  Serial.print(" F  ");
  //  Serial.println(Fahrenheit);
  //For Temperature Sensor --END--
  delay(500);

  //For TDS Sensor -- Start --
  gravityTds.setTemperature(Celcius);  // set the temperature and execute temperature compensation (using temperature reference from temperature sesnor)
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  delay(500);
  //For TDS Sensor -- END --


  // For Ph Sensor -- Start --
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(phSensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
    avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  phValue = 3.5 * phValue;                  //convert the millivolt into pH value
  Serial.print("    pH: ");
  Serial.println(phValue, 2);

  // For PH sensor -- END --
  delay(500);



  //-------------------------------------------
  //String data = String(Celcius)+String(tdsValue)+String(phValue);
  Serial.print(Celcius);   Serial.print("A");
  Serial.print(tdsValue);   Serial.print("B");
  Serial.print(phValue,2);   Serial.print("C");
  
  //------------------------------------
  delay(500);    //testing
}
