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

//globale Konstanten, hier wird der pin für den lautsprecher und und die geschwindigkeit festgelegt
const unsigned short tone_length = 250;
const unsigned short pin = 12;
const unsigned short lenght_refrain = 6;
const unsigned short lenght_strophe = 7;

//Deklarationen von allen funktionen
void strophe ();
void refrain ();
void play(const note* notes, const char** texte , unsigned short length);
void displayText (const char* text);
void playOnSpeaker (int pin, note n);

// Set the LCD address to 0x27 (or 0x3f, etc.) for a 16x2 LCD. Check your LCD module's documentation.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and dimensions if needed

//array mit den lyrics des refrains
const char* lyricsRefrain[lenght_refrain] = { "Oh", "Oh Oh", "Oh Oh Oh", "Oh Oh Oh Oh", "Oh Oh Oh Oh Oh", "Oh Oh Oh Oh Oh" };

//array mit den preisen an der bar
const char* preise[lenght_strophe] = {
    "Bier         6$",
    "Bier         6$",
    "Vodka Lemon  9$",
    "Vodka Lemon  9$",
    "special     11$",
    "special     11$",
    "alle mitsingen!"
};

//array von noten der strophe
note notesStrophe[lenght_strophe] = { E, E, G, E, D, C, B };

//array von noten des refrain
note notesRefrain[lenght_refrain] = { E, E, G, E, D, C };



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
void play(const note* notes, const char** texte , unsigned short length)
{
    for (int i = 0; i < length; i++)
    {
        displayText(texte[i]);

        playOnSpeaker (pin, notes[i]);

        delay (tone_length);

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
void playOnSpeaker (int pin, note n)
{
    int f = n; //umwandlung in integer ist automatisch
    tone (pin, f);
}
