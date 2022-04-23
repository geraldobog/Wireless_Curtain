int Pin1 = 13;//IN1 is connected to 13 
int Pin2 = 12;//IN2 is connected to 12  
int Pin3 = 14;//IN3 is connected to 14 
int Pin4 = 27;//IN4 is connected to 27 

 
int pole1[] ={0,0,0,0, 0,1,1,1, 0};//pole1, 8 step values
int pole2[] ={0,0,0,1, 1,1,0,0, 0};//pole2, 8 step values
int pole3[] ={0,1,1,1, 0,0,0,0, 0};//pole3, 8 step values
int pole4[] ={1,1,0,0, 0,0,0,1, 0};//pole4, 8 step values


int poleStep = 0; 
int  dirStatus = 3;// stores direction status 3= stop (do not change)


String buttonTitle1[] ={"Open", "Closing"};
String buttonTitle2[] ={"Opening", "Closing"};
String argId[] ={"ccw", "cw"};


#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "Gerzki";
const char *password = "12345678";

WebServer server(80);

void handleRoot() {
   //Robojax.com ESP32 Relay Motor Control
 String HTML ="<!DOCTYPE html><html><head><title>ESP32 Curtain Control</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  //HTML += "<style>{ font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  //HTML += ".angleButton { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;";
  //HTML += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  HTML += "<style><link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">";
  HTML += "</style></head><body><h1>Robojax 28BYJ-48 Stepper Motor Control </h1>";

   if(dirStatus ==2){
    HTML +="<h2><span style=\"background-color: #FFFF00\">Opening</span></h2>";    
   }else if(dirStatus ==1){
    HTML +="<h2><span style=\"background-color: #FFFF00\">Closing</span></h2>";      
   }else{
    HTML +="<h2><span style=\"background-color: #FFFF00\">Off</span></h2";    
   }
      if(dirStatus ==1){
        HTML +="  <div class=\"btn\"><a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[0];
        HTML += "=off\">";
        HTML +=buttonTitle1[0]; //motor ON title
      }else{
        HTML +="  <div class=\"btn\"><a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
         HTML += argId[0];
        HTML += "=on\">";       
        HTML +=buttonTitle2[0];//motor OFF title   
      }   
     HTML +="</a></div>";  
           
      if(dirStatus ==2){
        HTML +="<div class=\"btn\"><a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?";
        HTML += argId[1];
        HTML += "=off\">";
        HTML += buttonTitle1[1]; //motor ON title
      }else{
        HTML +="  <div class=\"btn\"><a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/motor?";  
        HTML += argId[1];
        HTML += "=on\">";       
        HTML +=buttonTitle2[1];//motor OFF title   
      }  
      
      
  HTML +="</a> </div>";     

   
   

  HTML +="</body></html>";
  server.send(200, "text/html", HTML);  
}//handleRoot()

void handleNotFound() {
  //   28BYJ-48 Steper Motor Control
  String message = "File Not Found";
  message += "URI: ";
  message += server.uri();
  message += "Method: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "Arguments: ";
  message += server.args();
  message += "";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "";
  }

  server.send(404, "text/plain", message);
  //   28BYJ-48 Steper Motor Control
}//end of handleNotFound()

void setup(void) {
  //   28BYJ-48 Steper Motor Control
 
 pinMode(Pin1, OUTPUT);//define pin for ULN2003 in1 
 pinMode(Pin2, OUTPUT);//define pin for ULN2003 in2   
 pinMode(Pin3, OUTPUT);//define pin for ULN2003 in3   
 pinMode(Pin4, OUTPUT);//define pin for ULN2003 in4   

 
  Serial.begin(115200);//initialize the serial monitor
  Serial.println("Robojax 28BYJ-48 Stepper Motor Control");

  //   28BYJ-48 Steper Motor Control
    
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());

//multicast DNS   //   28BYJ-48 Steper Motor Control
  if (MDNS.begin("espiot")) {
    Serial.println("MDNS responder started");
    Serial.println("access via http://espiot");
  }

  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, motorControl);           
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

}//end of setup

void loop(void) {
    //   28BYJ-48 Steper Motor Control
server.handleClient();
if(dirStatus ==1){ 
   poleStep++; 
    driveStepper(poleStep);    
 }else if(dirStatus ==2){ 
   poleStep--; 
    driveStepper(poleStep);    
 }else{
  driveStepper(8);   
 }
 if(poleStep>7){ 
   poleStep=0; 
 } 
 if(poleStep<0){ 
   poleStep=7; 
 } 

   delay(1);
   //   28BYJ-48 Steper Motor Control
}//end of loop


void motorControl() {


    if(server.arg(argId[0]) == "on")
    {
      dirStatus = 1;// CCW 
               
    }else if(server.arg(argId[0]) == "off"){
      dirStatus = 3;  // motor OFF 
          
    }else if(server.arg(argId[1]) == "on"){
      dirStatus = 2;  // CW  
          
    }else if(server.arg(argId[1]) == "off"){
      dirStatus = 3;  // motor OFF
          
    }  

  

  handleRoot();
}//motorControl end



void driveStepper(int c)
{
     digitalWrite(Pin1, pole1[c]);  
     digitalWrite(Pin2, pole2[c]); 
     digitalWrite(Pin3, pole3[c]); 
     digitalWrite(Pin4, pole4[c]);   
}
