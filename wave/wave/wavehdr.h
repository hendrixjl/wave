//
//  wavehdr.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_wavehdr_h
#define wave_wavehdr_h

#include "binutils.h"
#include <string>
#include <stdint.h>

class wavhdr
{
    enum { CHUNK_ID_SIZE = 4, FORMAT_SIZE = 4 };
public:
    wavhdr()=default;
    wavhdr(const wavhdr&)=default;
    wavhdr(wavhdr&&)=default;
    ~wavhdr()=default;
    wavhdr& operator=(const wavhdr&)=default;
    
    wavhdr(std::istream& in)
    : ChunkId(CHUNK_ID_SIZE, ' '),
    ChunkSize{},
    Format(FORMAT_SIZE, ' ')
    {
        in.read(&ChunkId[0], CHUNK_ID_SIZE);
        binread(in, ChunkSize);
        in.read(&Format[0], FORMAT_SIZE);
    }
    
    std::ostream& textout(std::ostream& out) const  {
        out << "Header=" << ChunkId << " size=" << ChunkSize << " Format=" << Format;
        out << " {" << size() << "}";
        return out;
    }
    
    std::ostream& binout(std::ostream& out) const {
        out.write(ChunkId.c_str(), CHUNK_ID_SIZE);
        binwrite(out, ChunkSize);
        out.write(Format.c_str(), FORMAT_SIZE);
        return out;
    }
    
    uint32_t size() const {
        return CHUNK_ID_SIZE + sizeof(uint32_t) + FORMAT_SIZE;
    }
    
    uint32_t filesize() const {
        return CHUNK_ID_SIZE + sizeof(uint32_t) + ChunkSize;
    }
    
    void fix_size(uint32_t dataSize) {
        ChunkSize = dataSize + FORMAT_SIZE;
    }
private:
    std::string ChunkId{};
    uint32_t ChunkSize{};
    std::string Format{};
};


inline std::ostream& operator<<(std::ostream& out, const wavhdr& o) {
    o.textout(out);
    return out;
}


#endif
