#include <Wire.h>
#include "Adafruit_TCS34725.h"

//ACCURATE ONLY FROM 0 - 95mm(3.75 in) max

const int COLOR_SAMPLE[][6] = { //ColorTemp, Lux, RGB values, clear value
  {1828, 64828, 2832, 330, 424, 3362},       //red_1 -- close
  {1883, 64929, 2629, 350, 418, 3216},       //red_2 -- 2 in away
  {1940, 65169, 1863, 298, 319, 2361},       //red_3 -- 2 in away

  {7724, 3276, 687, 2746, 1140, 4566},       //green_1 -- close
  {8524, 4531, 801, 3730, 1497, 6022},       //green_2 -- 2 in away
  {1891, 64989, 2471, 344, 394, 3049},       //green_3 -- 2 in away
  // {1895, 64997, 2479, 352, 397, 3069},       //green_4 -- 2 in away

  {1863, 65003, 2268, 293, 355, 2748 },       //blue_1 -- close
  {47607, 64542, 432, 1522, 4450, 6262},      //blue_2 -- 
  {6298, 3063, 748, 2539, 958, 4207},         //blue_3 -- 
  // {35508, 64895, 461, 1370, 3627, 5331},      //blue_4-- 
  // {42844, 64626, 536, 1848, 4992, 7206},      //blue_5 -- 


  {0, 1629, 3146, 4281, 5610, 10240},        //white_1 -- close
  {0, 2459, 3676, 5728, 7362, 10240},        //white_2 -- 
  {0, 1581, 2487, 3455, 4187, 9836},         //white_3 -- 


  {0, 64081, 1676, 1631, 4762, 7770},       //purple_1 -- close
  {0, 63639, 2197, 2196, 6353, 10240},       //purple_2 -- 
  // {11664, 64724, 1228, 1204, 3162, 5417},    //purple_3 -- 
  {11129, 64627, 1518, 1456, 3709, 6463},    //purple_4 -- 


  {2596, 1422, 3542, 2191, 1210, 6681},     //yellow_1 -- close
  {2482, 1085, 2814, 1673, 877, 5166},      //yellow_2 -- 
  {9333, 65067, 1107, 966, 2234, 4170},     //yellow_3 -- 
  // {8815, 65145, 997, 831, 1885, 3590},      //yellow_4 -- 

};
const char* COLOR_NAME[] = {
  "Red", "Red", "Red",
  "Green", "Green", "Green",
  "Blue", "Blue", "Blue",
  "White", "White", "White",
  "Purple", "Purple", "Purple",
  "Yellow" "Yellow", "Yellow",
};

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_16X);

int getColorDis(int red, int green, int blue, int lux, int cTemp, int SAMPLE[5]){
  //ColorTemp, Lux, RGB values, clear value
  // Serial.print("sample testing var: ");Serial.println(SAMPLE[0]);
  return sqrt( 
    pow(red - SAMPLE[2], 2) 
    + pow(green - SAMPLE[3], 2) 
    + pow(blue - SAMPLE[4], 2)  
  );
}


char* discrim(int R, int G, int B, int LUX, int CTEMP){
  int colorDis;               //color dis
  int best_colDis = 3000;            //what it says
  char* possible_color = "NULL";       //least dis

  for(int i=0; i< 18; i++){
      colorDis = getColorDis(R, G, B, LUX, CTEMP,  COLOR_SAMPLE[i]);
      if(colorDis < 50 || colorDis < best_colDis){
          best_colDis = colorDis;
          possible_color = COLOR_NAME[i];
      }
  }
  return possible_color;
}

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}


void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;


  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  delay(1000);

  char* color = discrim(r, g, b, lux, colorTemp);
  Serial.print("Color: ");Serial.println(color);

  // Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  // Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  // Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  // Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  // Serial.print("C: "); Serial.print(c, DEC); Serial.println(" ");
}