/*
* authors: N/A (template)
* last edited: N/A
*/

// Implementatie van standaard functies.

#include <iostream>
#include <cstdlib>  // voor rand
#include "standaard.h"

//*************************************************************************

bool integerInBereik (const char *variabele, int waarde,
                      int minWaarde, int maxWaarde)
{
  if (waarde>=minWaarde && waarde<=maxWaarde)
    return true;
  else
  { std::cout << variabele << "=" << waarde << ", maar moet in [" << minWaarde
         << "," << maxWaarde << "] liggen." << std::endl;
    return false;
  }

}  // integerInBereik

//*************************************************************************

bool integerInBereik (int waarde, int minWaarde, int maxWaarde)
{
  if (waarde>=minWaarde && waarde<=maxWaarde)
    return true;
  else
    return false;

}  // integerInBereik

//*************************************************************************

int randomGetal (int min, int max)
{ int bereik,
      r;

  bereik = max - min + 1;

  r = ((rand())%bereik) + min;
  return r;

}  // randomGetal

//*************************************************************************

int ggd (int m, int n)
{ int rest;

  while (n!=0)
  { rest = m%n;
    m = n;
    n = rest;
  }

  return m;

}  // Grootste gemene deler

//*************************************************************************
