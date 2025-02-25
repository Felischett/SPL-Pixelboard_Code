#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Arduino.h"

class Taster {
  private:
    int pin;
    unsigned long entprellZeit;
    bool letzterZustand;
    unsigned long startZeit;
    unsigned long letzteZeit;
    bool entprellterZustand;
    bool gedruecktMeldung;
    bool langeGedruecktMeldung;

  public:
    Taster(int tasterPin, unsigned long entprell = 50);
    void entprellen();
    bool wurdeGedrueckt();
    bool wurdeLangeGedrueckt();
    bool istGedrueckt();
};

class Joystick : public Taster {
  private:
    int xPin;  // Pin für den X-Wert
    int yPin;  // Pin für den Y-Wert

  public:
    Joystick(int tasterPin, int xPin, int yPin, unsigned long entprell = 50);
    int leseX();
    int leseY();
};

#endif
