// https://codebender.cc/sketch:271084#Neopixel%20Flames.ino

void fireAction()
{
 //  Uncomment one of these RGB (Red, Green, Blue) values to
  //  set the base color of the flame.  The color will flickr
  //  based on the initial base color
  
  //  Regular (orange) flame:
    // int r = 226, g = 121, b = 35;
    int r = 226, g = 100, b = 10;
      
  //  Purple flame:
  //  int r = 158, g = 8, b = 148;

  //  Green flame:
  //int r = 74, g = 150, b = 12;

  //  Flicker, based on our initial RGB values
  for(int i=0; i<pixels.numPixels(); i++) {
    int flicker = random(0,55);
    int r1 = r-flicker;
    int g1 = g-flicker;
    int b1 = b-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
    pixels.setPixelColor(i,r1,g1, b1);
  }
  pixels.show();

  //  Adjust the delay here, if you'd like.  Right now, it randomizes the 
  //  color switch delay to give a sense of realism
  tasker.setTimeout(fireAction, random(10,113) );
}
