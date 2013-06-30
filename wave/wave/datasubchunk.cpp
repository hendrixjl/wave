//
//  datasubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/27/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "datasubchunk.h"
#include "subchunk_factory.h"
#include "binutils.h"
#include <iostream>
#include <stdexcept>

using namespace std;

static auto b = subchunk_factory::instance().register_type<datasubchunk>("data", create_subchunk<datasubchunk>());

// Convert a block of bytes to 8-bit sample data
uint8_t bytesToSample(const char *b, uint8_t& sample) {
    sample = static_cast<uint8_t>(*b);
    return sample;
}

// Convert a block of bytes to 16-bit sample data
int16_t bytesToSample(const char *b, int16_t& sample) {
    sample = static_cast<int16_t>(uint16_t(b[0]) + uint16_t(b[1])*0x0100);
    return sample;
}

// Convert a block of bytes to 32-bit sample data
int32_t bytesToSample(const char *b, int32_t& sample) {
    sample = static_cast<int32_t>(uint32_t(b[0]) +
                                  uint32_t(b[1])*0x0100 +
                                  uint32_t(b[2])*0x010000 +
                                  uint32_t(b[2])*0x01000000);
    return sample;
}

// Convert a block of bytes to a vector of sample data
template<typename T>
vector<T> from_bytebuffer(const string& buffer, vector<T>& result) {
    for (auto i=0; i<buffer.size()/sizeof(T); ++i) {
        T sample{};
        bytesToSample(&buffer[i*sizeof(T)], sample);
        result.push_back( sample );
    }
    return result;
}

// Read a file stream into a vector of sample data
//    Assumption: bytes is a whole multiple of sizeof(T)
template<typename T>
vector<T> from_stream(istream& in, uint32_t bytes, vector<T>& result) {
    if ((bytes % sizeof(T)) != 0) {
        throw length_error(string(__func__) +
                       ": Data size is not a multiple of sample type " +
                       typeid(T).name() + " size!");
    }
    auto data = buffered_read(in, bytes);
    result = from_bytebuffer(data, result);
    return result;
}

datasubchunk::datasubchunk(std::istream& in)
: SubchunkId{"data"},
SubchunkSize{binread(in,SubchunkSize)},
Data{from_stream(in, SubchunkSize, Data)}
{
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
