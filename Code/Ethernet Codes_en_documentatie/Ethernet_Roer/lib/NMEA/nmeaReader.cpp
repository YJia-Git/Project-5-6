#include "nmeaReader.h"

String nmeaReader::checkSumCalculator(String currentString) {
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

void nmeaReader::newMessage(String input)  {
    message = input;
}

bool nmeaReader::check()  {
    String checksum = message.substring(message.indexOf("*") + 1, message.length());
    String data = message.substring(1, message.indexOf("*"));
    // check if message is recieved correctly
    if (checksum.toInt() == checkSumCalculator(data).toInt()) {
    return true;
    } else {
    //        // print both of the checksums
    //        Serial.print(checksum);
    //        Serial.print('\t');
    //        Serial.print(checkSumCalculator(data));
    return false;
    }
}
String nmeaReader::header() {
    String data = message.substring(1, message.indexOf("*"));
    return data.substring(0, 5);
}
String nmeaReader::field(int index) {
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