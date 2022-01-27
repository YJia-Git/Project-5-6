#include "nmea.h"
// init nmea base code
void nmea::begin(String header, int numberOfFieleds)  {
    this->header = header;
    fields = numberOfFieleds;
}
// A function to calculate the checksum of a string
String nmea::checkSumCalculator(String currentString) {
    int checkSum = 0;
    // loop trough all chars of the whole string
    for (int i = 0; i < currentString.length(); i++) {
        char x = currentString[i];
        byte y = x;
        // the XOR
        checkSum ^= y;
    }
    String hex = String(checkSum, HEX);
    return hex;
}
// make nmea message
String nmea::make(String field[])  {
    // start making the nmea data
    String nmea = header; // devices code
    for (int i = 0; i < fields; i++)  {
    nmea += "," + String(field[i]);
    }
    // add checksum
    nmea = "!" + nmea + "*" + checkSumCalculator(nmea);
    return nmea;
}
// get the nmea message and check if recieved correctly
bool nmea::check(String input)  {
    message = input;
    String checksum = message.substring(message.indexOf("*") + 1, message.length());
    String data = message.substring(1, message.indexOf("*"));
    // check if message is recieved correctly
    if (checksum.toInt() == checkSumCalculator(data).toInt()) {
        return true;
    } else {
        return false;
    }
}
// return the header of the message
String nmea::getHeader() {
    String data = message.substring(1, message.indexOf("*"));
    return data.substring(0, 5);
}
// return the field value of index(start from 0)
String nmea::getField(int index) {
    String data = message.substring(6, message.indexOf("*"));
    int i, j;
    i = data.indexOf(",");
    j = data.indexOf(",", i + 1);
    if (index >= 0) {
    for (int h = 0; h < index; h++)  {
        i = j;
        j = data.indexOf(",", i + 1);
        if (j < 0) {
        // set to end, because it is the last field
        j = data.length();
        }
        // check if field is valid
        if (i < 0 || i + 1 >= j)  {
        return "index not found";
        }
    }
    return data.substring(i + 1, j);
    } else {
    return "invalid index";
    }
}