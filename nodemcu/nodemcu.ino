#include<SoftwareSerial.h>
SoftwareSerial SUART(D2, D1);//SRX = D2, STX = D1

char c;
String dataIn;
int8_t indexOfA, indexOfB, indexOfC;

String data1 , data2, data3;

void setup()
{
  Serial.begin(57600);
  SUART.begin(9600);
}

void loop()
{
  if (SUART.available())
  {
    while (SUART.available() > 0)
    {
      c = SUART.read();



      if (c == '\n') {
        break;
      }
      else {
        dataIn += c;
      }
    }
    if (c == '\n')
    {
      Parse_the_Data();

      upData();
      //      Serial.println(data1);
      //      delay(2000);
      //      Serial.println(data2);
      //      delay(2000);
      //      Serial.println(data3);
      //      delay(2000);
      //      Serial.println("==================");

      c = 0;
      dataIn = "";
    }
  }
}

void upData() {
  Serial.println("==================");
  Serial.println(data1);
  delay(200);
  Serial.println(data2);
  delay(200);
  Serial.println(data3);
  delay(200);
  Serial.println("==================");
  delay(2000);
}
void Parse_the_Data() {
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");

  data1 = dataIn.substring ( 0, indexOfA);
  data2 = dataIn.substring ( indexOfA + 1, indexOfB);
  data3 = dataIn.substring ( indexOfB + 1, indexOfC);
}
