#include <iostream>
#include "HCTree.hpp"
#include <vector>
#include <fstream>
using namespace std;

int main( int argc, char* argv[]) {
    // open input file for reading
    ifstream infile;
    infile.open(argv[1], ios::binary);
    ofstream outfile;

    // check if file is empty
    if (infile.peek() == std::ifstream::traits_type::eof()) {
        infile.close();
        // open file
        outfile.open(argv[2], ios::binary);
        outfile.close();    
        cout << "Input file empty\n";
        return 0;
    }

    // create bit input stream
    BitInputStream bitIn(infile);

    int next_byte; // to store the byte read in 
    vector<int> freq(256); // vector to save header info and recreate tree
    int byteCount = 0; // keep track of how many bytes have been read
    int i = 0; // int to set freq vector

    // read file header (first 1024 bytes?)
    while ((next_byte = infile.peek()) != EOF && byteCount <= 1024 && i < 256) {
        freq[i] = bitIn.readInt();
        byteCount++;
        i++;
    }

    int count = 0;
    // count number of chars to decode (how many times to call decode)
    for (unsigned int i = 0; i < freq.size(); i++) {
        if (freq[i] > 0) {
            count += freq[i];
        }
    }

    // reconstruct tree
    HCTree t;
    t.build(freq);

    // open output file for writing
    outfile.open(argv[2]);

    // decode using HCTree for count number of times
    // need to start reading bits after header??
    for(int i = 0; i < count; i++) {
        int num = t.decode(bitIn);
        outfile.write((char*)&num, sizeof(unsigned char));
    }

    // close files
    outfile.close();
    infile.close();

    return 0;
}
