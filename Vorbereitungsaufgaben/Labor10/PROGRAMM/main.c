#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {

  uint16_t  numberOfElements;
  uint16_t* dataArray;

  uint16_t  currentPosition;

} ringspeicher;


void init_ringspeicher(ringspeicher* speicher, uint16_t numberOfElements) {

  speicher->numberOfElements = numberOfElements;
  speicher->dataArray        = (uint16_t*) calloc(numberOfElements, 2);

  speicher->currentPosition  = 0;

}

void ringspeicher_add(ringspeicher* speicher, uint16_t value) {

  if(speicher->currentPosition == (speicher->numberOfElements-1)) {

    speicher->currentPosition = 0;

  }


}


int main() {

  int ringLength = 16;

  ringspeicher* meinKleinesSpeichi = malloc(sizeof(ringspeicher));

  init_ringspeicher(meinKleinesSpeichi, ringLength);



  free(meinKleinesSpeichi);
  return 0;
}



/* #pragma vector=ADC12VECTOR? */
/* _interrupt adc12_isr() { */




/* } */
