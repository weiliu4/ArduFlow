import processing.serial.*;

// The serial port:
Serial myPort;   


void setup () 
{ 
  frameRate(10);  
  
  // Sketch einstellen
  size (300, 300);
  stroke (255);

  // List all the available serial ports:
  println(Serial.list());

  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[1], 115200);
}

int hor,ver;
int started = 0;
int inByte = 0;

int time,lasttime;

void draw () 
{
  while (myPort.available() > 0) {
    inByte = myPort.read();
    if ((inByte & 0xC0) == 0xC0) 
    {
      time = millis();
      println(time - lasttime);
      lasttime = time;
      
      hor = 0;
      ver = 0;
      started = 1;
    }
  
    if (started == 1)
    {
      fill((inByte & 0x3f) * 4);
      rect(hor*10,ver*10,10,10);
      hor++;
      if (hor >= 30) { hor = 0; ver++; }
      if (ver >= 30) { ver = 0; started = 0; }
    }
    /*
    else
    {
      print(hex(inByte));
      print("  ");
    }
    */
  }

}
