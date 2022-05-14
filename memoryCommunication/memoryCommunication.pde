import processing.net.*;
import processing.serial.*;

Serial myPort;
String val;

Client c;
String reading;

boolean firstContact = false;

void setup() {
  // Connect to the server’s IP address and port­
  c = new Client(this, "130.229.150.221", 5); // Replace with your server’s IP and port

  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  if (c.available() > 0) {
    reading = c.readString();
    println(reading);
  }
}

void serialEvent (Serial myPort) {
  val = myPort.readStringUntil('\n');
  
  if (val != null) {
    val = trim(val);
    println(val);
    c.write(val);
  
    if (firstContact == false) {
      if (val.equals("A")) {
        myPort.clear();
        firstContact = true;
        myPort.write("A");
        println("contact");
      }
    }
    else {
      myPort.write("A");
    }
  }
}
