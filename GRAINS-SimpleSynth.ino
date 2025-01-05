#include <synth.h> // https://github.com/dzlonline/the_synth

synth the_synth;

#define VOICE0 0

#define CV_POT_IN1 A2
#define CV_POT_IN2 A1
#define CV_POT3 A0
#define CV_IN3 A3
#define CV_AUDIO_IN A4

boolean gate = false;
boolean oldgate = false;
byte note = 0;
byte length = 100;
byte wave = SINE;
byte envelope = ENVELOPE0;

void setup() {
  Serial.begin(115200);
  pinMode(CV_POT_IN1, INPUT);
  pinMode(CV_POT_IN2, INPUT);
  pinMode(CV_POT3, INPUT);
  pinMode(CV_IN3, INPUT);
  pinMode(CV_AUDIO_IN, INPUT);

  the_synth.begin();

  // Set up voice 0 with some default values
  the_synth.setupVoice(VOICE0, wave, 60, envelope, length, 64);
}

void loop() {
  // Check for gate on GRAINS input "A" (Arduino Pin A4):
  // "analogRead()" returns values between 0 and 1023.
  // If the value is bigger than 512, this is considered as "gate on"
  gate = analogRead(CV_AUDIO_IN) > 512;

  // React only if the state of input "A" changed
  if (oldgate != gate) {
    if (gate) {
      Serial.print("note on -> ");
      // FIXME: workaround for a problem with my Arturia Keystep
      delay(5);

      // Set waveform, envelope and length
      length = analogRead(CV_POT_IN2) / 8; // length can be 0-128

      // SINE=0, TRIANGLE=1, SQUARE=2, SAW=3, RAMP=4, NOISE=5
      wave = analogRead(CV_POT_IN1) / 200;

      // ENVELOPE0 = 0, ENVELOPE1 = 1, ENVELOPE2 = 2, ENVELOPE3 = 3
      // envelope = map(analogRead(CV_POT3), 0, 1023, 0, 3);
      envelope = analogRead(CV_POT3) / 340;

      // Read the voltage value (0 - 1023) for the pitch
      int sensorValue = analogRead(CV_IN3);

      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.8V)
      float voltage = sensorValue * (3.8 / 1023.0);

      // Convert the voltage to a MIDI note value between 36 and 82
      //   0.0V = C2 (Midi Note 36)
      //   3.8V = A#5/Bb5 (Midi Note 82)
      // http://www.phys.unsw.edu.au/jw/notes.html
      note = map(voltage * 1000, 0, 3800, 36, 82);

      // NOTE: There seems to be a problem with the tracking of
      //       notes depending on whether GRAINS is connected to
      //       USB or not. Tracking is correct, when USB is
      //       connected. But then you hear the "USB whining" noise

      the_synth.setPitch(VOICE0, note);
      the_synth.setLength(VOICE0, length);
      the_synth.setWave(VOICE0, wave);
      the_synth.setEnvelope(VOICE0, envelope);
      the_synth.trigger(VOICE0);

/*
      // Debug Output
      Serial.print("Voltage: ");
      Serial.print(voltage);
      Serial.print(" Note: ");
      Serial.print(note);
      Serial.print(" Length: ");
      Serial.print(length);
      Serial.print(" Wave: ");
      Serial.print(wave);
      Serial.print(" Envelope: ");
      Serial.println(envelope);
*/
    } else {
      Serial.println("note off");
      // TODO: stop sound when length is set to 127?
    }

    oldgate = gate;
  }
}
