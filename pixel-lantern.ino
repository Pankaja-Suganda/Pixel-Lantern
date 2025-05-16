#include <FastLED.h>
#include <avr/wdt.h>

#define NUM_LEDS 50

#define DATA_PIN_1 2
#define DATA_PIN_2 3

CRGB leds[NUM_LEDS];

// Layout
#define MID_LED 0
const uint8_t row1[] = {7, 8, 1,   2,  3,  4,  5,  6};
const uint8_t row2[] = {16, 9,  10, 11, 12, 13, 14, 15};
const uint8_t row3[] = {17, 18, 19, 20, 21, 22, 23, 24};
const uint8_t row4[] = {25, 26, 27, 28, 29, 30, 31, 32};
const uint8_t corners[] = {39, 33, 35, 37};

void setup() {
  Serial.begin(115200);

  wdt_enable(WDTO_8S);

  FastLED.addLeds<WS2812, DATA_PIN_1, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, DATA_PIN_2, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

void clearAll() {
  FastLED.clear();
  FastLED.show();
}

void fadeall(int fade){
	for (int i = 0; i < NUM_LEDS; i++){
		leds[i].nscale8(fade);
	}
  wdt_reset();
}

void collase_in(CRGB color, int fade, int delay_value){
  
  for (uint8_t i = 0; i < 8; i++) leds[row4[i]] = color;
  FastLED.show(); 
  fadeall(fade); 
  delay(delay_value);
  
  
  for (uint8_t i = 0; i < 8; i++) leds[row3[i]] = color;
  FastLED.show(); 
  fadeall(fade); 
  delay(delay_value);

  for (uint8_t i = 0; i < 8; i++) leds[row2[i]] = color;
  FastLED.show(); 
  fadeall(fade);
  delay(delay_value);

  for (uint8_t i = 0; i < 8; i++) leds[row1[i]] = color;
  leds[MID_LED] = color;

  for (uint8_t i = 0; i < 8; i++) leds[corners[i]] = color;
  FastLED.show(); 
  fadeall(10);
  delay(300);
  clearAll();
  wdt_reset();

  
}

void expand_out(CRGB color, int fade, int delay_value){

  leds[MID_LED] = CRGB::Pink;
  FastLED.show(); 
  delay(delay_value);
  
  for (uint8_t i = 0; i < 8; i++) leds[row1[i]] = color;
  FastLED.show(); 
  fadeall(fade); 
  delay(delay_value);
  
  
  for (uint8_t i = 0; i < 8; i++) leds[row2[i]] = color;
  FastLED.show(); 
  fadeall(fade); 
  delay(delay_value);

  for (uint8_t i = 0; i < 8; i++) leds[row3[i]] = color;
  FastLED.show(); 
  fadeall(fade);
  delay(delay_value);

  for (uint8_t i = 0; i < 8; i++) leds[row4[i]] = color;
  FastLED.show(); 
  fadeall(fade);
  for (uint8_t i = 0; i < 8; i++) leds[corners[i]] = color;
  fadeall(fade);
  delay(delay_value);
  clearAll();
  wdt_reset();

  
}

void rounding(CRGB color, CRGB corner_color, int fade){
  leds[MID_LED] = CRGB::Pink;
  FastLED.show(); 
  uint8_t  j = 0;
  for (uint8_t i = 0; i < 8; i++){
    // for (uint8_t j = 0; j < 2; j++){

    leds[row1[i + j]] = color;
    leds[row2[i + j]] = color;
    leds[row3[i + j]] = color;
    leds[row4[i + j]] = color;

    if (i%8){
      for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = corner_color;
    }
    FastLED.show(); 
    fadeall(fade); 
    delay(150);
  }

  clearAll();
  wdt_reset();
  
}

void zig_zag(CRGB color, int fade) {
  const uint8_t* rows[] = {row1, row2, row3, row4};
  const uint8_t sizes[] = {7, 9, 9, 10};
  leds[MID_LED] = CRGB::Pink;
  FastLED.show(); 
  delay(100);

  for (int i = 0; i < 4; i++) {
    // clearAll();
    for (int j = 0; j < sizes[i]; j++) {
      int idx = (i % 2 == 0) ? j : (sizes[i] - 1 - j);
      leds[rows[i][idx]] = color;
      FastLED.show(); fadeall(fade); delay(100);
    }
  }

  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = color;
  fadeall(fade); delay(100);
  clearAll();
  wdt_reset();
  
}

void rotating(CRGB color) {
  for (int i = 0; i < 36; i++) {
    leds[i] = color;
    FastLED.show(); 
    fadeall(200);
    delay(40);
  }
  clearAll();
  wdt_reset();
  
}

void diagonal_sweep(CRGB color, int fade, int delay_value) {
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = color;
  fadeall(fade); delay(100);

  const uint8_t diagonals[][8] = {
    {row1[0]},
    {row1[1], row2[0]},
    {row1[2], row2[1], row3[0]},
    {row1[3], row2[2], row3[1], row4[0]},
    {row1[4], row2[3], row3[2], row4[1]},
    {row1[5], row2[4], row3[3], row4[2]},
    {row1[6], row2[5], row3[4], row4[3]},
    {row1[7], row2[6], row3[5], row4[4]},
    {row2[7], row3[6], row4[5]},
    {row3[7], row4[6]},
    {row4[7]}
  };

  for (int i = 0; i < sizeof(diagonals) / sizeof(diagonals[0]); i++) {
    for (int j = 0; j < 8; j++) {
      if (diagonals[i][j] != 0) leds[diagonals[i][j]] = color;
    }
    FastLED.show();
    fadeall(fade);
    delay(delay_value);
  }

  clearAll();
  wdt_reset();
  
}

void pattern_1(){
  expand_out(CRGB::Blue, 50, 200);
  expand_out(CRGB::Yellow,  50, 200);
  expand_out(CRGB::Red, 50, 200);
  expand_out(CRGB::White,  50, 200);
  expand_out(CRGB::OrangeRed, 50, 200);
}

void pattern_2(){
  expand_out(CRGB::Blue, 50, 200);
  collase_in(CRGB::Blue, 50, 200);
  expand_out(CRGB::Yellow,  50, 200);
  collase_in(CRGB::Yellow, 50, 200);
  expand_out(CRGB::Red, 50, 200);
  collase_in(CRGB::Red, 50, 200);
  expand_out(CRGB::White,  50, 200);
  collase_in(CRGB::White, 50, 200);
  expand_out(CRGB::OrangeRed, 50, 200);
  collase_in(CRGB::OrangeRed, 50, 200);
}

void pattern_3(bool in_fade_mute, bool out_fade_mute){
  leds[MID_LED] = CRGB::Pink;
  FastLED.show(); 
  delay(200);
  
  for (uint8_t i = 0; i < 8; i++) leds[row1[i]] = CRGB::Blue;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::Blue;
  FastLED.show(); 
  if (out_fade_mute) fadeall(10); 
  delay(200);
  
  
  for (uint8_t i = 0; i < 8; i++) leds[row2[i]] = CRGB::Yellow;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::Yellow;
  FastLED.show(); 
  if (out_fade_mute) fadeall(10); 
  delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row3[i]] = CRGB::Red;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::Red;
  FastLED.show(); 
  if (out_fade_mute) fadeall(10); 
  delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row4[i]] = CRGB::White;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::White;
  FastLED.show(); 
  if (out_fade_mute) fadeall(10); 
  delay(200);
  
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::Orange;
  FastLED.show(); 
  fadeall(10);
  delay(200);
  clearAll();

  for (uint8_t i = 0; i < 8; i++) leds[row4[i]] = CRGB::Blue;
  FastLED.show(); 
  if (in_fade_mute) fadeall(10); 
  delay(200);
  
  
  for (uint8_t i = 0; i < 8; i++) leds[row3[i]] = CRGB::Yellow;
  FastLED.show(); 
  if (in_fade_mute) fadeall(10); 
  delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row2[i]] = CRGB::Red;
  FastLED.show(); 
  if (in_fade_mute) fadeall(10); 
  delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row1[i]] = CRGB::White;
  FastLED.show(); 
  if (in_fade_mute) fadeall(10); 
  delay(200);

  clearAll();
  wdt_reset();
}

