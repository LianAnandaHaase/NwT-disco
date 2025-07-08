#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//enumeration für noten, die gleich mit der jeweiligen frequenz verknüpft sind.
enum note {
    C = 262,  // gerundet von 261.63
    D = 294,  // 293.66
    E = 330,  // 329.63
    F = 349,
    G = 392,
    A = 440,
    B = 466,
    H = 494   // 493.88
};

enum tone_length {
    _32  = 1,
    _16  = 2,
    _16p = 3,
    _8   = 4,
    _8p  = 6,
    _4   = 8,
    _4p  = 12,
    _2   = 16,
    _2p  = 24,
    _1   = 32
};

struct note_hoehe_laenge {
    note tonhoehe;
    tone_length length;
};

//globale Konstanten, hier wird der pin für den lautsprecher und und die geschwindigkeit festgelegt
const unsigned short tone_length = 70;
const unsigned short pin = 12;
const unsigned short lenght_refrain = 5;
const unsigned short lenght_strophe = 6;

//Deklarationen von allen funktionen, dann kann man die Funktionen unten in beliebiger Reihenfolge schreiben
void strophe ();
void refrain ();
void play(const note* notes, const char** texte , unsigned short length);
void displayText (const char* text);
void playOnSpeaker (int pin, note n);

// Set the LCD address to 0x27 (or 0x3f, etc.) for a 16x2 LCD. Check your LCD module's documentation.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and dimensions if needed

//array mit den lyrics des refrains
const char* lyricsRefrain[lenght_refrain] = { "Oh", "Oh Oh", "Oh Oh Oh", "Oh Oh Oh Oh", "Oh Oh Oh Oh Oh"};

//array mit den preisen an der bar
const char* preise[lenght_strophe] = {
    "Bier         6$",
    "Cola         6$",
    "Vodka Lemon  9$",
    "Gin tonic    9$",
    "Fanta       11$",
    "Sprite      11$"
};

//array von noten der strophe
note_hoehe_laenge notesStrophe[lenght_strophe] = { {E, _4}, {G, _8}, {E, _4}, {D, _4}, {C, _4}, {B, _8} };

//array von noten des refrain
note_hoehe_laenge notesRefrain[lenght_refrain] = { {E, _4},  {G, _4}, {E, _8},{D, _4} , {C, _4} };



void setup() {
  // put your setup code here, to run once:
  pinMode(pin, OUTPUT);

  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
    refrain ();
    strophe ();
}

//spielt den refrain ab.
void refrain ()
{
    play(notesRefrain, lyricsRefrain, lenght_refrain);
}

//spielt die strophe ab.
void strophe ()
{
    play(notesStrophe, preise, lenght_strophe);
}

//funktion, die einen array von note abspielen kann, ihr wird das richtige array mithile eines zeigers übergeben.
void play(const note_hoehe_laenge* tones, const char** texte , unsigned short length)
{
    for (int i = 0; i < length; i++)
    {
        displayText(texte[i]);

        playOnSpeaker (tones[i].tonhoehe );

        delay (tone_length * tones[i].length);

    }
}

//funktion, die text auf dem display anzeigt
void displayText (const char* text)
{
  lcd.clear();
  // Set the cursor to the first column and first row
  lcd.setCursor(0, 0);
  
  lcd.print(text);
}

//funktion, die noten abspielt, am richtigen pin, mit der richtigen frequenz
void playOnSpeaker (note n)
{
    int f = n; //umwandlung in integer ist automatisch
    tone (pin, f);
}
