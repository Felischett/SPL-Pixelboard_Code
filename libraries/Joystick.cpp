#include "Joystick.h"

// Konstruktor für Taster
Taster::Taster(int tasterPin, unsigned long entprell) {
  pin = tasterPin;
  entprellZeit = entprell;
  letzterZustand = LOW;
  letzteZeit = 0;
  entprellterZustand = false;
  gedruecktMeldung = false;
  langeGedruecktMeldung = false;
  pinMode(pin, INPUT_PULLUP); // Pin als Input mit Pull-up setzen
}

// Entprellen des Tasters
void Taster::entprellen() {
  entprellterZustand = !digitalRead(pin); // Invertieren, da Pull-up verwendet wird

  // Prüfen, ob sich der Zustand geändert hat und ob die Entprellzeit überschritten ist
  if (entprellterZustand != letzterZustand && (millis() - letzteZeit) > entprellZeit) {
    letzteZeit = millis(); // letzte entprellte Zeit speichern

    // Wenn der Taster gedrückt wurde (Zustand HIGH)
    if (entprellterZustand == HIGH) {
      startZeit = millis(); // Startzeit des Drückens speichern
      gedruecktMeldung = false; // Rücksetzen für "wurdeGedrueckt"
      langeGedruecktMeldung = false; // Rücksetzen für "wurdeLangeGedrueckt"
    } else {
      // Wenn der Taster losgelassen wurde
      unsigned long gedruecktZeit = millis() - startZeit;

      if (gedruecktZeit >= 3000) {
        Serial.println("Taster lange gedrückt");
      } else {
        Serial.println("Taster kurz gedrückt");
      }
    }
  }
  letzterZustand = entprellterZustand;
}

// Prüfen, ob der Taster kurz gedrückt wurde
bool Taster::wurdeGedrueckt() {
  if (!entprellterZustand && !gedruecktMeldung) {
    unsigned long gedruecktZeit = millis() - startZeit;
    if (gedruecktZeit < 1000) {
      gedruecktMeldung = true; // Sicherstellen, dass diese Methode erst nach dem Loslassen TRUE liefert
      return true;
    }
  }
  return false;
}

// Prüfen, ob der Taster lange gedrückt wurde
bool Taster::wurdeLangeGedrueckt() {
  if (!entprellterZustand && !langeGedruecktMeldung) {
    unsigned long gedruecktZeit = millis() - startZeit;
    if (gedruecktZeit >= 1000) {
      langeGedruecktMeldung = true; // Sicherstellen, dass diese Methode erst nach dem Loslassen TRUE liefert
      return true;
    }
  }
  return false;
}

// Gibt den aktuellen Zustand des Tasters zurück
bool Taster::istGedrueckt() {
  return entprellterZustand;
}

// Konstruktor für Joystick
Joystick::Joystick(int tasterPin, int xPin, int yPin, unsigned long entprell) 
  : Taster(tasterPin, entprell), xPin(xPin), yPin(yPin) {}

// Methode, um den aktuellen X-Wert zu lesen
int Joystick::leseX() {
  return analogRead(xPin);  // Liest den analogen Wert des X-Pins
}

// Methode, um den aktuellen Y-Wert zu lesen
int Joystick::leseY() {
  return analogRead(yPin);  // Liest den analogen Wert des Y-Pins
}
