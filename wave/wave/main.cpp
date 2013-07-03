//
//  main.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/13/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "binutils.h"
#include "wavehdr.h"
#include "subchunk.h"
#include "wavefile.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <vector>
#include <array>

using namespace std;

/**
 * @brief Copy a wave file to a file.
 * @param outname - the name of the output file
 * @param wv - the wave file
 * @return true if successful
 */
bool makeCopy(const string& outname, const wavefile& wv) {
    auto out = ofstream{outname};
    if (!out) {
        cout << "Could not open " << outname << endl;
        return false;
    }
    wv.binout(out);
    cout << "Wrote duplicate file " << outname << endl;
    return true;
}

/**
 * @brief Read a wave file
 * @parm infile - The name of the input file
 * @return std::pair<bool, wavefile>
 */
pair<bool,wavefile> readwf(const string& infile)
{
    auto in = ifstream{infile};
    if (!in)
    {
        cout << "Could not open " << infile << endl;
        return make_pair(false, wavefile{});
    }
    
    auto wf = wavefile{};
    try {
        wf = wavefile{in};
    } catch (std::exception& e) {
        cout << "Exception creating wavefile object from " << infile << endl;
        cout << e.what() << endl;
        return make_pair(false, wf);
    }
    
    return make_pair(true, move(wf));
}

/**
 * @brief Copy a wave file
 * @param infile
 * @param output
 * @return true if successful
 */
bool copywf(const string& infile, const string& outfile)
{
    auto in = readwf(infile);
    if (in.first)
    {
        in.second.textout(cout);
        cout << endl;
        return makeCopy(outfile, in.second);
    }
    return false;
}

/**
 * @brief print a wave file
 * @param infile
 * @return true if successful
 */
bool printwf(const string& infile) // Print a wave file
{
    auto in = readwf(infile);
    if (in.first)
    {
        in.second.textout(cout);
        cout << endl;
        return true;
    }
    return false;
}

/**
 * @brief Fix a wave file
 * @param infile
 * @param outfile
 * @return true if successful
 */
bool fixwf(const string& infile, const string& outfile)
{
    cout << "Try to fix " << infile << endl;
    auto in = ifstream{infile};
    if (!in)
    {
        cout << "Could not open " << infile << endl;
        return false;
    }
    
    auto wf = wavefile{};
    
    if (wf.fix(in)) {
        wf.textout(cout);
        return makeCopy(outfile, wf);
    }
    return false;
}

int main(int argc, const char * argv[])
{
    // insert code here...
    if (argc < 3) {
        cout << "       " << argv[0] << " -print <infile>" << endl;
        cout << "Usage: " << argv[0] << " -copy <infile> <outfile>" << endl;
        cout << "Usage: " << argv[0] << " -fix <infile> <outfile>" << endl;
        return 1;
    }
    
    auto cmd = string{argv[1]};
    
    if (cmd == "-fix")
    {
        if (argc != 4) {
            cout << "Usage: " << argv[0] << " -fix <infile> <outfile>" << endl;
            return 0;
        }
        fixwf(argv[2], argv[3]);
    }
    else {
        if (cmd == "-print") {
            printwf(argv[2]);
        } else {
            if (argc != 4) {
                cout << "Usage: " << argv[0] << " -copy <infile> <outfile>" << endl;
            }
            copywf(argv[2], argv[3]);
        }
    }
    return 0;
}


