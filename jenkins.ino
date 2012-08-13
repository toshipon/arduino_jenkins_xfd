
/*
  Jenkins Client 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x0D, 0x86, 0xFA };

// Enter a name of Jenkins Server
char serverName[] = "XX.XX.XX.XX";

int numProjects = 1;
String projects[] = {
  "drive-prj-build-regular"
};

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
String result;

void setup() {
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  Ethernet.begin(mac);
  // give the Ethernet shield a second to initialize:
  Serial.println("initializing...");
  delay(1000);
}

void loop()
{

  boolean failed = false;
  
  for(int i=0;i<numProjects;i++){

    Serial.println("connecting...");
  
    // if you get a connection, report back via serial:
    if (client.connect(serverName, 80)) {
      Serial.println("connected: "+projects[i]);
      // Make a HTTP request:
      client.println("GET /jenkins/job/"+projects[i]+"/lastCompletedBuild/api/json?tree=result HTTP/1.0");
      client.println();
    } 
    else {
      Serial.println("connection failed");
    }
  
    delay(1000);
  
    while (client.available()) {     
      char c = client.read();
      result+=c;
    }
  
    Serial.println(result);
  
    if(result.endsWith("{\"result\":\"FAILURE\"}") ||
       result.endsWith("{\"result\":\"UNSTABLE\"}")){
      failed=true;
    } 
    
    result="";
  
    Serial.println("disconnecting.");
    client.stop();
    
    delay(1000);

  }
  
  if(failed){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
  } else {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
  }

  delay(30000);
}

