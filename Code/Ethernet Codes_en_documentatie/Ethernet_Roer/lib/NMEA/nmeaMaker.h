#include <Arduino.h>
class nmeaMaker  {
  private:
    String header;
    int fields;
    String checkSumCalculator(String currentString);
  public:
    nmeaMaker(String header, int numberOfFieleds);
    String make(String field[]);
};