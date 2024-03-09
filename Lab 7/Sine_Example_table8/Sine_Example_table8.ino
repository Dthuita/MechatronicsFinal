#define PRINTRATE 417
#define PHASEINC ((64 * 256)/60)  /* Samples per full cycle */
#define COPI 51
#define SCK 52
#define CS 24

#include <Wire.h>
#include <SPI.h>

int inval;
int phase;
long lastPrintTime;
int waveTable[256];

int wave(int phase) {
  if ((phase < 0) || (phase >= (256 * 64))) {
     return 0;
   }
   return waveTable[phase/64];  
}


void setup() {
  phase = 0;
  lastPrintTime = 0;
  init_waveTable();

  SPI.begin();   // initiate SPI as controller
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)) ;
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
}

void loop() {
  if ((micros()-lastPrintTime) > PRINTRATE) {
    DACout(wave(phase));
    lastPrintTime = lastPrintTime + PRINTRATE;
    phase = phase + PHASEINC;
    if (phase >= (256 * 64)) {
      phase = phase - (256*64);
    }
  } 
}

void DACout(unsigned int dacval) {
  dacval = dacval*4 + 0x1000;
  digitalWrite(CS, LOW);
  SPI.transfer16(dacval); 
  digitalWrite(CS, HIGH); 
}

