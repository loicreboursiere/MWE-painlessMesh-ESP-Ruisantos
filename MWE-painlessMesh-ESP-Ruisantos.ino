/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
*/

#include "painlessMesh.h"
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#define   MESH_PREFIX     "painlessMesh-CLICK"
#define   MESH_PASSWORD   "PrendsTesClics"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

String node1_MAC = "A0:20:A6:16:C0:0E"; 
String node2_MAC = "5C:CF:7F:A3:C8:FE";
String node3_MAC = "68:C6:3A:9F:4E:CB";
String node = "Node 0";

char ssid[20] = "MESH-CLICK-main";
char password[20] = "PrendsTesClics";

IPAddress local_ip( 192, 168, 1, 200 );
IPAddress gateway( 192, 168, 1, 1 );
IPAddress subnet( 255, 255, 255, 0 );


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hi from ";
  msg += node;
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  Serial.println("MAC address ");
  Serial.println(WiFi.macAddress());

  WiFi.softAPConfig( local_ip, gateway, subnet );
  WiFi.softAP( ssid, password, 1, false, 10 );

  if( WiFi.macAddress() == node1_MAC ) 
      node = "Node 1 ";
  else if( WiFi.macAddress() == node2_MAC )
      node = "Node 2 ";
  else if( WiFi.macAddress() == node3_MAC )
      node = "Node 3 ";
  else 
      node = "MAC ADDRESS NOT RECOGNIZED";

  Serial.print( "Node is on ");
  Serial.print( node );
  Serial.println( " With IP adrress " );
  Serial.println( WiFi.localIP() );

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  mesh.update();

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);
  // it will run the user scheduler as well

}
