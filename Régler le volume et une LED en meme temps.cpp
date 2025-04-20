#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

const int potPin = A0;
const int ledPin = 9;
SoftwareSerial mySerial(10, 11); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

int previousVolume = -1;

void setup() {
  pinMode(ledPin, OUTPUT);

  mySerial.begin(9600);
  Serial.begin(115200);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Erreur DFPlayer Mini. Vérifie les connexions.");
    while (true);
  }

  myDFPlayer.volume(15); // Volume initial
  myDFPlayer.play(1);    // Joue la première piste
}

void loop() {
  int potValue = analogRead(potPin);

  // LED: mappe en 0-255 pour PWM
  int ledValue = map(potValue, 0, 1023, 0, 255);
  analogWrite(ledPin, ledValue);

  // Volume: mappe en 0-30 pour DFPlayer
  int volume = map(potValue, 0, 1023, 0, 30);

  // Met à jour le volume seulement si c’est différent (évite surcharge série)
  if (volume != previousVolume) {
    myDFPlayer.volume(volume);
    previousVolume = volume;
  }

  // Pas de delay = réaction immédiate
}
