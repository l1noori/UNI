/*
* authors: N.I. van Laarhoven (s3323854) and J. H Liu (s2723468)
* last edited: 13/05/2024
*/

// Implementatie van klasse Stand.

#include <iostream>
#include <iomanip>
#include <array>
#include "stand.h"
#include "standaard.h"

//*************************************************************************

Stand::Stand()
{
  // max bij max
  hoogte = MaxDimBord;
  breedte = MaxDimBord;
  for (int i = 0 ; i < MaxDimBord; i++){
    for (int j = 0 ; i < MaxDimBord; j++){
      bord[i][j] = -1;
    }
  }

}

//*************************************************************************

Stand::Stand (int waardeM, int waardeN)
{
  // waardeM bij waardeN
  hoogte = waardeM;
  breedte = waardeN;
  for (int i = 0; i < waardeM; i++){
    for (int j = 0; j < waardeN; j++){
      bord[i][j] = -1;
    }
  }

}

//*************************************************************************

Stand::~Stand ()
{
  // Leeg.
}  // ~Stand

//*************************************************************************

Stand Stand::operator=(Stand& original){

  if (this == &original){
    return *this;
  }
  hoogte = original.getHoogte();
  breedte = original.getBreedte();
  aantalStenen = original.getAantalStenen();

  //kopieer bord
  for (int i = 0; i < hoogte ; i++){
    for (int j = 0; j < breedte ; j++){
      bord[i][j] = original.getVakje(i,j);
    }
  }

  //kopieer alleStenen
  for (int p = 0 ; p < aantalStenen; p++){
    Steen ding = original.geefSteen(p);
    alleStenen[p] = ding;
  }
  return *this;
}

//*************************************************************************

bool Stand::operator==(Stand& stranger)
{
  //Het enige wat boeit is of de borden gelijk zijn aan elkaar
  for (int i = 0; i < hoogte ; i++){
    for (int j = 0 ; j < breedte ; j++){
      if (bord[i][j] != stranger.getVakje(i,j)){
        return false;
      }
    }
  }
  return true;
}

//*************************************************************************

int Stand::getVakje (int i, int j)
{
  return bord[i][j];

}  // getVakje

//*************************************************************************

void Stand::drukAf ()
{
  //Print de stenen die niet gebruikt zijn
  for (int k = 0 ; k <= aantalStenen-1; k++){
    if (!alleStenen[k].getGebruikt()){
      std::cout << k << ": " << std::endl;
      alleStenen[k].drukAf();   
    }
  }

  //print het bord uit:
  for (int p = 0 ; p < hoogte; p++){
    for (int q = 0; q < breedte; q++){
      if (bord[p][q] == -1){
        std::cout << " . ";
      }
      else {
        std::cout << " " << bord[p][q] << " ";
      }
    }
    std::cout<< std::endl;
  }
  

}  // drukAf

//*************************************************************************

bool Stand::leesInStenen (const char *invoernaam)
{

  std::ifstream invoerFile(invoernaam);
  if (invoerFile.fail()){
    std::cerr << "File could not be opened.";
    return false;
  }
  invoerFile >> aantalStenen;
  if (!integerInBereik("Aantal stenen", aantalStenen, 1, MaxNrstenen)){
    return false;
  }
  for (int i = 0; i < aantalStenen; i++){
    if (invoerFile.peek() == EOF){
      std::cerr << "Het aantal stenen komt niet overeen. Error 1";
      return false;
    }
    Steen steen = Steen();
    if (!steen.leesIn(invoerFile)){
      std::cerr << "Er is een fout in de configuratie van de stenen.";
      return false;
    }
    alleStenen[i] = steen;
  }
  char last;
  invoerFile >> last;
  return true;

}  // leesInStenen

//*************************************************************************

void Stand::printOrientaties (int steennr){
  std::cout << "Mogelijke orientaties: " << std::endl;
  for (int i = 0; i < 8; i++){
    alleStenen[steennr].drukAfRotatie(i);
  }
}

//*************************************************************************

