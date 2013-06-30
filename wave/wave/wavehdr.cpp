//
//  wavehdr.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/30/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "wavehdr.h"
#include <stdexcept>
#include <string>
using namespace std;

wavhdr::wavhdr(std::istream& in)
: ChunkId{buffered_read(in, CHUNK_ID_SIZE)},
ChunkSize{binread(in, ChunkSize)},
Format{buffered_read(in, FORMAT_SIZE)}
{
    if (Format != "WAVE") {
        throw domain_error(string(__func__) +
                           ": Format is not WAVE.");
    }
}

std::ostream& wavhdr::textout(std::ostream& out) const  {
    out << "Header=" << ChunkId << " size=" << ChunkSize << " Format=" << Format;
    out << " {" << size() << "}";
    return out;
}

std::ostream& wavhdr::binout(std::ostream& out) const {
    out.write(ChunkId.c_str(), CHUNK_ID_SIZE);
    binwrite(out, ChunkSize);
    out.write(Format.c_str(), FORMAT_SIZE);
    return out;
}
