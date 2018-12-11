#include <MeAuriga.h>
#include <SoftwareSerial.h>
#include <Wire.h>

////////////////////////////////////////////////////////////////////////////////
// CONSTANTS

// Motors power
#define POWER 100

// This depends on your configuration
#define ENC_RIGHT_PORT         SLOT_1
#define ENC_LEFT_PORT          SLOT_2
#define LINE_SENSOR_PORT       PORT_9


////////////////////////////////////////////////////////////////////////////////
// CLASSES

struct LineFollower {

  // Singleton Object
  static LineFollower& instance()
  {
    static LineFollower INSTANCE;
    return INSTANCE;
  }

  /**
   * setup: setup the encoder motors
   *
   * Note: it must be called in the arduino setup()
   *
   * Note: it is called _setup to prevent the "cannot overload" bug in arduino IDE 1.8
   */
  void _setup()
  {
    attachInterrupt(enc_right.getIntNum(), isr_process_encoder_right, RISING);
    attachInterrupt(enc_left.getIntNum(), isr_process_encoder_left, RISING);
  }

  /**
   * loop: follow the line and set motors PWM accordingly
   *
   * Line follower code from Auriga firmware (https://github.com/Makeblock-official/Makeblock-Libraries/blob/master/examples/Firmware_for_Auriga/Firmware_for_Auriga.ino#L2769)
   * Note: it is called _loop to prevent the "cannot overload" bug in arduino IDE 1.8
   */
  void _loop()
  {
    int line_sensors = line_sensor.readSensors();

    switch (line_sensors) {
    case S1_IN_S2_IN:
      forward(power);
      line_dir=10;
      break;

    case S1_IN_S2_OUT:
      forward(power);
      if (line_dir > 1) line_dir--;
      break;

    case S1_OUT_S2_IN:
      forward(power);
      if (line_dir < 20) line_dir++;
      break;

    case S1_OUT_S2_OUT:
      if (line_dir == 10) forward(-power);
      if (line_dir < 10) left(power);
      if (line_dir > 10) right(power);
      break;
    }

    enc_right.updateSpeed();
    enc_left.updateSpeed();
  }

  /**
   * set_motors_pwm: set left and right motors PWM value [0..255]
   */
  void set_motors_pwm(int left, int right)
  {
    enc_right.setMotorPwm(right);
    enc_left.setMotorPwm(left);
  }

  /**
   * forward: go forward
   */
  void forward(int sp)
  {
    set_motors_pwm(sp, -sp);
  }

  /**
   * left: turn on itself on the left
   */
  void left(int sp)
  {
    set_motors_pwm(-sp, -sp);
  }

  /**
   * right: turn on itself on the right
   */
  void right(int sp)
  {
    set_motors_pwm(sp,sp);
  }

  /**
   * power: motors power
   */
  int power = 80;

private:
  int line_dir = 10;

  // Sensors
  MeEncoderOnBoard enc_right;
  MeEncoderOnBoard enc_left;
  MeLineFollower line_sensor;

  LineFollower() :
    enc_right(ENC_RIGHT_PORT),
    enc_left(ENC_LEFT_PORT),
    line_sensor(LINE_SENSOR_PORT)
  {}
  LineFollower(LineFollower const&) = delete;
  void operator=(LineFollower const&) = delete;

  static void isr_process_encoder_right(void) {
    auto& obj = LineFollower::instance();
    if(digitalRead(obj.enc_right.getPortB()) == 0) obj.enc_right.pulsePosMinus();
    else obj.enc_right.pulsePosPlus();
  }

  static void isr_process_encoder_left(void) {
    auto& obj = LineFollower::instance();
    if(digitalRead(obj.enc_left.getPortB()) == 0) obj.enc_left.pulsePosMinus();
    else obj.enc_left.pulsePosPlus();
  }

};

////////////////////////////////////////////////////////////////////////////////
// SETUP

bool running = false;
static LineFollower& line_follower = LineFollower::instance();


void setup()
{
  // Setup the serial conection
  Serial.begin(115200);

  // Setup the line_follower class
  line_follower._setup();
  line_follower.power = POWER;

  // Setup PWM to 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP

void loop()
{
  // Check serial commands
  if (Serial.available()) {
    char ch = Serial.read();
    switch (ch) {
    case '0':
      running = false;
      line_follower.set_motors_pwm(0,0);
      break;
    case '1':
      running = true;
      break;
    default:
      /* Unknown Command! */
      break;
    }
  }

  // If it is running, call the line follower loop method
  if (running)
    line_follower._loop();

}
