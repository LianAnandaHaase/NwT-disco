#include <iostream>
using namespace std;

//variabel block, hier wird der pin für den lautsprecher und und die geschwindigkeit festgelegt
const ushort tone_length = 250;
const ushort pin = 12;
const ushort lenght_refrain = 6;
const ushort lenght_strophe = 7;

//enumeration für noten, die gleich mit der frequenz verknüpft sind.
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

//funktion, die die note, als enum note dargestellt, in ihre freauenz umwandelt, und diese zurück gibt
unsigned int cast_enum (note note)
{
    return static_cast<unsigned int>(note);
}

//array mit den lyrics des refrains
const char* lyricsRefrain[lenght_refrain] = { "Oh", "Oh", "Oh", "Oh", "Oh", "Oh" };

//array mit den preisen an der bar
const char* preise[lenght_strophe] = {
    "Bier         6€",
    "Bier         6€",
    "Vodka Lemon  9€",
    "Vodka Lemon  9€",
    "special     11€",
    "special     11€",
    "alle mitsingen!"
};

//array von noten der strophe
note notesStrophe[lenght_strophe] = { E, E, G, E, D, C, B };

//array von noten des refrain
note notesRefrain[lenght_refrain] = { E, E, G, E, D, C };

//funktion, die noten abspielt, am richtigen pin, mit der richtigen frequenz un dauer
void playOnSpeaker (int pin, note f)
{
    cout << "pin:  " << pin << "  Frequenz:  " << cast_enum (f) << endl;
    //delay (duration);
}

void displayText (const char* text)
{
    cout << text << endl;
}

//funktion, die einen array von note abspielen kann, ihr wird das richtige array mithile eines zeigers übergeben.
void play(const note* notes, const char** texte , ushort length)
{
    for (int i = 0; i < length; i++)
    {
        displayText(texte[i]);

        playOnSpeaker(pin, notes[i]);

    }
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

int main()
{
    refrain ();
    strophe ();

    return 0;
}
