//
//  datasubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "datasubchunk.h"

#include "subchunk_factory.h"

static auto b = subchunk_factory::instance().register_type<datasubchunk<uint16_t>>("fmt ", create_subchunk<datasubchunk>());


inline uint8_t bytesToSample(const char *b, uint8_t& sample) {
    sample = static_cast<uint8_t>(*b);
    return sample;
}

inline int16_t bytesToSample(const char *b, int16_t& sample) {
    sample = static_cast<int16_t>(uint16_t(b[0]) + uint16_t(b[1])*0x0100);
    return sample;
}


datasubchunk::datasubchunk(std::istream& in)
: SubchunkId{"data"}
{
    binread(in, SubchunkSize);
    auto data = std::string(chunksize, '\0');
    in.read(&data[0], chunksize);
    for (auto i=0; i<size/sizeof(Sample_t); ++i) {
        Sample_t sample{};
        bytesToSample(&data[i/sizeof(Sample_t)], sample);
        Data.push_back( sample );
    }   
}

datasubchunk::datasubchunk(uint32_t size, const std::vector<int16_t>& data)
: SubchunkId{"data"}, SubchunkSize{size}, Data{data} {}
