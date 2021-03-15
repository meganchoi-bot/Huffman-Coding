#include "BitOutputStream.hpp"
#include <fstream>

// Write single bit to buffer
void BitOutputStream::writeBit(int bit) { 
    // if bitwise buffer is full, flush
    if(buffer_index == 8) {
        flush();
    }
    // write bit to buffer using shift and or
    buffer = buffer | (bit << (7 - buffer_index));
    buffer_index++;
}

// write int to compressed file for frequency?
void BitOutputStream::writeInt(int i) {
    out.write((char*)&i, 3);
}

void BitOutputStream::flush() {
    out.put(buffer); // write the bitwise buffer to the ostream
    out.flush(); // flush the ostream (optional, slower to do it here)
    buffer = 0; // clear the bitwise buffer
    buffer_index = 0; // reset buffer_index
}