bool Stand::zetMogelijk(int steennr, int orient, int p , int q)
{
  if (alleStenen[steennr].getGebruikt()){
    std::cerr << "Deze steen is al gebruikt." << std::endl;
    return false;
  }
  for (int i=0; i < MaxDimSteen ; i ++){
    for (int j=0; j < MaxDimSteen ; j++){
      if (alleStenen[steennr].gevuldOrientatie(i,j, orient)){
        if((i+p) >= hoogte || (j+q) >= breedte || bord[i+p][j+q] != -1){ 
          return false;
        }
      }
    }
  }
  return true;
}

//*************************************************************************

bool Stand::legSteenNeer (int rij, int kolom, int steennr, int orient)
{
 if (!zetMogelijk(steennr, orient, rij, kolom)){
    return false;
  } else { //Plaats de steen
    for (int i = 0; i < MaxDimSteen; i++){
      for (int j = 0; j < MaxDimSteen; j++){
        if (alleStenen[steennr].gevuldOrientatie(i,j,orient)){
          bord[i+rij][j+kolom] = steennr;
        }
      }
    }
  }
  alleStenen[steennr].gebruikSteen();
  return true;
}  // legSteenNeer

//*************************************************************************

bool Stand::vol(){
  for (int i = 0; i < hoogte ; i++){
    for (int j = 0; j < breedte; j++){
      if (bord[i][j] == -1){
        return false;
      }
    }
  }
  return true;
}
//*************************************************************************

// false if different, true if the same
bool Stand::vergelijkStand(Stand a, Stand b)
{
  for (int i = 0; i < hoogte ; i++){
    for (int j = 0 ; j < breedte ; j++){
      if (a.getVakje(i,j) != b.getVakje(i,j)){
        return false;
      }
    }
  }
  return true;
}

//*************************************************************************

bool Stand::inGehad(std::vector<Stand> gehad, Stand s)
{
  for (auto & element : gehad ){
    if (vergelijkStand(element, s)){ //als s in gehad 
      return true;
    }
  }
  return false;

}

//*************************************************************************

void Stand::afkappen_rec(int kopie[MaxDimBord][MaxDimBord], int i, int j, int &teller ){
  if (i>= hoogte || i< 0 || j <0 || j >= breedte || kopie[i][j] >= 0){
    return;
  } else {
    if (kopie[i][j] == -1){
      kopie[i][j] = -2;
      teller++;
      afkappen_rec(kopie, i+1,j, teller);
      afkappen_rec(kopie,i, j+1, teller);
      afkappen_rec(kopie,i-1,j,teller);
      afkappen_rec(kopie, i, j-1, teller);
    }
  }
}
  
//*************************************************************************

bool Stand::afkappen()
{
  
  int teller = 0;
  int min = 1000000;      //Neem groot getal
  int deler = alleStenen[0].grootte();
  std::vector<int> deelruimtes;
  int kopie[MaxDimBord][MaxDimBord];
  for (int i = 0; i < hoogte ; i++){  //houd hierin bij welke
    for (int j= 0; j < breedte; j++){ //posities al zijn bekeken
      kopie[i][j] = bord[i][j];
    }
  }
  for (int p = 0; p < hoogte ; p++){    //Bepaal deelruimte groottes
    for (int q = 0; q < breedte; q++){
      afkappen_rec(kopie,p,q,teller);
      if (teller != 0){
        deelruimtes.push_back(teller);
        teller = 0;
      }
    }
  }
  //Bepaal minimum van stenen en ggd van stenen
  for (int u = 0; u < aantalStenen; u++){ //vergelijk alle niet-gebruikte stenen
    if (!alleStenen[u].getGebruikt()){
      int aantal = alleStenen[u].grootte();
      if (aantal < min){
        min = aantal;
      }
      for (int v = u; v < aantalStenen; v++){
        if (!alleStenen[u].getGebruikt()){
          int aantalv = alleStenen[v].grootte();
          if (ggd(aantal, aantalv) < deler){
            deler = ggd(aantal, aantalv);
          }
        }
      }
    }
  }
  for (int l = 0 ; l < static_cast<int>(deelruimtes.size()); l++){ //check deelruimtes met deler en min
    if (deelruimtes[l] % deler != 0 || deelruimtes[l] < min){
      return true;
    }
  }

  return false;
}

