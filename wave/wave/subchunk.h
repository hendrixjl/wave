//
//  subchunk.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_subchunk_h
#define wave_subchunk_h

#include "binutils.h"
#include <string>
#include <stdint.h>
#include <vector>
#include <iomanip>

class subchunk
{
public:
    enum { SUBCHUNKID_SIZE=4 };
    virtual ~subchunk() {}
    virtual std::unique_ptr<subchunk> clone() const=0;
    virtual std::ostream& textout(std::ostream& out) const=0;
    virtual std::ostream& binout(std::ostream& out) const=0;
    virtual uint32_t size() const=0;
};

template <typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&&... args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}


inline std::ostream& operator<<(std::ostream& out, const subchunk& o) {
    o.textout(out);
    return out;
}

class fmtsubchunk : public subchunk
{
public:
        
    fmtsubchunk(uint32_t size,
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

    std::unique_ptr<subchunk> clone() const {
        return make_unique<fmtsubchunk>(SubchunkSize,
                                        AudioFormat,
                                        NumChannels,
                                        SampleRate,
                                        ByteRate,
                                        BlockAlign,
                                        BitsPerSample);
    }
    
    std::ostream& textout(std::ostream& out) const {
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
    
    std::ostream& binout(std::ostream& out) const {
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
    
    uint32_t size() const {
        return SUBCHUNKID_SIZE + sizeof(uint32_t) + SubchunkSize;
    }
    
    uint16_t audioFormat() const {
        return AudioFormat;
    }
    
    uint16_t numChannels() const {
        return NumChannels;
    }
    
    uint32_t sampleRate() const{
        return SampleRate;
    }
    
    uint32_t byteRate() const {
        return ByteRate;
    }
    
    uint16_t blockAlign() const {
        return BlockAlign;
    }
    
    uint16_t bitsPerSample() const {
        return BitsPerSample;
    }
    
private:
    std::string SubchunkID;
    uint32_t SubchunkSize;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
};



class fillrsubchunk : public subchunk
{
public:
    
    fillrsubchunk(uint32_t size, const std::string& data) : SubchunkId("FLLR"), SubchunkSize(size), Data(data) {
    }
    
    std::unique_ptr<subchunk> clone() const {
        return make_unique<fillrsubchunk>(SubchunkSize, Data);
    }
    
    std::ostream& textout(std::ostream& out) const {
        out << "SubchunkId=" << SubchunkId;
        out << " SubchunkSize=" << SubchunkSize;
        out << " {" << size() << "}";
        return out;
    }
    
    std::ostream& binout(std::ostream& out) const {
        out.write(SubchunkId.c_str(), SUBCHUNKID_SIZE);
        binwrite(out, SubchunkSize);
        out.write(Data.c_str(), Data.size());
        return out;
    }
    
    uint32_t size() const {
        return SUBCHUNKID_SIZE + sizeof(uint32_t) + SubchunkSize;
    }
private:
    std::string SubchunkId;
    uint32_t SubchunkSize;
    std::string Data;
};

uint8_t bytesToSample(const char *b, uint8_t& sample) {
    sample = static_cast<uint8_t>(*b);
    return sample;
}

int16_t bytesToSample(const char *b, int16_t& sample) {
    sample = static_cast<int16_t>(uint16_t(b[0]) + uint16_t(b[1])*0x0100);
    return sample;
}


template<typename Sample_t>
class datasubchunk : public subchunk // assume 16 bits
{
public:
        
    datasubchunk(uint32_t size, const std::string& data) : SubchunkId{"data"}, SubchunkSize{size} {
        for (auto i=0; i<size/sizeof(Sample_t); ++i) {
            Sample_t sample{};
            bytesToSample(&data[i/sizeof(Sample_t)], sample);
            Data.push_back( sample );
        }
    }
    
    datasubchunk(uint32_t size, const std::vector<int16_t>& data) : SubchunkId{"data"}, SubchunkSize{size}, Data{data} {}

    
    std::unique_ptr<subchunk> clone() const {
        return make_unique<datasubchunk>(SubchunkSize, Data);
    }

    std::ostream& textout(std::ostream& out) const {
        out << "Subchunk2Id=" << SubchunkId;
        out << " Subchunk2Size=" << SubchunkSize;
        out << " {" << size() << "} ";
        if (!Data.empty()) {
//            for (const auto& d : Data) {
//                out << " " << d;
//            }
            out << " " << Data[0] << "... for " << Data.size() << " samples";
        }
        return out;
    }
    
    std::ostream& binout(std::ostream& out) const {
        out.write(SubchunkId.c_str(), SUBCHUNKID_SIZE);
        binwrite(out, SubchunkSize);
        for (auto d : Data) {
            binwrite(out, d); // @TODO make portable
        }
        return out;
    }
    
    uint32_t size() const {
        return SUBCHUNKID_SIZE + sizeof(uint32_t) + SubchunkSize;
    }
private:
    std::string SubchunkId;
    uint32_t SubchunkSize;
    std::vector<Sample_t> Data;
};


inline std::unique_ptr<subchunk> make_subchunk(std::istream& in) {
    auto subchkId = std::string(subchunk::SUBCHUNKID_SIZE, ' ');
    in.read(&subchkId[0], subchunk::SUBCHUNKID_SIZE);
    uint32_t chunksize;
    binread(in, chunksize);
    
    if (subchkId == "fmt ")
    {
        uint16_t AudioFormat;
        uint16_t NumChannels;
        uint32_t SampleRate;
        uint32_t ByteRate;
        uint16_t BlockAlign;
        uint16_t BitsPerSample;
        binread(in, AudioFormat);
        binread(in, NumChannels);
        binread(in, SampleRate);
        binread(in, ByteRate);
        binread(in, BlockAlign);
        binread(in, BitsPerSample);
        return make_unique<fmtsubchunk>(chunksize, AudioFormat, NumChannels,
                                        SampleRate, ByteRate, BlockAlign, BitsPerSample);
    }
    
    auto data = std::string(chunksize, '\0');
    in.read(&data[0], chunksize);
    
    if (subchkId == "FLLR") {
        return make_unique<fillrsubchunk>(chunksize, data);
    }
    
    if (subchkId == "data") {
        return make_unique<datasubchunk<int16_t>>(chunksize, data);
    }
    
    return nullptr;
}

inline std::unique_ptr<subchunk> make_fixed_subchunk(uint32_t size, std::istream& in) {
    auto subchkId = std::string(subchunk::SUBCHUNKID_SIZE, ' ');
    in.read(&subchkId[0], subchunk::SUBCHUNKID_SIZE);
    uint32_t chunksize;
    binread(in, chunksize);
    
    if (chunksize == 0) {
        chunksize = size;
    }
    
    if (subchkId == "fmt ")
    {
        uint16_t AudioFormat;
        uint16_t NumChannels;
        uint32_t SampleRate;
        uint32_t ByteRate;
        uint16_t BlockAlign;
        uint16_t BitsPerSample;
        binread(in, AudioFormat);
        binread(in, NumChannels);
        binread(in, SampleRate);
        binread(in, ByteRate);
        binread(in, BlockAlign);
        binread(in, BitsPerSample);
        return make_unique<fmtsubchunk>(chunksize, AudioFormat, NumChannels,
                                        SampleRate, ByteRate, BlockAlign, BitsPerSample);
    }
    
    auto data = std::string(chunksize, '\0');
    in.read(&data[0], chunksize);
    
    if (subchkId == "FLLR") {
        return make_unique<fillrsubchunk>(chunksize, data);
    }
    
    if (subchkId == "data") {
        return make_unique<datasubchunk<int16_t>>(chunksize, data);
    }
    
    return nullptr;
}


#endif
