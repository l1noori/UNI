// Definitie van klasse Steen.

#ifndef SteenHVar  // om te voorkomen dat dit .h bestand meerdere keren
#define SteenHVar  // wordt ge-include 

#include <fstream>
#include <vector>
#include "constantes.h"

class Steen
{ public:
    // constructor
    Steen ();    
    Steen operator=(Steen& original);

    // Lees zo mogelijk een steen in vanuit fin.
    // Pre:
    // * fin bevat achtereenvolgens:
    //   - een regel met twee integers m0 en n0, gescheiden door een spatie
    //   - m0 regels met n0 karakters . of X
    // Post:
    // * als m0<=MaxDim en n0<=MaxDim is de ingelezen steen opgeslagen in
    //   array inhoud
    // Retourneer:
    // * true, als 0<=m0<=MaxDim en 0<=n0<=MaxDim
    // * false, anders
    bool leesIn (std::ifstream &fin);

    // Wrapper functie voor alle functies die orientaties
    // van de ingelezen steen construeert
    //
    // Post:
    // * inhoud[][][] bevat nu alle orientaties voor elke steen
    void maakAlleRotaties();

    // Geef waarde van m.
    int getM ()
    {
      return m;

    };  // getM

    // Geef waarde van n.
    int getN ()
    {
      return n;

    };  // getN

    // Geeft terug de inhoud van 
    int getInhoud(int i, int j,int k){
      return inhoud[k][i][j];
    }

    // Is vakje (i,j) van de steen gevuld?
    bool gevuld (int i, int j);

    // Is vakje (i,j) van orientatie k gevuld?
    // (Houd rekening met symmetrie)
    bool gevuldOrientatie(int i, int j, int k);

    // Geef het aantal vakjes dat de steen bedekt.
    int grootte ();

    // Druk steen af op het scherm.
    void drukAf ();

    // Druk steen af met rotatie
    bool drukAfRotatie(int rotatie);

    // Returnt true als de steen1 en steen 2 hetzelfde zijn
    // Returnt false als ze dat niet zijn
    bool dezelfdeSteen(int steen1, int steen2);

    // Gaat inhoud[][][] af en bepaald voor elke steen of 
    // er gelijke orientaties zijn
    // Post:
    // * Op plek inhoud[orientatie][0][0] staat met een 
    //   char een verwijzing naar een gelijke orientatie
    //    die eerder is gevonden
    void vindDubbelen();
    
    // Roteer steen een aantal keer 90 graden met de klok mee.
    void roteer (int aantalkeer);

    // Spiegel steen in horizontale as.
    void spiegelHorizontaleAs ();

    // Spiegel steen in verticale as.
    void spiegelVerticaleAs ();

    // Met inversen vinden we alle mogelijke rotaties
    void vindRotaties();

    // geeft terug of de steen al is gebruikt
    bool getGebruikt ();

    // Zet de variabele gebruikt op true
    void gebruikSteen();

  private:
    char inhoud[8][MaxDimSteen][MaxDimSteen];
      // Bovenste rij is rij 0, onderste rij is rij MaxDim-1.
      // Meest linker kolom is kolom 0, meest rechter kolom is kolom MaxDim-1.
    int m, n;  // Feitelijk aantal rijen (vanaf boven) / kolommen (vanaf links)
               // van steen dat in gebruik (deels gevuld) is.
               // Deze waardes worden gezet bij het inlezen van een steen.
    int huidigeRotatie; 
    bool gebruikt; // true als de steen al is gelegd
    int vakjes = 0;   

};

#endif

