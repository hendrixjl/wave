//
//  fmtsubchunk.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef __wave__fmtsubchunk__
#define __wave__fmtsubchunk__

#include "subchunk.h"
#include "mymemory.h"
#include <iostream>
#include <string>
#include <cstdint>

class fmtsubchunk : public subchunk
{
public:

    enum class AudioFormat_t : uint16_t {
        PCM
    };

    fmtsubchunk(std::istream& in);
    
    fmtsubchunk(uint32_t size,
                AudioFormat_t audioFormat,
                uint16_t numChannels,
                uint32_t sampleRate,
                uint32_t byteRate,
                uint16_t blockAlign,
                uint16_t bitsPerSample);
    
    std::unique_ptr<subchunk> clone() const {
        return std::make_unique<fmtsubchunk>(SubchunkSize,
                                        AudioFormat,
                                        NumChannels,
                                        SampleRate,
                                        ByteRate,
                                        BlockAlign,
                                        BitsPerSample);
    }
    
    std::ostream& textout(std::ostream& out) const;
    
    std::ostream& binout(std::ostream& out) const;
    
    uint32_t size() const {
        return SUBCHUNKID_SIZE + sizeof(uint32_t) + SubchunkSize;
    }
    
    AudioFormat_t audioFormat() const {
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
    AudioFormat_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
};

#endif /* defined(__wave__fmtsubchunk__) */
