#include "EntprellterTaster.h"

EntprellterTaster::EntprellterTaster(int tasterPin)
  : pin(tasterPin),
    entprellterZustand(false),
    letzteAenderung(0),
    druckBeginn(0),
    gedruecktGemeldet(true) {
  pinMode(pin, INPUT_PULLUP);
}

void EntprellterTaster::aktualisiere() {
  bool aktuellerZustand = !digitalRead(pin); // Negation für negative Logik
  unsigned long aktuelleZeit = millis();

  if (aktuellerZustand != entprellterZustand && 
      (aktuelleZeit - letzteAenderung) > entprellZeit) {
    entprellterZustand = aktuellerZustand;
    letzteAenderung = aktuelleZeit;

    if (entprellterZustand) {
      druckBeginn = aktuelleZeit;
      gedruecktGemeldet = false;
    }
  }
}

bool EntprellterTaster::istGedrueckt() {
  return entprellterZustand;
}

bool EntprellterTaster::wurdeGedrueckt() {
  if (!entprellterZustand && !gedruecktGemeldet) {
    if (millis() - druckBeginn < langeDruckZeit) {
      gedruecktGemeldet = true;
      return true;
    }
  }
  return false;
}

bool EntprellterTaster::wurdeLangeGedrueckt() {
  if (entprellterZustand && !gedruecktGemeldet) {
    if (millis() - druckBeginn >= langeDruckZeit) {
      gedruecktGemeldet = true;
      return true;
    }
  }
  return false;
}