void init_waveTable(){
waveTable[0] = 512;
waveTable[1] = 524;
waveTable[2] = 537;
waveTable[3] = 549;
waveTable[4] = 562;
waveTable[5] = 574;
waveTable[6] = 586;
waveTable[7] = 599;
waveTable[8] = 611;
waveTable[9] = 623;
waveTable[10] = 636;
waveTable[11] = 648;
waveTable[12] = 660;
waveTable[13] = 672;
waveTable[14] = 684;
waveTable[15] = 695;
waveTable[16] = 707;
waveTable[17] = 719;
waveTable[18] = 730;
waveTable[19] = 741;
waveTable[20] = 752;
waveTable[21] = 763;
waveTable[22] = 774;
waveTable[23] = 785;
waveTable[24] = 795;
waveTable[25] = 806;
waveTable[26] = 816;
waveTable[27] = 826;
waveTable[28] = 836;
waveTable[29] = 845;
waveTable[30] = 855;
waveTable[31] = 864;
waveTable[32] = 873;
waveTable[33] = 882;
waveTable[34] = 890;
waveTable[35] = 898;
waveTable[36] = 907;
waveTable[37] = 914;
waveTable[38] = 922;
waveTable[39] = 929;
waveTable[40] = 936;
waveTable[41] = 943;
waveTable[42] = 950;
waveTable[43] = 956;
waveTable[44] = 962;
waveTable[45] = 968;
waveTable[46] = 973;
waveTable[47] = 979;
waveTable[48] = 984;
waveTable[49] = 988;
waveTable[50] = 993;
waveTable[51] = 997;
waveTable[52] = 1000;
waveTable[53] = 1004;
waveTable[54] = 1007;
waveTable[55] = 1010;
waveTable[56] = 1013;
waveTable[57] = 1015;
waveTable[58] = 1017;
waveTable[59] = 1019;
waveTable[60] = 1020;
waveTable[61] = 1021;
waveTable[62] = 1022;
waveTable[63] = 1022;
waveTable[64] = 1023;
waveTable[65] = 1022;
waveTable[66] = 1022;
waveTable[67] = 1021;
waveTable[68] = 1020;
waveTable[69] = 1019;
waveTable[70] = 1017;
waveTable[71] = 1015;
waveTable[72] = 1013;
waveTable[73] = 1010;
waveTable[74] = 1007;
waveTable[75] = 1004;
waveTable[76] = 1000;
waveTable[77] = 997;
waveTable[78] = 993;
waveTable[79] = 988;
waveTable[80] = 984;
waveTable[81] = 979;
waveTable[82] = 973;
waveTable[83] = 968;
waveTable[84] = 962;
waveTable[85] = 956;
waveTable[86] = 950;
waveTable[87] = 943;
waveTable[88] = 936;
waveTable[89] = 929;
waveTable[90] = 922;
waveTable[91] = 914;
waveTable[92] = 907;
waveTable[93] = 898;
waveTable[94] = 890;
waveTable[95] = 882;
waveTable[96] = 873;
waveTable[97] = 864;
waveTable[98] = 855;
waveTable[99] = 845;
waveTable[100] = 836;
waveTable[101] = 826;
waveTable[102] = 816;
waveTable[103] = 806;
waveTable[104] = 795;
waveTable[105] = 785;
waveTable[106] = 774;
waveTable[107] = 763;
waveTable[108] = 752;
waveTable[109] = 741;
waveTable[110] = 730;
waveTable[111] = 719;
waveTable[112] = 707;
waveTable[113] = 695;
waveTable[114] = 684;
waveTable[115] = 672;
waveTable[116] = 660;
waveTable[117] = 648;
waveTable[118] = 636;
waveTable[119] = 623;
waveTable[120] = 611;
waveTable[121] = 599;
waveTable[122] = 586;
waveTable[123] = 574;
waveTable[124] = 562;
waveTable[125] = 549;
waveTable[126] = 537;
waveTable[127] = 524;
waveTable[128] = 512;
waveTable[129] = 500;
waveTable[130] = 487;
waveTable[131] = 475;
waveTable[132] = 462;
waveTable[133] = 450;
waveTable[134] = 438;
waveTable[135] = 425;
waveTable[136] = 413;
waveTable[137] = 401;
waveTable[138] = 388;
waveTable[139] = 376;
waveTable[140] = 364;
waveTable[141] = 352;
waveTable[142] = 340;
waveTable[143] = 329;
waveTable[144] = 317;
waveTable[145] = 305;
waveTable[146] = 294;
waveTable[147] = 283;
waveTable[148] = 272;
waveTable[149] = 261;
waveTable[150] = 250;
waveTable[151] = 239;
waveTable[152] = 229;
waveTable[153] = 218;
waveTable[154] = 208;
waveTable[155] = 198;
waveTable[156] = 188;
waveTable[157] = 179;
waveTable[158] = 169;
waveTable[159] = 160;
waveTable[160] = 151;
waveTable[161] = 142;
waveTable[162] = 134;
waveTable[163] = 126;
waveTable[164] = 117;
waveTable[165] = 110;
waveTable[166] = 102;
waveTable[167] = 95;
waveTable[168] = 88;
waveTable[169] = 81;
waveTable[170] = 74;
waveTable[171] = 68;
waveTable[172] = 62;
waveTable[173] = 56;
waveTable[174] = 51;
waveTable[175] = 45;
waveTable[176] = 40;
waveTable[177] = 36;
waveTable[178] = 31;
waveTable[179] = 27;
waveTable[180] = 24;
waveTable[181] = 20;
waveTable[182] = 17;
waveTable[183] = 14;
waveTable[184] = 11;
waveTable[185] = 9;
waveTable[186] = 7;
waveTable[187] = 5;
waveTable[188] = 4;
waveTable[189] = 3;
waveTable[190] = 2;
waveTable[191] = 2;
waveTable[192] = 1;
waveTable[193] = 2;
waveTable[194] = 2;
waveTable[195] = 3;
waveTable[196] = 4;
waveTable[197] = 5;
waveTable[198] = 7;
waveTable[199] = 9;
waveTable[200] = 11;
waveTable[201] = 14;
waveTable[202] = 17;
waveTable[203] = 20;
waveTable[204] = 24;
waveTable[205] = 27;
waveTable[206] = 31;
waveTable[207] = 36;
waveTable[208] = 40;
waveTable[209] = 45;
waveTable[210] = 51;
waveTable[211] = 56;
waveTable[212] = 62;
waveTable[213] = 68;
waveTable[214] = 74;
waveTable[215] = 81;
waveTable[216] = 88;
waveTable[217] = 95;
waveTable[218] = 102;
waveTable[219] = 110;
waveTable[220] = 117;
waveTable[221] = 126;
waveTable[222] = 134;
waveTable[223] = 142;
waveTable[224] = 151;
waveTable[225] = 160;
waveTable[226] = 169;
waveTable[227] = 179;
waveTable[228] = 188;
waveTable[229] = 198;
waveTable[230] = 208;
waveTable[231] = 218;
waveTable[232] = 229;
waveTable[233] = 239;
waveTable[234] = 250;
waveTable[235] = 261;
waveTable[236] = 272;
waveTable[237] = 283;
waveTable[238] = 294;
waveTable[239] = 305;
waveTable[240] = 317;
waveTable[241] = 329;
waveTable[242] = 340;
waveTable[243] = 352;
waveTable[244] = 364;
waveTable[245] = 376;
waveTable[246] = 388;
waveTable[247] = 401;
waveTable[248] = 413;
waveTable[249] = 425;
waveTable[250] = 438;
waveTable[251] = 450;
waveTable[252] = 462;
waveTable[253] = 475;
waveTable[254] = 487;
waveTable[255] = 500;
}