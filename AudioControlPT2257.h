#ifndef AudioControlPT2257_h
#define AudioControlPT2257_h
#include <Arduino.h>

/*
  PT2257 - Electronic Volume Controller IC
  Datasheet - http://www.princeton.com.tw/Portals/0/Product/PT2257.pdf

  Pinout
     |-----_-----|
  1 -| Lin   Rin |- 8
  2 -| Lout Rout |- 7
  3 -| Gnd    V+ |- 6
  4 -| SDA   SCL |- 5
     |-----------|

  The interface protocol consists of the following:
  • A Start bit
  • A Chip Address byte=88H 10001000
  • ACK=Acknowledge bit
  • A Data byte
  • A Stop bit

  Max. clock speed=100K bits/s

  FUNCTION BITS
  MSB    2    3    4    5    6    7    LSB  Function
  ----------------------------------------------------------------------------------------
  1    1    1    1    1    1    1    1    Function OFF (-79dB)
  1    1    0    1    A3   A2   A1   A0   2-Channel, -1dB/step
  1    1    1    0    0    B2   B1   B0   2-Channel, -10dB/step
  1    0    1    0    A3   A2   A1   A0   Left Channel, -1dB/step
  1    0    1    1    0    B2   B1   B0   Left Channel, -10dB/step
  0    0    1    0    A3   A2   A1   A0   Right Channel, -1dB/step
  0    0    1    1    0    B2   B1   B0   Right Channel, -10dB/step
  0    1    1    1    1    0    0    M    2-Channel MUTE (M=1 -> MUTE=ON / M=0 -> MUTE=OFF)

  ATTENUATION UNIT BIT
  A3   AB2  AB1  AB0  ATT(-1) ATT(-10)
  0    0    0    0     0      0
  0    0    0    1    -1    -10
  0    0    1    0    -2    -20
  0    0    1    1    -3    -30
  0    1    0    0    -4    -40
  0    1    0    1    -5    -50
  0    1    1    0    -6    -60
  0    1    1    1    -7    -70
  1    0    0    0    -8
  1    0    0    1    -9

*/

#define PT2257_ADDR 0x88        //Chip address
#define EVC_OFF     0b11111111  //Function OFF (-79dB)
#define EVC_2CH_1   0b11010000  //2-Channel, -1dB/step
#define EVC_2CH_10  0b11100000  //2-Channel, -10dB/step
#define EVC_L_1     0b10100000  //Left Channel, -1dB/step
#define EVC_L_10    0b10110000  //Left Channel, -10dB/step
#define EVC_R_1     0b00100000  //Right Channel, -1dB/step
#define EVC_R_10    0b00110000  //Right Channel, -10dB/step
#define EVC_MUTE    0b01111000  //2-Channel MUTE

class AudioControlPT2257 {
  private:
    uint8_t volume = 0;
    bool mute = false;
	byte level(uint8_t);
	void writeVolume(uint8_t) ;
	void writeMute(bool);
	void off();
	
  public:
	AudioControlPT2257();
    void increase();
    void decrease();
    void toggleMute();
    uint8_t getVolume();
    bool getMute();
    void serialPrintStateJSON();
};
#endif