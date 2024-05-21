/*
* authors: N.I. van Laarhoven (s3323854) and J. H Liu (s2723468)
* last edited: 13/05/2024
*/

// Definitie van klasse Stand.

#ifndef StandHVar  // om te voorkomen dat dit .h bestand meerdere keren
#define StandHVar  // wordt ge-include 

#include <fstream>
#include "constantes.h"
#include "steen.h"
#include <vector>
#include <array>
#include <set>

class Stand
{ public:
    // default constructor
    Stand ();

    // constructor voor een bord met nwHoogte rijen en nwBreedte kolommen
    Stand (int nwHoogte, int nwBreedte);

    // destructor
    ~Stand ();

    Stand operator=(Stand& original);

    bool operator==(Stand& stranger);

    bool operator<(Stand& stranger);

    // Geef waarde van hoogte.
    int getHoogte ()
    {
      return hoogte;
    }

    // Geef waarde van breedte.
    int getBreedte ()
    {
      return breedte;
    }

    // Geef het aantal stenen terug
    int getAantalStenen()
    {
      return aantalStenen;
    }

    //Geeft de Steen terug van een index
    Steen geefSteen(int index){
      return alleStenen[index];
    }

    // Geef het nummer van de steen die vakje (i,j) van het bord bedekt,
    // of 0 als het vakje nog leeg is.
    // Nodig voor automatische test bij het nakijken.
    int getVakje (int i, int j);

    // Druk het bord en de nog beschikbare stenen (met hun nummers, in
    // de oorspronkelijke orientatie) af op het scherm.
    void drukAf ();

    // Lees stenen in vanuit het tekstbestand met naam invoernaam.
    // Controleer daarbij:
    // * of het bestand wel te openen is,
    // * of het aantal stenen en de dimensies van deze stenen wel passen
    //   binnen de grenzen van de opdracht
    // Retourneer:
    // * true, als het invoerbestand te openen was en het aantal stenen
    //   en de dimensies van deze stenen passen binnen de grenzen van
    //   de opdracht
    // * false, anders
    //
    // Pre:
    // * de bedekte vakjes van een steen vormen een samenhangend geheel,
    //   dat wil zeggen: ze sluiten horizontaal of verticaal op elkaar aan
    // * de beschrijving van een steen bevat geen overbodige rijen en
    //   kolommen
    // Post:
    // * Als de returnwaarde true is, zijn de stenen opgeslagen
    //   in membervariabelen
    bool leesInStenen (const char *invoernaam);

    // Bepaalt of een bepaalde zet mogelijk is of niet
    // Retourneer:
    // * true, als zet mogelijk is
    // * false, als zet niet mogelijk is
    //
    // Pre:
    // * Ingevoerde rij(p) en kolom (q) zijn positief
    // * Ingevoerde orientatie is groter dan 0
    bool zetMogelijk(int steennr, int orient, int p, int q);

    // Leg indien mogelijk steen `steennr' in orientatie orient op het bord,
    // in een bounding box met linker boven vakje (rij, kolom).
    // Retourneer:
    // * true, als het lukt om de steen neer te leggen
    // * false, als het niet lukt om de steen neer te leggen
    bool legSteenNeer (int rij, int kolom, int steennr, int orient);
    
    // Hulpfunctie van afkappen(),
    // Bepaalt recursief de grootte van een (1) lege deelruimte
    // van het bord.
    //
    // Pre:
    // * kopie[][] is geinitialiseerd met waardes
    // Post:
    // * De integer teller bevat nu de grootte van de deelruimte
    void afkappen_rec(int kopie[MaxDimBord][MaxDimBord], int i, int j, int &teller);
    
    // Geeft het antwoord terug op de vraag:
    // Moeten we eerder afkappen (van een stand verder bekijken)?
    // Bepaalt groottes van deelruimtes en vergelijkt deze
    // groottes met de ggd van alle stenen
    //
    // Retourneer:
    // * true, als verder de stand bekijken geen nut heeft
    // * false, als verder de stand bekijken WEL nut heeft
    bool afkappen();

