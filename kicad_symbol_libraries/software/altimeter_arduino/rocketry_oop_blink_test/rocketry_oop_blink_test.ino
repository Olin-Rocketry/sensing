// blink test for teensy using oop

class Led {
  // set up pins as private variables
  private:
    byte led_pin;
    
  public:
    // create constructor in public portion
    // call init to initialize the pin modes
    Led(byte led_pin) {
      this->led_pin = led_pin;
      init();  
    }
    
    void init() {
      pinMode(led_pin, OUTPUT);  
    }


    // test functions
    void blink() {
      digitalWrite(led_pin, !digitalRead(led_pin));
      delay(1000);  
    }

    void light_up() {
      digitalWrite(led_pin, HIGH);
      delay(1000);
    }

    void turn_off() {
      digitalWrite(led_pin, LOW);
      delay(1000);  
    }
};


// declare instance of Led as global for scope
const int led_pin = 13;
Led onboard(led_pin);


void setup() {}

void loop() {
  onboard.blink();  
}