void pattern_4(int fade){
  rounding(CRGB::Blue, CRGB::Blue, fade);
  rounding(CRGB::Yellow, CRGB::Yellow, fade);
  rounding(CRGB::Red, CRGB::Red, fade);
  rounding(CRGB::White, CRGB::White, fade);
  rounding(CRGB::OrangeRed, CRGB::OrangeRed, fade);
}

void pattern_5(int fade){
  CRGB colors[] = {CRGB::Blue, CRGB::Yellow, CRGB::Red, CRGB::White, CRGB::OrangeRed};
  leds[MID_LED] = CRGB::Pink;
  FastLED.show(); 
  for (uint8_t i = 0; i < 8; i++){
    for (uint8_t j = 0; j < 5; j++){
      leds[row1[i + j]] = colors[j];
      leds[row2[i + j]] = colors[j];
      leds[row3[i + j]] = colors[j];
      leds[row4[i + j]] = colors[j];
    }

    if (i%8){
      for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::White;
    }
    FastLED.show(); 
    fadeall(fade); 
    delay(150);
  }
  
  clearAll();
}

void pattern_6(int fade){
  CRGB colors[] = {CRGB::Blue, CRGB::Yellow, CRGB::Red, CRGB::White, CRGB::OrangeRed};
  leds[MID_LED] = CRGB::Pink;
  FastLED.show(); 
  for (uint8_t i = 0; i < 8; i++){
    for (uint8_t j = 0; j < 5; j++){
      leds[row1[i + j]] = colors[0];
      leds[row2[i + j]] = colors[1];
      leds[row3[i + j]] = colors[2];
      leds[row4[i + j]] = colors[3];
    }

    if (i%8){
      for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = colors[4];
    }
    FastLED.show(); 
    fadeall(fade); 
    delay(150);
  }
  
  clearAll();
}

