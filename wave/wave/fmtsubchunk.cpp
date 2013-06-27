//
//  fmtsubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "fmtsubchunk.h"
#include "subchunk_factory.h"

using namespace std;

fmtsubchunk::fmtsubchunk(std::istream& in) : SubchunkID{"fmt "} {
    binread(in, SubchunkSize);
    binread(in, AudioFormat);
    binread(in, NumChannels);
    binread(in, SampleRate);
    binread(in, ByteRate);
    binread(in, BlockAlign);
    binread(in, BitsPerSample);
}


fmtsubchunk::fmtsubchunk(uint32_t size,
            uint16_t audioFormat,
            uint16_t numChannels,
            uint32_t sampleRate,
            uint32_t byteRate,
            uint16_t blockAlign,
            uint16_t bitsPerSample)
:
SubchunkID{"fmt "},
SubchunkSize{size},
AudioFormat{audioFormat},
NumChannels{numChannels},
SampleRate{sampleRate},
ByteRate{byteRate},
BlockAlign{blockAlign},
BitsPerSample{bitsPerSample} {}


static auto b = subchunk_factory::instance().register_type<fmtsubchunk>("fmt ", create_subchunk<fmtsubchunk>());

