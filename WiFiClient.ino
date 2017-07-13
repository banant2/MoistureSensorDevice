/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "BA Network";
const char* password = "3146308066";
const char* host = "api.thingspeak.com";
const char* streamId = "field5";
const char* APIKey = "UI2614YOMH9BVM73";

double showMedian(int array[]);
void isort(int a[]);
void wifiConnect(char* ssid, char* password);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(D7, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(D8, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

int value = 0;
int loopcounter=0;
int results [4];

void loop() { 

  delay(1000);  

  digitalWrite(D7, HIGH);  // Turn the LED off by making the voltage HIGH

  value = analogRead(A0);
  
  digitalWrite(D7, LOW);  // Turn the LED off by making the voltage HIGH
  digitalWrite(D8, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(D8, LOW);  // Turn the LED off by making the voltage HIGH

  results[loopcounter] = value;
  
  delay(1000);
      
  ++loopcounter;
  
  if (loopcounter > 4)
  {
    isort(results);
    
    Serial.print("results are: ");
    //int median = showMedian(results);
    for (int i = 0; i< 5; i++) 
    {
    Serial.println(results[i]); 
      }

    int median = results[2];
    
    Serial.print("Median is: ");
    Serial.println(median);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

      // We now create a URL for the request
    String url = "/update?";
    url += "key=";
    url += APIKey;
    url += "&field5=";
    url += median;
  
    Serial.print("Requesting URL: ");
    Serial.println(url);
  
    // This will send the request to the server
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n" + 
               "Accept: */*\r\n" +
               "User-Agent: Mozilla/4.0  (compatible; esp8266 Lua; Windows NT 5.1)\r\n" +
               "\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    Serial.println();
    Serial.println("closing connection");
    
    ESP.deepSleep(1e8);
    }
}

void wifiConnect(char* ssid, char* password)
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

double showMedian(int array[])
{
    int middle = sizeof(array)/2;
    Serial.println(middle);
    
    int average, median;

    if (sizeof(array) % 2)
    {
        median = (array[middle] + array[middle + 1]) / 2.0;

    }
    else
    {
        median = array[middle];

    }
    return median;
}

void isort(int a[])
{
 for (byte i = 1; i < sizeof(a)+1; ++i)
 {
   int j = a[i];
   int k;

   for (k = i - 1; (k >= 0) && (j < a[k]); k--)
   {
     a[k + 1] = a[k];
   }
   a[k + 1] = j;
 }
}
