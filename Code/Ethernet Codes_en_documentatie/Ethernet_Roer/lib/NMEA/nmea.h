#include <Arduino.h>
class nmea  {
  private:
    String header;
    int fields;
    String message;
    String checkSumCalculator(String currentString);
  public:
    void begin(String header, int numberOfFieleds);
    String make(String field[]);
    bool check(String input);
    String getHeader();
    String getField(int index);
};
