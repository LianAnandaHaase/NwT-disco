//includirungen für lcd display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//enumeration für noten, die gleich mit der jeweiligen frequenz verknüpft sind.
enum note {
    a = 220,
    h = 247,
    C = 262,  // gerundet von 261.63
    D = 294,  // 293.66
    E = 330,  // 329.63
    F = 349,
    G = 392,
    A = 440,
    B = 466,
    H = 494  // 493.88
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
const unsigned short pause = 10; //kurze lücke zwisch noten, um kurzt geiche hinter einander von langen zu unterscheiden.
const unsigned short pin = 12; //pin an den der lautsprecher angeschlossen ist
const unsigned short blau = 7; //pins an die die rgb angeschlossen ist
const unsigned short grun = 6;
const unsigned short rot = 5;
const unsigned short pin_sync = 10;
const unsigned short length_refrain = 30;
const unsigned short length_strophe = 35;
int takt_position = 24;// Zählt mit, wo im Takt wir uns befinden für Signal. Der Auftakt ist eine Viertelnote, das sind 8 32tel 


//Deklarationen von allen funktionen, dann kann man die Funktionen unten in beliebiger Reihenfolge schreiben
void strophe ();
void refrain ();
void play(const note* notes, const char** texte , unsigned short length);
void displayText (const char* text);
void playOnSpeaker (int pin, note n);
char* substr(char* arr, int begin, int len);
void sync(int tone_length);
void rgb(bool input);

// Set the LCD address to 0x27 (or 0x3f, etc.) for a 16x2 LCD. Check your LCD module's documentation.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address and dimensions if needed

//array mit den lyrics des refrains
const char* lyricsRefrain[length_refrain] = {
  "Soon", "Soon may", "Soon may the", "Wel", "Weller","Wellerman",
  "Wellerman come", "to", "to bring", "to bring us",         
  "to bring us     sug", "to bring us     sugar", "and", "and tea", "and tea and", 
  "and tea and rum", "One", "One day", "One day when", 
  "One day when the", "ton", "tonguin'", "tonguin'is", "tonguin'is done",
  "We'll", "We'll take", "We'll take our", "We'll take our  leave", 
  "and", "and go"
};

//array mit den lyrics der strophe
const char* lyricsStrophe[length_strophe] = {
   "There", "There once", "There once was", "There once was a", "There once was aship",       
   "that", "that put", "that put to", "that put to sea",  
   "The", "The name", "The name of", "The name of the the", "The name of the ship",                
   "was", "was the", "was the Bil", "was the Billy", "was the Billy o'", "was the Billy o'Tea",         
   "The", "The winds", "The winds blew",  "The winds blew  up",
   "her", "her bow", "her bow dipped", "her bow dipped  down",
   "O", "O blow", "O blow my", "O blow my bul", "Oh blow my bully",
   "boys", "boys blow"
};

//array mit den tönen der strophe
note_hoehe_laenge notesStrophe[length_strophe] = {
  {E, _4}, {a, _4}, {a, _8}, {a, _8}, {a, _4},           // There once was a ship
  {C, _4}, {E, _4}, {E, _4}, {E, _4p},                   // that put to sea

  {E, _8}, {F, _4}, {D, _8}, {D, _8}, {D, _4},           // The name of the ship
  {D, _8}, {F, _8}, {A, _8}, {A, _8}, {E, _4}, {E, _4p}, // was the Billy o' Tea

  {E, _8}, {a, _4}, {a, _4}, {a, _4},                    // The winds blew up
  {C, _4}, {E, _4}, {E, _4}, {E, _4},                    // her bow dipped down

  {E, _4}, {E, _4}, {D, _4}, {C, _8}, {C, _8},           // O blow my bully
  {h, _4}, {a, _1},                                      // boys blow
};

//array von noten des refrain
note_hoehe_laenge notesRefrain[length_refrain] = {
  {A, _2}, {A, _4p}, {F, _8}, {G, _8}, {G, _8},    // Soon may the Weller-
  {E, _4}, {E, _4p},                               // -man come

  {E, _8}, {F, _4}, {D, _4}, {D, _8}, {E, _8},     // To bring us sugar
  {F, _4}, {A, _4}, {E, _4}, {E, _2},              // and tea and rum

  {A, _2}, {A, _4}, {F, _8}, {F, _8},              // One day when the
  {G, _8}, {G, _8}, {E, _4}, {E, _4},              // tonguin'is done

  {E, _4}, {E, _4}, {D, _4},                       // We'll take our 
  {C, _4}, {h, _4}, {a, _2p}                       // leave and go
};

void setup() {
  pinMode(pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pin_sync, OUTPUT);
  pinMode(blau, OUTPUT);
  pinMode(grun, OUTPUT);
  pinMode(rot, OUTPUT);
  //Serial.begin(9600);

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
        noTone(pin);
        delay(pause);

        //Serial.println(texte[i]);
        sync(tones[i].length);

    }
}

void sync(int tone_length)
{
    takt_position += tone_length;
    if(takt_position == 32)//voller Takt sind 32/32
    {
       takt_position = 0;
    }

    if(takt_position == 0) 
    {
       digitalWrite(pin_sync, HIGH);
       digitalWrite(LED_BUILTIN, HIGH);
       //Serial.println("HIGH");
       rgb (true);
    }
    else 
    {
        digitalWrite(pin_sync, LOW);
        digitalWrite(LED_BUILTIN, LOW);
        //Serial.println("LOW");
        rgb(false);
    }
 }

 void rgb(bool input)
 {
    if(input == true)
    {
      analogWrite(blau, 200);
      analogWrite(grun, 0);
      analogWrite(rot , 250);
    }
    else 
    {
      analogWrite(blau, 0);
      analogWrite(grun, 0);
      analogWrite(rot, 0);
    };
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
    char* res = new char[len + 1];//Um eins länger wegen Null-Termination
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;//Null-Termination
    return res;
}

//funktion, die noten abspielt, am richtigen pin, mit der richtigen frequenz
void playOnSpeaker (note n)
{
    int f = n; //umwandlung in integer ist automatisch
    tone (pin, f);
}
