

import processing.serial.*;
Serial receiver ;
import pt.citar.diablu.processing.mindset.*;
MindSet mindSet;

int throttle   = 0;
int yaw        = 127;
int pitch      = 127;
int roll       = 127;


void setup() {

  size(150, 500);

  receiver  = new Serial(this, "COM10", 115200);
  mindSet = new MindSet(this, "COM5");

  smooth();

  strokeWeight(5);
  stroke(255);
  strokeCap(SQUARE);

  fill(255);
  
} 


void draw()
{
  // Start with a black background
  background(0);

  // Draw horizontal line to at 40% from bottom
  // This line indicates the minimum (40%) attention needed
  line( 0, height*0.60, width, height*.60);

  // Draw a line from the horizontal center upwards
  // This line gives an indication of your attention
  // The height is mapped in reverse to get a percentage from top
  // Example: by 40% (0.4) attention the height value is (100 - 40) 60% (0.6) from top
  line( width*.5, height, width*.5, height*map( float( attentionLevel ) / 100, 0, 1, 1, 0 ) );

  // Push the attention level to the throttle variable
  // 40 = minimum attention needed to do something
  // 100 = maximum attention
  // 30 = 8-bit min value for Arduino
  // 255 = 8-bit max value for Arduino
  throttle = int( map( attentionLevel, 40, 100, 30, 255 ) );

  // Constrain values to 8 bit values to prevent errors
  throttle    = constrain( throttle,  0, 255);
  pitch       = constrain( pitch,     0, 255);
  roll        = constrain( roll,      0, 255);
  yaw         = constrain( yaw,       0, 255);

  // When there is communication possible send the values to the Arduino receiver 
  if ( receiver .available() > 0) 
  {  
    println( "attentionLevel: "+attentionLevel+" throttle: "+throttle+" yaw: "+yaw+" pitch: "+pitch+" roll: "+roll );
    receiver .write( "throttle: "+throttle+" yaw: "+yaw+" pitch: "+pitch+" roll: "+roll );
  }
  
} // draw()

// Killswitch, press K to reset and close the program
void keyPressed() {
  if (key == 'k' || key == ESC) { 
    if ( receiver .available() > 0) 
    {  
      receiver .write("throttle: "+0+" yaw: "+127+" pitch: "+127+" roll: "+127);
      exit();
    }
  }
}


// MindSet variables and functions
int signalStrenght = 0;
int attentionLevel = 0;

public void attentionEvent( int attentionLevel_val ) 
{
  attentionLevel = attentionLevel_val;
}

// This function is activated when the connection with the MindSet is not optimal
public void poorSignalEvent( int signalNoise ) 
{
  // MindSet is adjusting
  if ( signalNoise == 200 ) {
    println( "Mindset is not touching your skin!" );
  }

  // Map the signal strenght to a percentage
  signalStrenght = int( map( ( 200-signalNoise ), 200, 0, 100, 0 ) );
  println( "Signal strength: " + signalStrenght + "%" );
}
