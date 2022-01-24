#include "nmeaMaker.h"

//A function to calculate the checksum of a string
String nmeaMaker::checkSumCalculator(String currentString) {
    int checkSum = 0;
    // loop trough the whole string
    for (int i = 0; i < currentString.length(); i++) {
    char x = currentString[i];
    byte y = x;
    // the XOR
    checkSum ^= y;
    } 
    // return the checksum in string format
    String hex = String(checkSum, HEX);
    return hex;
}
nmeaMaker::nmeaMaker(String header, int numberOfFieleds)  {
    this->header = header;
    fields = numberOfFieleds;
}
// number is amount of fields, start counting from 1
String nmeaMaker::make(String field[])  {
    // start making the nmea data
    String nmea = header; // devices code
    for (int i = 0; i < fields; i++)  {
    nmea += "," + String(field[i]);
    }
    // add checksum
    nmea = "!" + nmea + "*" + checkSumCalculator(nmea);
    return nmea;
}
