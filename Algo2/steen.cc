/*
* authors: N.I. van Laarhoven (s3323854) and J. H Liu (s2723468)
* last edited: 13/05/2024
*/

// Implementatie van klasse Steen.

#include <iostream>
#include "steen.h"
#include "standaard.h"

//*************************************************************************

Steen::Steen ()
{ 
  for (int i = 0; i < MaxDimSteen ; i++){
    for (int j = 0 ; j < MaxDimSteen; j++){
      for (int k = 0; k < 8; k++){
        inhoud[k][i][j] = '.';
      }
    }
  }
  m = MaxDimSteen;
  n = MaxDimSteen;
  huidigeRotatie = 0;
  gebruikt = false;

}  // Steen

//*************************************************************************

Steen Steen::operator=(Steen& original)
{
  m = original.getM();
  n = original.getN();
  gebruikt = original.getGebruikt();
  vakjes = original.grootte();

  //kopieer de inhoud
  for (int i = 0 ; i < MaxDimSteen; i++){
    for (int j = 0 ; j < MaxDimSteen; j++){
      for (int k = 0; k < 8; k++){
        inhoud[k][i][j] = original.getInhoud(i,j,k);
      }
    }
  }
  return *this;
}

//*************************************************************************

bool Steen::getGebruikt() 
{
  return gebruikt;
}

//*************************************************************************

void Steen::gebruikSteen () 
{
  gebruikt = true;
}

//*************************************************************************

bool Steen::leesIn (std::ifstream &fin)
{
  char input;
  fin >> m;
  fin >> n;
  if (!integerInBereik("M", m, 1, MaxDimSteen)
  || !integerInBereik("N", n, 1, MaxDimSteen)){
    return false;
  }
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      fin >> input;
      inhoud[0][i][j] = input;
      if (input == 'X'){
        vakjes++;
      }
    }
  }
  maakAlleRotaties();
  return true;

}  // leesIn


//*************************************************************************

 void Steen::maakAlleRotaties(){
  spiegelHorizontaleAs();
  spiegelVerticaleAs();
  vindRotaties();
  vindDubbelen();
 }
//*************************************************************************

// Is vakje (i,j) van de steen gevuld?
bool Steen::gevuld (int i, int j)
{
  if (inhoud[0][i][j]== 'X'){
    return true;
  }
  return false;

}

//*************************************************************************

bool Steen::gevuldOrientatie(int i, int j, int k)
{
  int orientatie = k;
  if (isdigit(inhoud[k][0][0])){
    orientatie = inhoud[k][0][0] - '0';
  }
  if (inhoud[orientatie][i][j] == 'X')
  {
    return true;
  }
  return false;
}

//*************************************************************************
int Steen::grootte ()
{
  return vakjes; 

}  // grootte

//*************************************************************************

void Steen::drukAf ()
{
  drukAfRotatie(0);

}  // drukAf

//*************************************************************************

bool Steen::drukAfRotatie (int rotatie)
{
  int x, y;
  
  if (isdigit(inhoud[rotatie][0][0])){
    return false;
  } else {
    if ((rotatie % 2) == 1){
      x = n; y = m;
    } else {
      x = m; y = n;
    }
    for (int i = 0; i < x ; i++){
      for (int j = 0 ; j < y; j++){
        std::cout << inhoud[rotatie][i][j];
      }
      std::cout<< std::endl;
    }
    std::cout<<std::endl;
    return true;
  }
}  // drukAf


//*************************************************************************

bool Steen::dezelfdeSteen(int steen1, int steen2){
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      if (inhoud[steen1][i][j] != inhoud[steen2][i][j]){
        return false;
      }
    }
  }
  return true;
}

//*************************************************************************

void Steen::vindDubbelen()
{
  for (int i = 0; i < 8; i++){
    for (int j = i+1; j < 8; j++){
      if (dezelfdeSteen(i, j)){
        inhoud[j][0][0] = i + '0';
      }
    }
  }
}

//*************************************************************************

void Steen::roteer (int aantalkeer)
{
  huidigeRotatie = (huidigeRotatie + aantalkeer) % 8;
}  // roteer

//*************************************************************************

void Steen::vindRotaties(){
  for (int i = 0; i < n; i++){
    for (int j = 0; j < m; j++){
      inhoud[5][i][j] = inhoud[0][j][i];
      inhoud[7][i][j] = inhoud[2][j][i];
      inhoud[1][i][j] = inhoud[4][j][i];
      inhoud[3][i][j] = inhoud[6][j][i];
    }
  }
}

//*************************************************************************

void Steen::spiegelVerticaleAs(){
  // wisselen j met n-1 - j
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      inhoud[6][i][j] = inhoud[0][i][n-1-j];
      inhoud[2][i][j] = inhoud[4][i][n-1-j];
    }
  }
}

//*************************************************************************

void Steen::spiegelHorizontaleAs ()
{
  // wisselen i met m-1 - i
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++){
      inhoud[4][i][j] = inhoud[0][m-1-i][j];
    }
  }
}  // spiegelHorizontaleAs

//*************************************************************************
