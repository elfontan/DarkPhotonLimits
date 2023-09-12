#include <TString.h>

void makeCardsAndWS(TString year);

int main (int argc, char *argv[]) {
  
  if (argc > 1){
    return 0;
  }

  makeCardsAndWS("2022");
  //makeCardsAndWS("2017");
  //makeCardsAndWS("2018"); // Two years not combined, combination was ongoing


  return 0;
}
