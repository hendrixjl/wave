//
//  fmtsubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "fmtsubchunk.h"
#include "subchunk_factory.h"
#include <iostream>

using namespace std;

fmtsubchunk::fmtsubchunk(std::istream& in)
: SubchunkID{"fmt "},
SubchunkSize{binread(in, SubchunkSize)},
AudioFormat{binread(in, AudioFormat)},
NumChannels{binread(in, NumChannels)},
SampleRate{binread(in, SampleRate)},
ByteRate{binread(in, ByteRate)},
BlockAlign{binread(in, BlockAlign)},
BitsPerSample{binread(in, BitsPerSample)}
{
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

std::ostream& fmtsubchunk::textout(std::ostream& out) const {
    out << "SubchunkID=" << SubchunkID;
    out << " SubchunkSize=" << SubchunkSize;
    out << " AudioFormat=" << AudioFormat;
    out << " NumChannels=" << NumChannels;
    out << " SampleRate=" << SampleRate;
    out << " ByteRate=" << ByteRate;
    out << " BlockAlign=" << BlockAlign;
    out << " BitsPerSample=" << BitsPerSample;
    out << " {" << size() << "}";
    return out;
}

std::ostream& fmtsubchunk::binout(std::ostream& out) const {
    out.write(SubchunkID.c_str(), SUBCHUNKID_SIZE);
    binwrite(out, SubchunkSize);
    binwrite(out, AudioFormat);
    binwrite(out, NumChannels);
    binwrite(out, SampleRate);
    binwrite(out, ByteRate);
    binwrite(out, BlockAlign);
    binwrite(out, BitsPerSample);
    return out;
}

static auto b = subchunk_factory::instance().register_type<fmtsubchunk>("fmt ", create_subchunk<fmtsubchunk>());

