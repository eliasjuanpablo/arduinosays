/* 
 This sketch is based on A Simple Simon Says Game by Arduino_Scuola (see https://create.arduino.cc/projecthub/Arduino_Scuola/a-simple-simon-says-game-25ba99)

  There are five buttons connected to A0, A1, A2 and A3.
  The buttons are used to insert the sequence.
  There is a piezo buzzer on pin 8.
  
  When a wrong sequence is inserted all the leds will blink for three time very fast otherwhise the
  inserted sequence is correct.
  
  Hardware needed:
  4x pushbuttons
  1x Blue led
  1x Yellow led
  1x Red led
  1x Green Led
  4x 1k resistors
  4x 10k resisors
  jumpers
  1x Piezo buzzer
 
*/

#define LED1_PIN 5
#define LED2_PIN 4
#define LED3_PIN 3
#define LED4_PIN 2

#define BTN1_PIN A0
#define BTN2_PIN A1
#define BTN3_PIN A2
#define BTN4_PIN A3

#define BUZZER_PIN 8


const int MAX_LEVEL = 100;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int notes[6];

int level = 1;

int velocity = 700;

void setup() {
  randomSeed(millis());

  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(BTN3_PIN, INPUT);
  pinMode(BTN4_PIN, INPUT);
  
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  notes[LED1_PIN] = 392;
  notes[LED2_PIN] = 523;
  notes[LED3_PIN] = 659;
  notes[LED4_PIN] = 784;
  
  turn_leds_off();
  Serial.begin(9600); // for debugging purposes
}

void loop()
{  
  if (level == 1) generate_sequence(); // can't put it in setup because it will run only once, thus it would always use the same sequence
  show_sequence();
  get_sequence();
}

void turn_leds_on()
{
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED3_PIN, HIGH);
  digitalWrite(LED4_PIN, HIGH);
}

void turn_leds_off()
{
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
}

void shine_led(int led_number, int duration)
{
  digitalWrite(led_number, HIGH);
  tone(BUZZER_PIN, notes[led_number]);
  delay(duration);
  digitalWrite(led_number, LOW);
  noTone(BUZZER_PIN);
}


void show_sequence()
{
  turn_leds_off();

  for (int i = 0; i < level; i++)
  {
    shine_led(sequence[i], velocity);
    delay(200);
  }
}

void get_sequence()
{
  int pressed = 0; // whether you pressed a button
  int input = 0;
  for (int i = 0; i < level; i++)
  {
    while(pressed == 0)
    {
      if (digitalRead(BTN1_PIN) == LOW)
      {
        pressed = 1;
        shine_led(LED1_PIN, 200);
        your_sequence[i] = LED1_PIN;
      }
      
      if (digitalRead(BTN2_PIN) == LOW)
      {
        pressed = 1;
        shine_led(LED2_PIN, 200);
        your_sequence[i] = LED2_PIN;
      }
      
      if (digitalRead(BTN3_PIN) == LOW)
      {
        pressed = 1;
        shine_led(LED3_PIN, 200);
        your_sequence[i] = LED3_PIN;
      }
      
      if (digitalRead(BTN4_PIN) == LOW)
      {
        pressed = 1;
        shine_led(LED4_PIN, 200);
        your_sequence[i] = LED4_PIN;
      }
    }
    pressed = 0;

    if (your_sequence[i] != sequence[i])
    {
      wrong_sequence();
      return;
    }
  }
  right_sequence();
}

void generate_sequence()
{
  for (int i = 0; i < MAX_LEVEL; i++)
    sequence[i] = random(2,6);
}

void wrong_sequence()
{
  tone(BUZZER_PIN, 400, 1000);
  for (int i = 0; i < 3; i++)
  {
    turn_leds_on();
    delay(250);
    turn_leds_off();
    delay(250);
  }
  delay(1000);

  level = 1;
  velocity = 1000;
}

void right_sequence()
{
  turn_leds_off();
  delay(250);
  
  turn_leds_on();
  delay(500);
  
  turn_leds_off();
  delay(500);
  
  if (level < MAX_LEVEL) level++;
  
  velocity -= 50; // increase difficulty
}
