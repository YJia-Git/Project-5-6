#include <Arduino.h>
 
class nmeaReader  {
  private:
    String message;
    String checkSumCalculator(String currentString);
  public:
    nmeaReader(String input): message(input) {};
    void newMessage(String input);
    bool check();
    String header();
    String field(int index);
};