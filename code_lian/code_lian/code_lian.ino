//includirungen für lcd display
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

//enumeration für ton längen
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

//typ für töne mit bestimter frequenz und und länge
struct note_hoehe_laenge {
    note tonhoehe;
    tone_length length;
};

//globale Konstanten, hier wird der pin für den lautsprecher und und die geschwindigkeit festgelegt
const unsigned short tone_length = 70;
const unsigned short pause = 20; //kurze lücke zwisch noten, um kurzt geiche hinter einander von langen zu unterscheiden.
const unsigned short pin = 12; //pin an den der lautsprecher angeschlossen ist
const unsigned short length_refrain = 30;
const unsigned short length_strophe = 35;

//Deklarationen von allen funktionen, dann kann man die Funktionen unten in beliebiger Reihenfolge schreiben
void strophe ();
void refrain ();
void play(const note* notes, const char** texte , unsigned short length);
void displayText (const char* text);
void playOnSpeaker (int pin, note n);
char* substr(char* arr, int begin, int len);

// Set the LCD address to 0x27 (or 0x3f, etc.) for a 16x2 LCD. Check your LCD module's documentation.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and dimensions if needed

//array mit den lyrics des refrains
const char* lyricsRefrain[length_refrain] = {
  "Soon", "may", "the", "Wel", "ler","man",   // Soon may the Wellerman
  "come", "to", "bring", "us",         // come to bring us
  "sugar", "and", "tea", "and",        // sugar and tea and
  "rum", "One", "day", "when",         // rum. One day when
  "the", "ton", "guin'", "is", "done",     // the tonguin' is done
  "We'll", "take", "our", "leave",     // We'll take our leave
  "and", "go"                          // and go
};

//array mit den lyrics der strophe
const char* lyricsStrophe[length_strophe] = {
  "There", "once", "was", "a",           // 4
  "ship", "that", "put", "to",           // 8
  "sea", "And", "the", "name",           // 12
  "of", "that", "ship", "was",           // 16
  "the", "Bil", "ly", "o'",              // 20
  "Tea", "The", "winds", "blew",         // 24
  "up", "her", "bow", "dipped",          // 28
  "down", "O", "blow", "my",             // 32
  "bul", "ly", "blow"                    // 35
};

//array mit den tönen der strophe
note_hoehe_laenge notesStrophe[length_strophe] = {
  {E, _8}, {F, _8}, {G, _8}, {E, _8},     // There once was a ship
  {F, _8}, {G, _8}, {A, _8}, {G, _8},     // that put to sea

  {A, _8}, {G, _8}, {F, _8}, {E, _8},     // And the name of that ship
  {F, _8}, {E, _8}, {D, _8}, {E, _8},     // was the Billy o' Tea

  {F, _8}, {G, _8}, {A, _8}, {A, _8},     // The winds blew up
  {G, _8}, {F, _8}, {E, _8}, {D, _8},     // her bow dipped down

  {E, _8}, {F, _8}, {G, _8}, {G, _8},     // O blow my bully
  {F, _8}, {E, _8}, {D, _4}               // boys blow (huh!)
};

//array von noten des refrain
note_hoehe_laenge notesRefrain[length_refrain] = {
  {E, _8}, {E, _8}, {F, _8}, {G, _8},    // Soon may the Weller-
  {G, _8}, {F, _8}, {E, _8}, {D, _8},    // -man come

  {E, _8}, {E, _8}, {F, _8}, {G, _8},    // To bring us sugar
  {G, _8}, {F, _8}, {E, _8}, {D, _8},    // and tea and rum

  {E, _8}, {F, _8}, {G, _8}, {A, _4},    // One day when the
  {G, _8}, {F, _8}, {E, _8},             // tonguin's done

  {D, _8}, {E, _8}, {F, _8}, {G, _4},    // We'll take our
  {F, _8}, {E, _8}, {D, _4}              // leave and go
};

void setup() {
  pinMode(pin, OUTPUT);

  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
}

void loop() {
    strophe ();
    refrain ();
}

//spielt den refrain ab.
void refrain ()
{
    play(notesRefrain, lyricsRefrain, length_refrain);
}

//spielt die strophe ab.
void strophe ()
{
    play(notesStrophe, lyricsStrophe, length_strophe);
}

//funktion, die einen array von noten abspielt
void play(const note_hoehe_laenge* tones, const char** texte , unsigned short length)
{
    for (int i = 0; i < length; i++)
    {
        displayText(texte[i]);

        playOnSpeaker (tones[i].tonhoehe );

        delay (tone_length * tones[i].length - pause);
        tone(pin, 0);
        delay(pause);

    }
}

//funktion, die text auf dem display anzeigt
void displayText (const char* text)
{
    lcd.clear();
    lcd.setCursor(0, 0); //setzt den cursor nach oben links
    
    int laenge = strlen (text); //speichert die länge des ausugebennen text als variable

    if(laenge > 32)  //falls mehr als 32 zeichen eingegeben werden solten, wird das nach der 32 stelle ignoriert.
        laenge = 32;
    
    if (laenge <= 16){  //fals weniger als 16 zeichen eingegeben werden, können diese direkt ohne zeilenumbruch angezeigt werden.
        lcd.print(text);
    }
    else {              //fals es aber mehr als 16 seinen solten, muss er nach der 16 stelle getrennt werden, und der teil danach in der nächsten zeile angezeigt werden
        const char* text_1 = substr(text, 0, 16);            //die ersten 16 zeichen werden als eigene zeichenkette abgespeichert
        const char* text_2 = substr(text, 16, laenge - 16);  //die zeichen nach der 16. setelle werden in einem anderen char array gespeichert, um diese dann in der zweiten zeile anzuzeigen
    
        lcd.print(text_1);   //schreibt den ersten teil in die erste zeile
        lcd.setCursor(0, 1); 
        lcd.print(text_2);
    }
 
 }

//funktion, die einen teil der zeichenkette extrahiert
char* substr(const char* arr, int begin, int len)
{
    char* res = new char[len + 1];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

//funktion, die noten abspielt, am richtigen pin, mit der richtigen frequenz
void playOnSpeaker (note n)
{
    int f = n; //umwandlung in integer ist automatisch
    tone (pin, f);
}
