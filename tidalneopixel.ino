#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUMPIXELS 60
#define NUMPATT 8
#define FLASHDUR 350


float intensity = 0.5;
unsigned long begintime [NUMPATT];
int pattcolor [NUMPATT][3];
int patton [NUMPATT];
int pattturnon [NUMPATT];
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();
  Serial.begin(9600);
  unsigned long nowtime = millis();
  
  for(int i = 0; i < NUMPATT; i++)
    {
      begintime[i] = nowtime;
      patton[i] = 0;
      pattturnon[i] = 0;
      for(int j = 0; j < 3; j++)
        {
          pattcolor[i][j] = (int)random(0, 256);
        };
    };
}

void loop()
{
  int showpix = 0; //flag to update state
  unsigned long curtime = millis();
  for(int i = 0; i < NUMPATT; i++)
    {
      if(patton[i] > 0)
        {
          unsigned long pixtime = begintime[i];
          if(pixtime - curtime > FLASHDUR)
            {
              showpix = 1;
              setcolor(i, 0);
              patton[i] = 0;
            };
        }
      else if (pattturnon[i] > 0)
        {
          showpix = 1;
          begintime[i] = curtime;
          setcolor(i, 1);
          patton[i] = 1;
          pattturnon[i] = 0;
        };
    };
  if(showpix > 0) pixels.show();
}

void serialEvent()
{
  unsigned long curtime = millis();
  while(Serial.available())
    {
      int inbyte = Serial.read();
      if(patton[inbyte] == 0)
          pattturnon[inbyte] = 1;
       else
          begintime[inbyte] = curtime;

    };
}

void setcolor(int idx, int onoff)
{
  int curcolor[3] = {pattcolor[idx][0], pattcolor[idx][1], pattcolor[idx][2]};
  float curintensity = intensity * onoff;
  for(int i = idx; i < NUMPIXELS; i += NUMPATT)
    {
      pixels.setPixelColor(i, (int)(curcolor[0]*curintensity), (int)(curcolor[1]*curintensity), (int)(curcolor[2] * curintensity));
    };
}

