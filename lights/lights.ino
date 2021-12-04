int blue = 6;
int button1 = 2;
int green = 4;
int button2 = 7;
int red = 8;
int button3 = 9;
int yellow = 11;
int button4 = 10;
int white = 13;
int button5 = 12;

int rgb_red = 3;
int rgb_green = 5;
//int rgb_blue = 6;

int note;
int state;
unsigned long tb;
unsigned long tg;
unsigned long tr;
unsigned long ty;
unsigned long tw;
unsigned long tb_press;
unsigned long tg_press;
unsigned long tr_press;
unsigned long ty_press;
unsigned long tw_press;
int b_state;
int g_state;
int r_state;
int y_state;
int w_state;

void setup() {
  Serial.begin(115200);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  //pinMode(rgb_blue, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    note = Serial.read();
    if (note%12 == 0) {
      lightPin(blue);
    } else if (note%12 == 2) {
      lightPin(green);     
    } else if (note%12 == 4) {
      lightPin(red);
    } else if (note%12 == 7) {
      lightPin(yellow);
    } else if (note%12 == 9) {
      lightPin(white);
    }
  }

  if (digitalRead(button1) == LOW && b_state == LOW) {
    b_state = HIGH;
    tb_press = millis();
    int diff = tb_press - tb;
    if (diff > 1000) {
      RGB(255, 0, 0); 
    } else if (diff < 100) {
      RGB(0, 255, 0);
    } else {
      RGB(255, 255-diff/4, 0);
    }
  } else if (digitalRead(button1) == HIGH && b_state == HIGH) {
    b_state = LOW;
    RGB(0, 0, 0);
  }
  if (digitalRead(button2) == LOW && g_state == LOW) {
    g_state = HIGH;
    tg_press = millis();
    int diff = tg_press - tg;
    if (diff > 1000) {
      RGB(255, 0, 0); 
    } else if (diff < 100) {
      RGB(0, 255, 0);
    } else {
      RGB(255, 255-diff/4, 0);
    }
  } else if (digitalRead(button2) == HIGH && g_state == HIGH) {
    g_state = LOW;
    RGB(0, 0, 0);
  }
  if (digitalRead(button3) == LOW && r_state == LOW) {
    r_state = HIGH;
    tr_press = millis();
    int diff = tr_press - tr;
    if (diff > 1000) {
      RGB(255, 0, 0); 
    } else if (diff < 100) {
      RGB(0, 255, 0);
    } else {
      RGB(255, 255-diff/4, 0);
    }
  } else if (digitalRead(button3) == HIGH && r_state == HIGH) {
    r_state = LOW;
    RGB(0, 0, 0);
  }
  if (digitalRead(button4) == LOW && y_state == LOW) {
    y_state = HIGH;
    ty_press = millis();
    int diff = ty_press - ty;
    if (diff > 1000) {
      RGB(255, 0, 0); 
    } else if (diff < 100) {
      RGB(0, 255, 0);
    } else {
      RGB(255, 255-diff/4, 0);
    }
  } else if (digitalRead(button4) == HIGH && y_state == HIGH) {
    y_state = LOW;
    RGB(0, 0, 0);
  }
  if (digitalRead(button5) == LOW && w_state == LOW) {
    w_state = HIGH;
    tw_press = millis();
    int diff = tw_press - tw;
    if (diff > 1000) {
      RGB(255, 0, 0); 
    } else if (diff < 100) {
      RGB(0, 255, 0);
    } else {
      RGB(255, 255-diff/4, 0);
    }
  } else if (digitalRead(button5) == HIGH && w_state == HIGH) {
    w_state = LOW;
    RGB(0, 0, 0);
  }
}

void lightPin(int colour) {
  if (state == HIGH) {
    state = LOW;
  } else {
    state = HIGH;
    if (colour == blue) {
      tb = millis();
    } else if (colour == green) {
      tg = millis();
    } else if (colour == red) {
      tr = millis();
    } else if (colour == yellow) {
      ty = millis();
    } else if (colour == white) {
      tw = millis();
    }
  }
  digitalWrite(colour, state);
}

void RGB(int red_value, int green_value, int blue_value) {
  analogWrite(rgb_red, red_value);
  analogWrite(rgb_green, green_value);
  //analogWrite(rgb_blue, blue_value);
}