//*************************************************************************

bool Stand::zoekOplossing(long long &aantalStanden, bool slim, 
                      int steen, int oplossing[MaxDimBord][MaxDimBord])
{ 
  //mogelijke zetten voor steen
  std::vector<std::array<int, 3>> oplossingen = bepaalZetten(steen);

  if (vol()){
    std::cout << "Er is een oplossing gevonden." << std::endl;
    for (int i = 0; i < hoogte ; i++){ //Zet oplossing in array
      for (int j = 0; j < breedte ; j++){
        oplossing[i][j] = getVakje(i,j);
      }
    }
    return true;
  }
  else if (oplossingen.empty() || steen >= aantalStenen) return 0; //er zijn geen mogelijke zetten meer
  else if ((!slim) || (slim && !afkappen())){
    for (auto & zet : oplossingen){
      Stand nieuw = *this;
      nieuw.legSteenNeer(zet[0], zet[1], steen, zet[2]); //zet steen neer
      aantalStanden++;
      // Ga naar de volgende steen
      if(nieuw.zoekOplossing(aantalStanden, slim, steen+1, oplossing)) return true;
    }
  }
  return false;
}

//*************************************************************************

void Stand::drukAfOplossing (int oplossing[MaxDimBord][MaxDimBord])
{
  std::cout<< "Hier is een oplossing: " << std::endl;
  for( int i = 0 ; i < hoogte ; i++){
    for (int j= 0; j < breedte; j++){
      if (oplossing[i][j] == -1){
        std::cout << " . ";
      }
      else {
        std::cout << " " << oplossing[i][j] << " ";
      }
    }
    std::cout << std::endl;
  }
}  // drukAfOplossing

//*************************************************************************

std::vector<std::array<int, 3>> Stand::bepaalZetten(int steennr){
  std::vector<std::array<int, 3>> zetten;
  for (int i = 0; i < hoogte; i++){ //voor elke plek op bord
    for (int j=0; j < breedte; j++){
      for (int k= 0 ; k <8 ; k++){ // voor all orientaties
        // Als het niet een symmetrie van een eerdere orientatie is
        // EN het is een geldige zet
        if (!isdigit(alleStenen[steennr].getInhoud(0,0,k)) && zetMogelijk(steennr, k, i, j)){
          zetten.push_back({i, j, k});
        }
      }
    }
  }
  return zetten;
}
//*************************************************************************

long long Stand::aantalOplossingen (long long &aantalStanden, bool slim, 
                                    int steen)
{ 
  int opl = 0;
  std::vector<std::array<int, 3>> oplossingen = bepaalZetten(steen);
  if (vol()) return 1;
  else if (oplossingen.empty() || steen >= aantalStenen) return 0;
  else if ((!slim) || (slim && !afkappen())){
    for (auto & zet : oplossingen){
      Stand nieuw = *this;
      nieuw.legSteenNeer(zet[0], zet[1], steen, zet[2]);
      aantalStanden++;
      opl+=nieuw.aantalOplossingen(aantalStanden, slim, steen+1);
    }
  }
  return opl;
}

//*************************************************************************
//wrapper functie
bool Stand::bepaalOplossing (long long &aantalStanden, bool slim, 
                                  int oplossing[MaxDimBord][MaxDimBord])
{
  aantalStanden = 0;
  return zoekOplossing(aantalStanden, slim, 0, oplossing);

}  // telOplossingen


//*************************************************************************
//wrapper functie
long long Stand::telOplossingen (long long &aantalStanden, bool slim)
{
  aantalStanden = 0;
  return aantalOplossingen(aantalStanden, slim, 0);
  

}  // telOplossingen

//*************************************************************************

