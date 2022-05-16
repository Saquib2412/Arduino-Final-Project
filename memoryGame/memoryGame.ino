int redPin = 11;
int greenPin = 10;
int bluePin = 9;

String colors[3] = {"red", "green", "blue"};
String sequence[5] = {};
// test
int knobPin = A0;
int buttonPin = 2;
int trigPin = 6;
int echoPin = 7;
int buzzer = 8;

bool playing = false;
bool sequenceExists = false;
bool sequenceHasBeenShown = false;
int sequenceIndex = 0;

int prevButtonState = 0;
int delayTime = 1000;
int score = 0;

String convertToString() {
  String data;
  for (int i = 0; i < 5; i++) {
    if (i == 4) {
      data += sequence[i];
    } else {
      data += sequence[i] + " ";
    }
  }
  return data;
}

// Take incoming data and convert to array for guessing
// SOURCE: https://forum.arduino.cc/t/how-to-split-a-string-with-space-and-store-the-items-in-array/888813
void fillSequence(String data) {
  int i = 0;
  while (data.length() > 0) {
  
    int index = data.indexOf(' ');
    if (index == -1) {// No space found
    
      sequence[i++] = data;
      break;
    }
    else {
      sequence[i++] = data.substring(0, index);
      data = data.substring(index+1);
    }
  }
}

void RGB(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

// Read data from ultrasonic sensor
int ultrasonicInput() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return distance;
}

void playSequence() {
  for (int i = 0; i < 5; i++) {
    if (sequence[i] == "red") {
      RGB(255, 0, 0);
      delay(delayTime);
      RGB(0, 0, 0);
      delay(delayTime);
    }
    else if (sequence[i] == "green") {
      RGB(0, 255, 0);
      delay(delayTime);
      RGB(0, 0, 0);
      delay(delayTime);
    }
    else if (sequence[i] == "blue") {
      RGB(0, 0, 255);
      delay(delayTime);
      RGB(0, 0, 0);
      delay(delayTime);
    }
  }
  RGB(255, 255, 255);
  delay(500);
  RGB(0, 0, 0);
  delay(500);
}

// Flashes green lights for winning
void playWin() {
  for (int i = 0; i < 5; i++) {
    RGB(0, 255, 0);
    delay(300);
    
    // Buzzer sound
    tone(buzzer, 2000);
    delay(100);
    noTone(buzzer);
    
    RGB(0, 0, 0);
    delay(300);
  }
}

// Flashes red lights for losing
void playLose() {
  for (int i = 0; i < 5; i++) {
    RGB(255, 0, 0);
    delay(300);
    
    // Buzzer sound
    tone(buzzer, 420);
    delay(100);
    noTone(buzzer);
    
    RGB(0, 0, 0);
    delay(300);
  }
}

// Live view of colours while turning knob
void liveView() {
  int knobValue = analogRead(knobPin);
  if (knobValue < 341) {
    RGB(255, 0, 0);
  }
  else if (knobValue > 340 && knobValue < 681) {
    RGB(0, 255, 0);  
  }
  else {
    RGB(0, 0, 255);
  }
}

void generateSequence() {
  String colorSelected;
  String color = sequence[sequenceIndex];
  bool selected = false;
  int prevButtonState = 0;

  while (!selected) {
    int buttonState = digitalRead(buttonPin);
    int knobValue = analogRead(knobPin);
    
    if (buttonState == 1 && buttonState != prevButtonState) {
      if (knobValue < 341) {
        colorSelected = "red";
        selected = true;

        // Buzzer sound
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
      }
      else if (knobValue > 340 && knobValue < 681) {
        colorSelected = "green";
        selected = true;
      
        // Buzzer sound
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
      }
      else {
        colorSelected = "blue";
        selected = true;
      
        // Buzzer sound
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
      }
    }
    sequence[sequenceIndex] = colorSelected;
    sequenceIndex++;
        
    prevButtonState = buttonState;
  }
}

void createSequence() {
  while (sequenceIndex < 5) {
    liveView();
  
    int buttonState = digitalRead(buttonPin);

    if (buttonState == 1 && buttonState != prevButtonState) {
      generateSequence();
      RGB(0,0,0);
      delay(300); 
    }
    prevButtonState = buttonState;
  }
  sequenceIndex = 0;
}

void guessSequence() {
  String colorSelected;
  String color = sequence[sequenceIndex];
  bool selected = false;
  int prevButtonState = 0;
  
  while (!selected) {
    int buttonState = digitalRead(buttonPin);
    int knobValue = analogRead(knobPin);

    liveView();

    if (buttonState == 1 && buttonState != prevButtonState) {
      if (knobValue < 341) {
        colorSelected = "red";
        selected = true;

        // Buzzer sound
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
      }
      else if (knobValue > 340 && knobValue < 681) {
        colorSelected = "green";
        selected = true;
        
        // Buzzer sound
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
      }
      else {
        colorSelected = "blue";
        selected = true;
        
        // Buzzer sound
        tone(buzzer, 1000);
        delay(100);
        noTone(buzzer);
      }
    }
    prevButtonState = buttonState;
  }

  if (colorSelected == color) {
    sequenceIndex++;
    RGB(0, 0, 0);
    delay(300);

    if (sequenceIndex == 5) {
      sequenceIndex = 0;
      sequenceExists = false;
      // delayTime -= 50;
      score++;

      playWin();
      sequenceIndex = 0;
      sequenceHasBeenShown = false;
      sequenceExists = false;
      delay(2000);
    }
  }
  else {
    playLose();

    // Same code block as when winning, may need to change if a score system is implemented
    sequenceIndex = 0;
    sequenceHasBeenShown = false;
    sequenceExists = false;
    delayTime = 1000;
    score = 0;
    delay(2000);
  }
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  randomSeed(analogRead(knobPin));
  Serial.begin(9600);
  Serial.println("Starting");
 
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (playing == false && buttonState == 1 && buttonState != prevButtonState) {
    playing = true;
  }

  while (playing) {
    if (sequenceExists == false) {
      buttonState = digitalRead(buttonPin);
      if (buttonState == 1 && buttonState != prevButtonState) {
        prevButtonState = buttonState;
        createSequence();

        RGB(255, 255, 255);
        delay(500);
        RGB(0, 0, 0);
        delay(500);

        // TODO: Here we should send the data
        bool sent = false;

        while (!sent) {
          int threshold = 3;
          int distance = ultrasonicInput();

          if (distance <= threshold) {
            String data = convertToString();
            Serial.println(data);
            sent = true;

            tone(buzzer, 1175);
            delay(200);
            tone(buzzer, 1319);
            delay(500);
            tone(buzzer, 1047);
            delay(200);
            noTone(buzzer);
          }
        }
        sequenceHasBeenShown = false;
        sequenceExists = true;
        prevButtonState = 0;
      }
    }
    else {
      if (sequenceHasBeenShown == false) {
        buttonState = digitalRead(buttonPin);
        if (buttonState == 1 && prevButtonState != buttonState) {
          sequenceHasBeenShown = true;
          playSequence();
        }
        prevButtonState = buttonState;
      }
      else {
        guessSequence();
      }
    }

    if (prevButtonState != buttonState) {
      prevButtonState = buttonState;
    }
  }
}