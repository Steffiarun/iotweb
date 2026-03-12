/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/ 

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "vivo Y02t";
const char* password = "steffiarun1";

// Set web server port number to 80
WiFiServer server(8000);
bool loggedIn = false;

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 2;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 1000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            if (header.indexOf("GET /login?pass=admin123") >= 0) {
  Serial.println("Correct Password - Admin Logged In");
  loggedIn = true;
}
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<style>");
client.println("body { background-color: #F0E68C;font-family: Arial; text-align: center;position: relative;top: 200px; }");
client.println(".button { background-color: #006400; border: none; color: white; padding: 15px 70px;position: relative;top: 150px;}");
client.println("font-size: 10px; margin: 20px; cursor: pointer; }");
client.println(".button2 { background-color: #555555;position:  relative;top: 200px; }");
client.println("</style></head><body>");
          if (!loggedIn) {

  // LOGIN PAGE
  client.println("<h2>Admin Login</h2>");
  client.println("<form action=\"/login\">");
  client.println("<input type=\"password\" name=\"pass\" placeholder=\"Enter Password\">");
  client.println("<br><br><input type=\"submit\" value=\"Login\" class=\"button\">");
  client.println("</form>");

  if (header.indexOf("pass=") >= 0 && !loggedIn) {
    client.println("<p style='color:red;'>Wrong Password</p>");
  }

} else {
   client.println("<h1 style='color:red;'>ESP32 Admin Control</h1>");

  // GPIO 26
  client.println("<p style='color:#DC143C;position:  absolute;top: 200px;'>GPIO 26 - State " + output26State + "</p>");
  if (output26State == "off") {
    client.println("<a href=\"/26/on\"><button class=\"button\">ON</button></a>");
  } else {
    client.println("<a href=\"/26/off\"><button class=\"button button2\">OFF</button></a>");
  }

  // GPIO 27
  client.println("<p>GPIO 27 - State " + output27State + "</p>");
  if (output27State == "off") {
    client.println("<a href=\"/27/on\"><button class=\"button\">ON</button></a>");
  } else {
    client.println("<a href=\"/27/off\"><button class=\"button button2\">OFF</button></a>");
  }

}
client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return charadcter,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}