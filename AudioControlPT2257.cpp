#include <Arduino.h>
#include <Wire.h>
#include "AudioControlPT2257.h"

AudioControlPT2257::AudioControlPT2257(){
	writeMute(false);
	writeVolume(0);
}

void AudioControlPT2257::increase() {
  volume++;
  if (volume > 79) volume = 79;

  writeVolume(volume);

  if (mute) {
    mute = false;
    writeMute(mute);
  }
}

void AudioControlPT2257::decrease() {
  volume--;
  if (volume < 0)volume = 0;

  writeVolume(volume);

  if (mute) {
    mute = false;
    writeMute(mute);
  }
}

void AudioControlPT2257::toggleMute() {
  if (mute) mute = false;
  else mute = true;
  writeMute(mute);
}

void AudioControlPT2257::serialPrintStateJSON() {
  Serial.print("{");
  Serial.print("volume:");
  Serial.print(volume);
  Serial.print("; mute:");
  Serial.print(mute);
  Serial.println("}");
}

byte AudioControlPT2257::level(uint8_t dB) {

  if (dB > 79) dB = 79;

  int b = dB / 10; //get the second digit
  int a = dB % 10; //get the first digit

  b = b & 0b0000111; //limit the most significant digit to 3 bit (7)

  return (b << 4) | a; //return both numbers in one byte (0BBBAAAA)
}


void AudioControlPT2257::writeVolume(uint8_t dB) {
  byte bbbaaaa = level(dB);

  byte aaaa = bbbaaaa & 0b00001111;
  byte bbb = (bbbaaaa >> 4) & 0b00001111;

  Wire.beginTransmission(PT2257_ADDR);
  Wire.write(EVC_2CH_10 | bbb);
  Wire.write(EVC_2CH_1 | aaaa);
  Wire.endTransmission();
}

void AudioControlPT2257::writeMute(bool state) {
  Wire.beginTransmission(PT2257_ADDR);
  Wire.write(EVC_MUTE | (state & 0b00000001));
  Wire.endTransmission();
}

void AudioControlPT2257::off() {
  Wire.beginTransmission(PT2257_ADDR);
  Wire.write(EVC_OFF);
  Wire.endTransmission();
}

uint8_t AudioControlPT2257::getVolume(){
  return volume;
}

bool AudioControlPT2257::getMute(){
  return mute;
}