void pattern_7(){
  zig_zag(CRGB::Blue, 100);
  zig_zag(CRGB::Yellow, 100);
  zig_zag(CRGB::Red, 100);
  zig_zag(CRGB::White, 100);
  zig_zag(CRGB::OrangeRed, 100);
}

void pattern_8(){
  rotating(CRGB::Blue);
  rotating(CRGB::Yellow);
  rotating(CRGB::Red);
  rotating(CRGB::White);
  rotating(CRGB::OrangeRed);
}

void short_pattern_6(CRGB color_1, CRGB color_2, CRGB color_3, CRGB color_4) {
  leds[MID_LED] = color_1;
  FastLED.show(); delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row1[i]] = color_1;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::ForestGreen;
  FastLED.show(); fadeall(20); delay(200);
  
  for (uint8_t i = 0; i < 8; i++) leds[row2[i]] = color_2;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::LimeGreen;
  FastLED.show(); fadeall(20); delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row3[i]] = color_3;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::ForestGreen;
  FastLED.show(); fadeall(20); delay(200);

  for (uint8_t i = 0; i < 8; i++) leds[row4[i]] = color_4;
  for (uint8_t i = 0; i < 4; i++) leds[corners[i]] = CRGB::Olive;
  FastLED.show(); 
  fadeall(10); 
  delay(200);
  clearAll();
  wdt_reset();
  
}

void short_pattern_5() {
  for (int i = 0; i < 4; i++) leds[corners[i]] = CRGB::OrangeRed;
  FastLED.show(); delay(300);
  clearAll(); delay(300);
  wdt_reset();
}

void short_pattern_4() {
  for (int b = 0; b < 255; b += 10) {
    leds[MID_LED].setRGB(b, b / 2, 0);
    FastLED.show(); delay(10);
  }
  for (int b = 255; b >= 0; b -= 10) {
    leds[MID_LED].setRGB(b, b / 2, 0);
    FastLED.show(); delay(10);
  }
  clearAll();
  wdt_reset();
}

// 8. Outer-to-Center Zoom
void thoran8() {
  for (int i = 9; i >= 0; i--) {
    if (i < 10) leds[row4[i]] = CRGB::Green;
    if (i < 9)  leds[row3[i]] = CRGB::Lime;
    if (i < 9)  leds[row2[i]] = CRGB::Chartreuse;
    if (i < 7)  leds[row1[i]] = CRGB::White;
    FastLED.show(); delay(80);
  }
  leds[MID_LED] = CRGB::White;
  FastLED.show(); delay(300);
  clearAll();
}

void short_pattern_3(CRGB mid, CRGB corner) {
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 4; i++) leds[corners[i]] = corner;
    leds[MID_LED] = mid;
    FastLED.show(); delay(250);
    clearAll(); delay(150);
  }
  wdt_reset();
}

void fillRow(const uint8_t row[], uint8_t len, CRGB color) {
  for (uint8_t i = 0; i < len; i++) {
    leds[row[i]] = color;
  }
  wdt_reset();
}


void short_pattern_2(CRGB color) {
  for (int i = 0; i < 3; i++) {
    clearAll();
    for (uint8_t j = 0; j < 4; j++) leds[corners[j]] = color;
    FastLED.show(); delay(300);
    clearAll(); FastLED.show(); delay(300);
  }
  wdt_reset();
}


void short_pattern_1(CRGB color) {
  for (int i = 0; i < 3; i++) {
    leds[MID_LED] = color; FastLED.show(); fadeall(10);  delay(100);
    leds[MID_LED] = CRGB::Black; FastLED.show(); fadeall(10); delay(100);
  }
  fillRow(row1, 7, color); FastLED.show(); fadeall(10); delay(100);
  fillRow(row2, 9, color); FastLED.show(); fadeall(10); delay(100);
  fillRow(row3, 9, color); FastLED.show(); fadeall(10); delay(100);
  fillRow(row4, 10, color); FastLED.show(); fadeall(10); delay(300);

  clearAll();
  wdt_reset();
}

