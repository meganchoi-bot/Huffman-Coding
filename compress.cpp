#include <iostream>
#include "HCTree.hpp"
#include <vector>
#include <fstream>
using namespace std;

int main( int argc, char* argv[]) {
    // check for correct number args
    if (argc != 3) {
        cout << "Incorrect number of parameters\n";
        return 1;
    }   

    // stream for reading
    ifstream infile;
    // stream for writing
    ofstream outfile;
    infile.open(argv[1], ios::binary);

    // check if file is empty
    if (infile.peek() == std::ifstream::traits_type::eof()) {
        infile.close();
        // open file
        outfile.open(argv[2], ios::binary);
        outfile.close();    
        cout << "Input file empty\n";
        return 0;
    }

    int next_byte; // create object to hold bytes
    vector<int> freq(256); // vector to hold bytes & frequencies
    int count;

    // read file to make populate freq vector
    while((next_byte = infile.get()) != EOF) { // loop as long as valid
        byte symbol = (byte)next_byte; // get the next byte
        freq[symbol]++;
        count++;
    }

    // build HCTree
    HCTree t;
    t.build(freq);

    // close infile for now
    infile.close();

    // open file for writing
    outfile.open(argv[2], ios::binary);
    BitOutputStream bitOut(outfile);

    // write naive header??
    for (unsigned int i = 0; i < freq.size(); i++) {
        bitOut.writeInt(freq[i]);
    }

    // open file again to encode
    infile.open(argv[1], ios::binary); // open in file again
    while((next_byte = infile.get()) != EOF) { // loop as long as valid
        byte symbol = (byte)next_byte; 
        t.encode(symbol, bitOut); // use tree to encode file
    }
    // flush out last bits
    bitOut.flush();

    // close files
    outfile.close();
    infile.close();

    return 0;
}
