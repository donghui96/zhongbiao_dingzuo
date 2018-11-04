
#include <dht11.h>//程序中调用了dht11的库
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#define AD5 A5   //定义模拟口A5
#define LED2 13
#define LED1 12//定义数字口13
int Button = 11;         //定义按键脚
int Intensity = 0;//光照度数值
String comdata = "";
String str = "";
int a = 3;

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); 

dht11 DHT11;//定义一个名字叫做DHT11，类型为dht11的值。

#define DHT11PIN 2//定义DHT11的引脚号为D2。

/////////////////摄氏度计算/////////////////////
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}    //摄氏温度度转化为华氏温度

double Kelvin(double celsius)
{
  return celsius + 273.15;
}     //摄氏温度转化为开氏温度

// 露点（点在此温度时，空气饱和并产生露珠）
// 参考: http://wahiduddin.net/calc/density_algorithms.htm
double dewPoint(double celsius, double humidity)
{
  double A0 = 373.15 / (273.15 + celsius);
  double SUM = -7.90298 * (A0 - 1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1) ;
  SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM - 3) * humidity;
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}

// 快速计算露点，速度是5倍dewPoint()
// 参考: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity / 100);
  double Td = (b * temp) / (a - temp);
  return Td;
}
/////////////////////////////////////////////////////////////

void setup()
{
  pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);//设置LED为输出模式
pinMode(Button, INPUT_PULLUP);
digitalWrite(LED1,LOW);
digitalWrite(LED2,LOW);
  Serial.begin(9600);
  u8g2.begin();   

}
//////////////////////////////////////////////////////////////////
void loop()
{
  
  ///////////////按键/////////////////////////////////////////
  int ButtonState = digitalRead(Button); //设置变量存储读到的状态值
   if(ButtonState==0) 
   {
   digitalWrite(LED1,LOW); //判断，如果读到的状态为1，表明按键已经按下，这时候点亮LED
   }
   
 else
   {
    digitalWrite(LED1,HIGH);
   }
  ////////////////DHT11/////////////////////////////////////////
  

  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
    case DHTLIB_OK:
      break;
    case DHTLIB_ERROR_CHECKSUM:
      break;
    case DHTLIB_ERROR_TIMEOUT:
      break;
    default:
      break;
  }
  ///////////////////////光敏电阻///////////////////
Intensity = analogRead(A1);  //读取模拟口AD5的值，存入Intensity变量
  a=5000/Intensity;
  ////////////////////烟雾传感器//////////////////////
  int val;
  val = analogRead(0);
  

   if(val>510) 
   {
   digitalWrite(LED2,HIGH);
   }
   else
   {
    digitalWrite(LED2,LOW);
   } 
   
///////////////////////字符处理///////////////////
  while (Serial.available() > 0)  
    {
        comdata += char(Serial.read());
        delay(2);
    }
    if (comdata.length() > 0)
    {
       
        str =comdata; 
        comdata = "";
        
    }
  ///////////////////////12864///////////////////
   u8g2.firstPage();
    do {
       u8g2.setFont(u8g2_font_5x7_tr); //设置字体//font_ncenB14_tr
      u8g2.setCursor(0, 30);    //设置光标处
       u8g2.print("GZ:");  //输出内容
      u8g2.setCursor(0,15);    //设置光标处
      u8g2.print("T:");  //输出内容
 u8g2.setCursor(60,15);    //设置光标处
 u8g2.print("S:");  //输出内容
 u8g2.setCursor(30,15);    //设置光标处
      u8g2.print("oC");  //输出内容
u8g2.setCursor(100,15);    //设置光标处
      u8g2.print("%");  //输出内容

     
      u8g2.setFont(u8g2_font_5x7_tr); //设置字体
     u8g2.setCursor(15,30);    //设置光标处
      u8g2.print((float)a);  //输出内容
     u8g2.setCursor(15,15);    //设置光标处
      u8g2.print((float)DHT11.temperature);  //输出内容
      u8g2.setCursor(75,15);    //设置光标处
      u8g2.print((float)DHT11.humidity);  //输出内容
       u8g2.setCursor(0,45);    //设置光标处
      u8g2.print(str);  //输出内容
   } while ( u8g2.nextPage() );
 
    }
