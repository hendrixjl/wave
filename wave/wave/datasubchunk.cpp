//
//  datasubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/27/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "datasubchunk.h"
#include "subchunk_factory.h"
#include "binutils.h
#include <iostream>

using namespace std;

static auto b = subchunk_factory::instance().register_type<datasubchunk>("data", create_subchunk<datasubchunk>());


uint8_t bytesToSample(const char *b, uint8_t& sample) {
    sample = static_cast<uint8_t>(*b);
    return sample;
}

int16_t bytesToSample(const char *b, int16_t& sample) {
    sample = static_cast<int16_t>(uint16_t(b[0]) + uint16_t(b[1])*0x0100);
    return sample;
}


datasubchunk::datasubchunk(std::istream& in)
: SubchunkId{"data"}
{
    binread(in, SubchunkSize);
    auto data = buffered_read(in, SubchunkSize);
    for (auto i=0; i<SubchunkSize/sizeof(Sample_t); ++i) {
        Sample_t sample{};
        bytesToSample(&data[i*sizeof(Sample_t)], sample);
        Data.push_back( sample );
    }
}

datasubchunk::datasubchunk(uint32_t size, const std::vector<Sample_t>& data)
: SubchunkId{"data"}, SubchunkSize{size}, Data{data} {}

std::ostream& datasubchunk::textout(std::ostream& out) const {
    out << "SubchunkId=" << SubchunkId;
    out << " SubchunkSize=" << SubchunkSize;
    out << " {" << size() << "} ";
    if (!Data.empty()) {
        // for (const auto& d : Data) {
        // out << " " << d;
        // }
        out << " " << Data[0] << "... for " << Data.size() << " samples";
    }
    return out;
}
    
std::ostream& datasubchunk::binout(std::ostream& out) const {
    out.write(SubchunkId.c_str(), SUBCHUNKID_SIZE);
    binwrite(out, SubchunkSize);
    for (auto d : Data) {
        binwrite(out, d); // @TODO make portable
    }
    return out;
}
