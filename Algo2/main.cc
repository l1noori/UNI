// Hoofdprogramma voor oplossing voor tweede programmeeropdracht Algoritmiek,
// voorjaar 2024: Stenen Leggen.
//
// Biedt de gebruiker een menustructuur om
// * een voor een zelf stenen te leggen op een bord
// * te vragen
//   - om een oplossing te bepalen vanaf de huidige stand
//   - of om het aantal oplossingen vanaf de huidige stand te tellen.

/*
* authors: N.I. van Laarhoven (s3323854) and J. H Liu (s2723468)
* last edited: 13/05/2024
*/

#include <iostream>
#include <ctime>  // voor clock() en clock_t
#include "standaard.h"
#include "stand.h"
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

//*************************************************************************

// Speel het spel vanaf stand s1.
// Hierbij krijgt de gebruiker herhaaldelijk de kans om
// * een steen in een bepaalde orientatie op het bord neer te leggen.
//   Als dat een toegestane zet is, wordt die uitgevoerd.
//   Na iedere zet wordt de stand afgedrukt.
// * te vragen
//   - om een oplossing te bepalen vanaf de huidige stand
//   - of om het aantal oplossingen vanaf de huidige stand te tellen.
//
// Dit alles gaat door totdat de gebruiker aangeeft dat hij wil stoppen
// met het huidige spel.
void doeSpel (Stand *s1)
{ int hoogte, breedte,
      keuze,
      rij, kolom, steennr, orient,
      oplossing[MaxDimBord][MaxDimBord];
  long long aantalStanden,  // aantal bekeken standen bij aanroep
                            // van bepaalOplossing of telOplossingen
            aantalOpl;  // aantal oplossingen gevonden met telOplossingen
  clock_t t1, t2;
  bool gelukt;   // oplossing gevonden

  hoogte = s1 -> getHoogte ();
  breedte = s1 -> getBreedte ();

  do
  { s1 -> drukAf ();
    std::cout << std::endl;
    std::cout << "1. Een zet doen" << std::endl;
    std::cout << "2. Bepaal een oplossing (dom)" << std::endl;
    std::cout << "3. Bepaal een oplossing (slim)" << std::endl;
    std::cout << "4. Tel oplossingen (dom)" << std::endl;
    std::cout << "5. Tel oplossingen (slim)" << std::endl;
    std::cout << "6. Stoppen met dit spel" << std::endl;
    std::cout << std::endl;
    std::cout << "Maak een keuze: ";
    std::cin >> keuze;

    switch (keuze)
    { case 1: std::cout << std::endl;
              std::cout << "Geef het nummer van een rij (0.." << hoogte-1
                   << " vanaf boven): ";
              std::cin >> rij;
              std::cout << "Geef het nummer van een kolom (0.." << breedte-1
                   << " vanaf links): ";
              std::cin >> kolom;
              std::cout << "Geef het nummer van een steen: ";
              std::cin >> steennr;
              s1->printOrientaties(steennr);
              std::cout << "Geef een orientatie van deze steen: ";
              std::cin >> orient;

              if (!s1->legSteenNeer (rij, kolom, steennr, orient))
              { std::cout << std::endl;
                std::cout << "De opgegeven zet was niet mogelijk." << std::endl;
              }
              break;
      case 2:
      case 3: t1 = clock ();
              if (keuze==2)
                gelukt =
                      s1 -> bepaalOplossing (aantalStanden, false, oplossing);
              else
                gelukt =
                      s1 -> bepaalOplossing (aantalStanden, true, oplossing);
              t2 = clock ();
              if (gelukt)
                s1 -> drukAfOplossing (oplossing);
              std::cout << std::endl;
              std::cout << "We hebben " << aantalStanden << " standen bekeken."
                   << std::endl;
              std::cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
                   << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden."
                   << std::endl;

              break;
      case 4:
      case 5: t1 = clock ();
              if (keuze==4)
                aantalOpl = s1 -> telOplossingen (aantalStanden, false);
              else
                aantalOpl = s1 -> telOplossingen (aantalStanden, true);
              t2 = clock ();
              std::cout << std::endl;
              std::cout << "We hebben " << aantalStanden << " standen bekeken."
                   << std::endl;
              std::cout << "We vonden " << aantalOpl << " oplossingen." << std::endl;
              std::cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
                   << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden."
                   << std::endl;
              break;
      case 6: break;
      default: std::cout << std::endl;
               std::cout << "Voer een goede keuze in!" << std::endl;
    }  // switch

  } while (keuze!=6);

}  // doeSpel

//*************************************************************************

void hoofdmenu ()
{ Stand *s1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
              // en weer weg te gooien
  int keuze,
      hoogte, breedte;
  char invoernaam[MaxBestandsNaamLengte+1];

  do
  {
    std::cout << std::endl;
    std::cout << "Geef een waarde voor het aantal rijen van het bord (hoogte): ";
    std::cin >> hoogte;
    std::cout << "Geef een waarde voor het aantal kolommen van het bord (breedte): ";
    std::cin >> breedte;
    if (integerInBereik ("hoogte", hoogte, 2, MaxDimBord)
        && integerInBereik ("breedte", breedte, 2, MaxDimBord))
    { s1 = new Stand (hoogte, breedte);

      std::cout << std::endl;
      std::cout << "Geef de naam van het tekstbestand met de stenen: ";
      std::cin >> invoernaam;
      if (s1 -> leesInStenen (invoernaam))
        doeSpel (s1);

      delete s1;  // netjes opruimen

      do
      { std::cout << std::endl;
        std::cout << "1. Een nieuw spel spelen" << std::endl;
        std::cout << "2. Stoppen" << std::endl;
        std::cout << std::endl;
        std::cout << "Maak een keuze: ";
        std::cin >> keuze;

        if (keuze<1 || keuze>2)
        { std::cout << std::endl;
          std::cout << "Voer een goede keuze in!" << std::endl;
        }
      } while (keuze<1 || keuze>2);

    }  // if integerInBereik

  } while (keuze!=2);

}  // hoofdmenu

//*************************************************************************
  
int main ()
{
  hoofdmenu ();

  return 0;

}
