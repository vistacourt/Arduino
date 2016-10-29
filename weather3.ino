#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

byte mac[] = {   0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(8080);
DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  dht.begin();
 
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() 
{
  
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        if (c == '\n' && currentLineIsBlank) 
        {
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 3600");  
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          
          float h = dht.readHumidity();
          float t = dht.readTemperature(true);
          float c = dht.readTemperature();
          Serial.println(t);
          Serial.println(h);

         
          client.print("<body bgcolor = 'black'>");
          client.print("<font color='blue' size='1' face = 'arial'>"); 
          client.print("<head><title>Arduino Master Control 2016</title></head><body><h1>Arduino Master Control 2016 Fuck Yeah !!!</h1>");
          client.print("<p>");
          client.print("<iframe src='http://free.timeanddate.com/clock/i4sg7w4u/n137/fs18/fc00f/tc000/pc000/ahl/avt/tt0/tw1/tm1/th2/ta1/tb4' frameborder='0' width='159' height='44'></iframe>");

          client.print("<br><br><br>");
          
          client.print("<font color = 'red' size = '20'>");
          client.print(t);
        
          
          client.print("<font color = 'yellow'>  -  Fahrenheit");
          client.print("<p>");
          client.print("<font color = 'red'>");
          client.print(c);
          client.print("<font color = 'orange'>  -  Celsius");
          client.print("<p>");
          client.print("<font color = 'red'>");
          client.print(h);
          client.print("<font color = 'lightblue'>  -  Humidity");
          client.print("<br><br>");
          client.print("<a href=""http://www.moonconnection.com/current_moon_phase.phtml"" target=""mc_moon_ph""><img src=""http://www.moonmodule.com/cs/dm/hn.gif"" width=""212"" height=""136"" border=""0"" alt="""" /><a href=""http://www.moonconnection.com/moon_cycle.phtml"" target=""mc_moon_ph"">");
          client.print("</p>");
   
          break;
        }
        if (c == '\n') {
          
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          
          currentLineIsBlank = false;
        }
      }
    }
    
    delay(1);
   
    client.stop();
    Serial.println("client disonnected");
  }
}
