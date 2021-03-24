int red_light_pin= 10;
int green_light_pin = 9;
int blue_light_pin = 8;
void setup() {
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
}
void loop() {
  //RGB_color(255, 0, 0); // Red
  //delay(2000);
  //RGB_color(0, 255, 0); // Green
  //delay(2000);
  //RGB_color(0, 0, 255); // Blue
  //delay(2000);
  //RGB_color(255, 110, 51); // Raspberry
  //delay(2000);
  //RGB_color(0, 255, 255); // Cyan
  //delay(2000);
  //RGB_color(255, 0, 130); // Magenta
  //delay(2000);
  RGB_color(255, 180, 0); // Yellow
  //delay(2000);
  //RGB_color(255, 255, 180); // White
  //delay(2000);*/
}
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
