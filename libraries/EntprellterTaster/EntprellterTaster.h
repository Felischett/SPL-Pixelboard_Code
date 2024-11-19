#ifndef ENTPRELLTER_TASTER_H
#define ENTPRELLTER_TASTER_H

#include <Arduino.h>

/**
 * @class EntprellterTaster
 * @brief Klasse zur Verwaltung eines entprellten Tasters mit Kurz- und Langdruck-Erkennung.
 */
class EntprellterTaster {
private:
  int pin;
  bool entprellterZustand;
  unsigned long letzteAenderung;
  unsigned long druckBeginn;
  bool gedruecktGemeldet;

  static const unsigned long entprellZeit = 50;       // 50 ms Entprellzeit
  static const unsigned long langeDruckZeit = 600;  // 1 Sekunde für langen Druck

public:
  /**
   * @brief Konstruktor für die Klasse EntprellterTaster.
   * @param tasterPin Der Pin, an dem der Taster angeschlossen ist.
   */
  EntprellterTaster(int tasterPin);

  /**
   * @brief Aktualisiert den Zustand des Tasters.
   */
  void aktualisiere();

  /**
   * @brief Gibt zurück, ob der Taster aktuell gedrückt ist.
   * @return true, wenn der Taster gedrückt ist, sonst false.
   */
  bool istGedrueckt();

  /**
   * @brief Prüft, ob der Taster kurz gedrückt wurde.
   * @return true, wenn der Taster kurz gedrückt wurde, sonst false.
   */
  bool wurdeGedrueckt();

  /**
   * @brief Prüft, ob der Taster lange gedrückt wurde.
   * @return true, wenn der Taster lange gedrückt wurde, sonst false.
   */
  bool wurdeLangeGedrueckt();
};

#endif // ENTPRELLTER_TASTER_H
