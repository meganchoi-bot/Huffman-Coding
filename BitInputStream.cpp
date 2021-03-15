#include "BitInputStream.hpp"
#include <fstream>

// Read single bit from buffer
int BitInputStream::readBit() {
    // if buffer has been read, refill from in
    if (buffer_index == 8) {
        buffer = in.get(); // read one byte from istream to bitwise buffer
        buffer_index = 0; 
    }
    // shift MSB to LSB
    int nBit = buffer >> (7 - buffer_index);
    buffer_index++;
    // Return bit at index in buffer
    return nBit & 1;
}

int BitInputStream::readInt() {
    int i;
    in.read((char*)&i, 3);
    return i;
}
