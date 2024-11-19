class EntprellterTaster {
  private:
    int pin;
    bool entprellterZustand;
    unsigned long letzteAenderung;   
    unsigned long druckBeginn;
    bool gedruecktGemeldet;
    static const unsigned long entprellZeit = 50; // 50 ms Entprellzeit
    static const unsigned long langeDruckZeit = 1000; // 1 Sekunde für langen Druck

  public:
    EntprellterTaster(int tasterPin)
      : pin(tasterPin), entprellterZustand(false), letzteAenderung(0), druckBeginn(0), gedruecktGemeldet(true) {
      pinMode(pin, INPUT_PULLUP);
    }

    void aktualisiere() {
      bool aktuellerZustand = !digitalRead(pin); // Negation für negative Logik
      unsigned long aktuelleZeit = millis();

      if (aktuellerZustand != entprellterZustand && (aktuelleZeit - letzteAenderung) > entprellZeit) {
        entprellterZustand = aktuellerZustand;
        letzteAenderung = aktuelleZeit;

        if (entprellterZustand) {   
          druckBeginn = aktuelleZeit;
          gedruecktGemeldet = false;
        }
      }
    }

    bool istGedrueckt() {
      return entprellterZustand;
    }

    bool wurdeGedrueckt() {
      if (!entprellterZustand && !gedruecktGemeldet) {
        if (millis() - druckBeginn < langeDruckZeit) {
          gedruecktGemeldet = true;
          return true;
        }
      }
      return false;
    }

    bool wurdeLangeGedrueckt() {
      if (entprellterZustand && !gedruecktGemeldet) {
        if (millis() - druckBeginn >= langeDruckZeit) {
          gedruecktGemeldet = true;
          return true;
        }
      }
      return false;
    }
};

// Testprogramm
const int TASTER_PIN = 2;
EntprellterTaster taster(TASTER_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Taster-Test gestartet");
}

void loop() {
  taster.aktualisiere();

  if (taster.wurdeGedrueckt()) {
    Serial.println("Taster wurde kurz gedrückt");
  }

  if (taster.wurdeLangeGedrueckt()) {
    Serial.println("Taster wurde lange gedrückt");
  }

  
} 
