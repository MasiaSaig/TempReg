#include "utils.h"

uint8_t getNumberOfDigits(int number){
  int8_t digits = 0;
  
  do{
    number = number / 10;
    ++digits;
  }while(number);
  
  return digits;
}