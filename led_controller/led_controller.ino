#define RED 11
#define GRN 10
#define BLU 9
#define PUSHBTN 2
#define POTPIN 0

typedef struct {
  int red;
  int green;
  int blue;
} color;

int selected; // current color selected to be modified by potentiometer
int ledToggle; // toggle to have leds on/off
color mainColor;
int sensorVal; // potentiometer value
int pressLength; // how long pushbutton was held down in milliseconds

int patternToggle; // toggle to have pattern of leds start or stop
color patternColor; // current color that pattern is outputting
int patternIdx; // index of what color we are on
color pattern[7] = { // what pattern of colors to create; current pattern is the rainbow
  {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 0, 255}, {75, 0, 130}, {138, 43, 226}
};


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(2)); // A2 unconnected, will have random analog noise to generate seed

  pinMode(PUSHBTN, INPUT);

  selected = RED; // start with setting red led
  ledToggle = HIGH; // start with leds on

  patternToggle = LOW; // no pattern when first turned on
  patternColor = pattern[0]; // have pattern start at first color
  patternIdx = 0;

  mainColor.red = random(206) + 50; // initilize leds to random rgb color 50 - 255
  mainColor.green = random(206) + 50;
  mainColor.blue = random(206) + 50;
}

void loop() {
  // read roughly the tenths of seconds the button in being held down
  pressLength = 0;
  while (digitalRead(PUSHBTN) == LOW ) {
    delay(100);
    pressLength += 100;
  }

  if (pressLength > 0 && pressLength < 1000) { // user has just pushed button for less than a second
    if (ledToggle) {
      switch (selected) { // switch the selected color
        case RED: selected = GRN;
          break;
        case GRN: selected = BLU;
          break;
        case BLU: selected = RED;
          break;
      }
    }
  }
  else if (pressLength >= 1000 && pressLength < 2000) { // user has held button down for one second
    ledToggle = !ledToggle; // turn off/on leds 
  }
  else if (pressLength >= 2000) { // user held button for two seconds
    if (ledToggle) { // toggle pattern only if leds are on
      patternToggle = !patternToggle; // activate led pattern
    }
  }

  // get potentiometer value
  sensorVal  = analogRead(POTPIN) / 4; // take potentiometer val 0-1023 and divide by 4 to get a RGB value between 0-255
  switch (selected) { // set the value of the selected color
    case RED: mainColor.red = sensorVal;
      break;
    case GRN: mainColor.green = sensorVal;
      break;
    case BLU: mainColor.blue = sensorVal;
      break;
  }

  // set led colors
  if (ledToggle == HIGH) {
    if (patternToggle == LOW) {
      setColor(mainColor.red, mainColor.green, mainColor.blue); // leds on with no pattern
    } else {
      setPatternColor(); // leds on and change depending on pattern
    }
  } else {
    setColor(0, 0, 0); // leds off
  }
}

void setColor(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GRN, g);
  analogWrite(BLU, b);
}

// fade color to color based on the pattern defined at the top
void setPatternColor() {
  color current_color = pattern[patternIdx];
  int nextColorIdx = (patternIdx + 1) % (sizeof(pattern) / sizeof(color)); // mod the index to go back to 0 once gone through all colors
  color next_color = pattern[nextColorIdx]; // color we want to fade into next

  // transform each color by adding/subtracting to r,g,b to get to next color
  if (patternColor.red != next_color.red) {
    if (next_color.red > current_color.red) {
      patternColor.red += 1;
    } else if (next_color.red < current_color.red) {
      patternColor.red -= 1;
    }
  }

  if (patternColor.green != next_color.green) {
    if (next_color.green > current_color.green) {
      patternColor.green += 1;
    } else if (next_color.green < current_color.green) {
      patternColor.green -= 1;
    }
  }

  if (patternColor.blue != next_color.blue) {
    if (next_color.blue > current_color.blue) {
      patternColor.blue += 1;
    } else if (next_color.blue < current_color.blue) {
      patternColor.blue -= 1;
    }
  }

  setColor(patternColor.red, patternColor.green, patternColor.blue); // output new color to leds
  
  if (patternColor.red == next_color.red && patternColor.green == next_color.green && patternColor.blue == next_color.blue) {
    // outputted color is equal to next color so go to next color
    patternIdx = nextColorIdx;
    delay(1000); // delay longer once we reach next color to display that one color for abit longer than normal
  }

  delay(25); // wait a little to get a slower transistion color to color
}

