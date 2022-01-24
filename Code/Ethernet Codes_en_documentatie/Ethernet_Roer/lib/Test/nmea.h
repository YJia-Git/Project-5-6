#include <Arduino.h>
class nmea  {
  private:
    String header;
    int fields;
    String message;
    String checkSumCalculator(String currentString);
  public:
    nmea(String header, int numberOfFieleds);
    nmea(String input): message(input) {};
    String make(String field[]);
    bool check(String input);
    String getHeader();
    String getField(int index);
};