void pattern_9() {
  diagonal_sweep(CRGB::Blue, 80, 100);
  diagonal_sweep(CRGB::Yellow, 80, 100);
  diagonal_sweep(CRGB::Red, 80, 100);
  diagonal_sweep(CRGB::White, 80, 100);
  diagonal_sweep(CRGB::OrangeRed, 80, 100);
}

// void pattern_10(CRGB color, int fade, int delay_value) {
//   const uint8_t* rows[] = {row1, row2, row3, row4};
//   for (int d = 0; d <= 10; d++) { // Max diagonal sum is 3+7 = 10
//     for (int row = 0; row < 4; row++) {
//       int col = d - row;
//       if (col >= 0 && col < 8) {
//         leds[rows[row][col]] = color;
//       }
//     }
//     FastLED.show();
//     fadeall(fade);
//     delay(delay_value);
//   }
//   clearAll();;
// }

void lightUpRow(const uint8_t* row, uint8_t length, CRGB color) {
  for (uint8_t i = 0; i < length; i++) {
    leds[row[i]] = color;
  }
}

void flashCorners(CRGB color) {
  for (uint8_t i = 0; i < 4; i++) {
    leds[corners[i]] = color;
  }
}

void pattern_10(CRGB color1, CRGB color2, CRGB color3, CRGB color4) {
  // Step 1: Center LED
  leds[MID_LED] = color1;
  FastLED.show(); delay(150);

  // Step 2: Light up rows in sequence
  lightUpRow(row1, 8, color1); flashCorners(CRGB::DarkGreen); FastLED.show(); delay(150);
  lightUpRow(row2, 8, color2); flashCorners(CRGB::MediumSeaGreen); FastLED.show(); delay(150);
  lightUpRow(row3, 8, color3); flashCorners(CRGB::SeaGreen); FastLED.show(); delay(150);
  lightUpRow(row4, 8, color4); flashCorners(CRGB::DarkOliveGreen); FastLED.show(); delay(150);

  // Step 3: Gentle fade out
  fadeall(20);
  delay(200);
  clearAll();
}



void loop() {
  Serial.println("Pattern: Start!");

  for (int i = 0; i < 3; i++) pattern_1(); 
  for (int i = 0; i < 3; i++) pattern_2(); 
  for (int i = 0; i < 1; i++) pattern_3(false, false); 
  for (int i = 0; i < 2; i++) pattern_3(false, true); 
  for (int i = 0; i < 2; i++) pattern_3(true, false); 
  for (int i = 0; i < 1; i++) pattern_3(true, true); 
  for (int i = 0; i < 1; i++) pattern_4(10);  
  for (int i = 0; i < 2; i++) pattern_4(100); 
  for (int i = 0; i < 3; i++) pattern_5(100); 
  for (int i = 0; i < 3; i++) pattern_6(100); 

  pattern_9();

  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Pink, CRGB::HotPink, CRGB::Purple, CRGB::Magenta); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::LightPink, CRGB::Pink, CRGB::HotPink, CRGB::DeepPink); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Yellow, CRGB::Gold, CRGB::Orange,  CRGB::SaddleBrown); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Red, CRGB::Magenta, CRGB::OrangeRed,  CRGB::Crimson); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::White, CRGB::LemonChiffon, CRGB::LightYellow,  CRGB::Gold); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Maroon, CRGB::DarkRed, CRGB::Red,  CRGB::PaleVioletRed); 

  short_pattern_1(CRGB::Blue); 
  short_pattern_1(CRGB::Yellow); 
  short_pattern_1(CRGB::Red); 
  short_pattern_1(CRGB::White); 
  short_pattern_1(CRGB::OrangeRed); 

  for (int i = 0; i < 1; i++) pattern_7(); 
  for (int i = 0; i < 1; i++) pattern_8(); 

  pattern_9();

  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Pink, CRGB::HotPink, CRGB::Purple, CRGB::Magenta); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::LightPink, CRGB::Pink, CRGB::HotPink, CRGB::DeepPink); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Yellow, CRGB::Gold, CRGB::Orange,  CRGB::SaddleBrown); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Red, CRGB::Magenta, CRGB::OrangeRed,  CRGB::Crimson); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::White, CRGB::LemonChiffon, CRGB::LightYellow,  CRGB::Gold); 
  for (int i = 0; i < 2; i++) short_pattern_6(CRGB::Maroon, CRGB::DarkRed, CRGB::Red,  CRGB::PaleVioletRed); 

  pattern_9();

  Serial.println("Pattern: End!");
  // pattern_10(CRGB::Pink, CRGB::HotPink, CRGB::Purple, CRGB::Magenta);
  
}