    // Checkt of het bord vol staat met stenen
    // Retourneer true als wel vol, false als niet vol
    bool vol();

    // Vergelijkt twee standen door te kijken naar de inhoud
    // van hun borden.
    // Geeft true terug als a & b gelijk zijn, anders false
    bool vergelijkStand(Stand a, Stand b);

    //Checkt of een stand in een vector gehad zit
    // Geeft true terug als in gehad, anders false
    bool inGehad(std::vector<Stand> gehad, Stand s);

    // Hulpfunctie van bepaalOplossing()
    // Gaat recursief alle mogelijke zetten per steen doen om
    // op een oplossing uit te komen. Stopt als het een oplossing
    // heeft gevonden
    //
    // Retourneer:
    // * true, als er wel een oplossing is gevonden
    // * false, anders
    bool zoekOplossing (long long &aantalStanden, bool slim,
                        int steen, int oplossing[MaxDimBord][MaxDimBord]);

    // Bepaal zo mogelijk 1 oplossing, uitgaande van de huidige stand.
    // Als slim==true, controleer dan na iedere zet of het aantal vakjes
    // in elk gebied nog een veelvoud van de grootste gemene deler is.
    // Als een oplossing gevonden is, stop dan met zoeken.
    // Retourneer:
    // * true, als er minstens 1 oplossing is
    // * false, anders
    // 
    // Post:
    // * De stand (de inhoud van het bord) is hetzelfde als voor aanroep
    //   van deze functie.
    // * Parameter aantalStanden is gelijk aan het aantal standen dat we
    //   hebben bekeken bij deze zoektocht
    // * Als er een oplossing is gevonden, bevat parameter oplossing deze
    //   oplossing (de complete bedekking van het bord met nummers van
    //   de stenen)
    bool bepaalOplossing (long long &aantalStanden, bool slim,
                          int oplossing[MaxDimBord][MaxDimBord]);

    // Debugfunctie: print alle orientaties van een steen uit
    void printOrientaties (int steennr);

    // Druk oplossing af op het scherm.
    // Pre:
    // * Parameter oplossing bevat een oplossing voor het huidige bord,
    //   met geldige dimensies en geldige steennummers.
    void drukAfOplossing (int oplossing[MaxDimBord][MaxDimBord]);

    // Hulpfunctie van telOplossingen
    // Gaat recursief per steen mogelijke zetten zetten en telt
    // ondertussen de oplossingen die het is tegengekomen
    long long aantalOplossingen (long long &aantalStanden, bool slim,
                                int steen);

    // Bepaalt voor een steennr, de mogelijke zetten die gedaan 
    // kunnen worden.
    // Retourneer:
    // * Een vector die bestaat uit arrays van lengte 3 {i,j,k}.
    //   Oftewel retourneert een vector bestaande uit mogelijke zetten
    std::vector<std::array<int, 3>> bepaalZetten(int steennr);

    // Tel het aantal oplossingen, uitgaande van de huidige stand.
    // Als slim==true, controleer dan na iedere zet of het aantal vakjes
    // in elk gebied nog een veelvoud van de grootste gemene deler is.
    // Retourneer:
    // * het gevonden aantal oplossingen
    // Post:
    // * Parameter aantalStanden is gelijk aan het aantal standen dat we
    //   hebben bekeken bij deze zoektocht
    long long telOplossingen (long long &aantalStanden, bool slim);

  private:
    int hoogte, breedte,   // aantal rijen/kolommen van het bord
        bord[MaxDimBord][MaxDimBord];
          // Bovenste rij is rij 0, onderste rij is rij hoogte-1.
          // Meest linker kolom is kolom 0, meest rechter kolom is
          // kolom breedte-1.

    int aantalStenen; //aantal stenen
    Steen alleStenen[MaxNrstenen];  //vector die alle van het spel bevat
    
};

#endif

