#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>  // Stelle sicher, dass diese Datei im Projekt vorhanden ist

// LED-Konfiguration
#define LED_PIN        32
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  8
#define MATRIX_TYPE    VERTICAL_ZIGZAG_MATRIX

// Joystick-Pins
#define JOY_X_PIN      34
#define JOY_Y_PIN      35

// Matrix und LED-Zugriff
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> matrix;
CRGB* leds = matrix[0];

// Bewegungsrichtung
enum Richtung { OBEN, UNTEN, LINKS, RECHTS };
Richtung aktuelleRichtung = RECHTS;

// Snake-Daten
const int maxLaenge = MATRIX_WIDTH * MATRIX_HEIGHT;
int snakeX[maxLaenge];
int snakeY[maxLaenge];
int snakeLaenge = 3;

// Futterposition
int futterX = 5;
int futterY = 3;

// Joystick-Schwellenwerte
const int schwelleOben  = 1000;
const int schwelleUnten = 3000;

/**
 * Initialisiert LEDs, Snake-Startposition und erstes Futter
 */
void setup()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, matrix.Size());
  FastLED.setBrightness(64);
  randomSeed(analogRead(JOY_X_PIN));

  // Startposition der Snake
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 2; snakeY[1] = 4;
  snakeX[2] = 1; snakeY[2] = 4;

  futterX = random(0, MATRIX_WIDTH);
  futterY = random(0, MATRIX_HEIGHT);
}

/**
 * Liest den Joystick aus und ändert die Richtung
 */
void leseJoystick()
{
  int xWert = analogRead(JOY_X_PIN);
  int yWert = analogRead(JOY_Y_PIN);

  if (xWert < schwelleOben && aktuelleRichtung != RECHTS) {
    aktuelleRrichtung = LINKS;
  } else if (xWert > schwelleUnten && aktuelleRichtung != LINKS) {
    aktuelleRichtung = RECHTS;
  } else if (yWert < schwelleOben && aktuelleRichtung != UNTEN) {
    aktuelleRichtung = OBEN;
  } else if (yWert > schwelleUnten && aktuelleRichtung != OBEN) {
    aktuelleRichtung = UNTEN;
  }
}

/**
 * Bewegt die Snake einen Schritt weiter
 * Gibt false zurück bei Kollision
 */
bool bewegeSchlange()
{
  for (int i = snakeLaenge - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  switch (aktuelleRichtung) {
    case LINKS:  snakeX[0]--; break;
    case RECHTS: snakeX[0]++; break;
    case OBEN:   snakeY[0]--; break;
    case UNTEN:  snakeY[0]++; break;
  }

  // Kollision mit Rand
  if (snakeX[0] < 0 || snakeX[0] >= MATRIX_WIDTH ||
      snakeY[0] < 0 || snakeY[0] >= MATRIX_HEIGHT) {
    return false;
  }

  // Kollision mit sich selbst
  for (int i = 1; i < snakeLaenge; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return false;
    }
  }

  // Futter gegessen
  if (snakeX[0] == futterX && snakeY[0] == futterY) {
    if (snakeLaenge < maxLaenge) {
      snakeLaenge++;
    }

    // Neues Futter generieren (nicht auf Snake)
    bool gültig = false;
    while (!gültig) {
      futterX = random(0, MATRIX_WIDTH);
      futterY = random(0, MATRIX_HEIGHT);
      gültig = true;
      for (int i = 0; i < snakeLaenge; i++) {
        if (snakeX[i] == futterX && snakeY[i] == futterY) {
          gültig = false;
          break;
        }
      }
    }
  }

  return true;
}

/**
 * Zeichnet Snake und Futter
 */
void zeichneSpiel()
{
  FastLED.clear();

  for (int i = 0; i < snakeLaenge; i++) {
    leds[matrix.mXY(snakeX[i], snakeY[i])] = CRGB::Green;
  }

  leds[matrix.mXY(futterX, futterY)] = CRGB::Red;

  FastLED.show();
}

/**
 * Spiel zurücksetzen nach Game Over
 */
void spielZuruecksetzen()
{
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < matrix.Size(); j++) {
      leds[j] = CRGB::Red;
    }
    FastLED.show();
    delay(200);
    FastLED.clear();
    FastLED.show();
    delay(200);
  }

  // Snake neu starten
  snakeLaenge = 3;
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 2; snakeY[1] = 4;
  snakeX[2] = 1; snakeY[2] = 4;
  aktuelleRichtung = RECHTS;

  futterX = random(0, MATRIX_WIDTH);
  futterY = random(0, MATRIX_HEIGHT);
}

/**
 * Hauptspiel-Loop
 */
void loop()
{
  leseJoystick();

  if (!bewegeSchlange()) {
    spielZuruecksetzen();
  }

  zeichneSpiel();
  delay(200);
}
