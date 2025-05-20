#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

// LED-Konfiguration
#define LED_PIN        32
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B
#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  8
#define MATRIX_TYPE    VERTICAL_ZIGZAG_MATRIX

// Joystick
#define JOY_X_PIN      34
#define JOY_Y_PIN      35

// Matrix-Objekt und LED-Array
cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> matrix;
CRGB* leds = matrix[0];

// Richtung
enum Richtung { OBEN, UNTEN, LINKS, RECHTS };
Richtung aktuelleRichtung = RECHTS;

// Snake-Variablen
const int maxLaenge = MATRIX_WIDTH * MATRIX_HEIGHT;
int snakeX[maxLaenge];
int snakeY[maxLaenge];
int snakeLaenge = 3;

// Futter
int futterX = 5;
int futterY = 3;

// Joystick-Schwellenwerte
const int schwelleOben  = 1000;
const int schwelleUnten = 3000;

/**
 * Initialisiert Snake, Matrix und Futter
 */
void setup()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, matrix.Size());
  FastLED.setBrightness(64);
  randomSeed(analogRead(JOY_X_PIN));

  // Snake Startposition
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 2; snakeY[1] = 4;
  snakeX[2] = 1; snakeY[2] = 4;

  futterX = random(0, MATRIX_WIDTH);
  futterY = random(0, MATRIX_HEIGHT);
}

/**
 * Liest Joystick und ändert Richtung
 */
void leseJoystick()
{
  int xWert = analogRead(JOY_X_PIN);
  int yWert = analogRead(JOY_Y_PIN);

  if (xWert < schwelleOben && aktuelleRichtung != RECHTS) {
    aktuelleRichtung = LINKS;
  } else if (xWert > schwelleUnten && aktuelleRichtung != LINKS) {
    aktuelleRichtung = RECHTS;
  } else if (yWert < schwelleOben && aktuelleRichtung != UNTEN) {
    aktuelleRichtung = OBEN;
  } else if (yWert > schwelleUnten && aktuelleRichtung != OBEN) {
    aktuelleRichtung = UNTEN;
  }
}

/**
 * Bewegt Snake und prüft Kollisionen
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

  // Wandkollision
  if (snakeX[0] < 0 || snakeX[0] >= MATRIX_WIDTH ||
      snakeY[0] < 0 || snakeY[0] >= MATRIX_HEIGHT) {
    return false;
  }

  // Eigenkollision
  for (int i = 1; i < snakeLaenge; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return false;
    }
  }

  // Futter fressen
  if (snakeX[0] == futterX && snakeY[0] == futterY) {
    if (snakeLaenge < maxLaenge) {
      snakeLaenge++;
    }

    // Neues Futter suchen
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
 * Universelle Methode: Zeichnet ein Koordinatensystem
 * X-Achse unten (y = MATRIX_HEIGHT - 1), Y-Achse links (x = 0)
 * Ursprung (0, MATRIX_HEIGHT - 1) wird weiß hervorgehoben
 */
void zeichneKoordinatensystem(CRGB* ledsArray,
                               cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE>& matrixObj)
{
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    ledsArray[matrixObj.mXY(x, MATRIX_HEIGHT - 1)] = CRGB::Blue;
  }

  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    ledsArray[matrixObj.mXY(0, y)] = CRGB::Blue;
  }

  ledsArray[matrixObj.mXY(0, MATRIX_HEIGHT - 1)] = CRGB::White;
}

/**
 * Zeichnet Snake, Futter und Koordinatensystem
 */
void zeichneSpiel()
{
  FastLED.clear();

  for (int i = 0; i < snakeLaenge; i++) {
    leds[matrix.mXY(snakeX[i], snakeY[i])] = CRGB::Green;
  }

  leds[matrix.mXY(futterX, futterY)] = CRGB::Red;

  zeichneKoordinatensystem(leds, matrix);

  FastLED.show();
}

/**
 * Zeigt Game Over und startet neu
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

  // Snake resetten
  snakeLaenge = 3;
  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 2; snakeY[1] = 4;
  snakeX[2] = 1; snakeY[2] = 4;
  aktuelleRichtung = RECHTS;
  futterX = random(0, MATRIX_WIDTH);
  futterY = random(0, MATRIX_HEIGHT);
}

/**
 * Hauptloop
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
