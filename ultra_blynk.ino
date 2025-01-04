
/*
NODEMCU code for the smart dustbin for serial communication with arduino and 
sending the same to blynk app. This way we update the level display in the app as per the current 
percentage occupancy of the dustbin.
THRESHOLD = 70% --> Above this level the lid won't open automatically until the Dusbin is emptied.
 
*/
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "dWMyL0k_ddp2JHLeB9VO29GV7rzi53dB";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "JioFiber-7FuAM";
char pass[] = "mahaveer@729";

BlynkTimer timer;

SoftwareSerial mySerial (D1, D2); //Rx, Tx
int percent;

BLYNK_READ(V5)
{
  // This command writes Dustbin percentage to Virtual Pin (5)
  Blynk.virtualWrite(V5, percent);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  mySerial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void alert(int a)
{
  if(a>=70)
  {
    Blynk.notify("Trash Can Full!!!");
  }
}

void loop()
{
  //read value from arduino transmitted to nodeMCU
  String msg = mySerial.readStringUntil('\n');

  //convert that to integer
  percent = msg.toInt();

  //print on serial monitor
  Serial.println(percent);

  //run the blynk app 
  Blynk.run();

  //send notification alert when dustbin level crosses the threshold value
  alert(percent);
  delay(1000);
}
