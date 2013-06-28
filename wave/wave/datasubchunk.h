//
//  datasubchunk.h
//  wave
//
//  Created by Jonathan Hendrix on 6/27/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef __wave__datasubchunk__
#define __wave__datasubchunk__

#include "subchunk.h"
#include "mymemory.h"
#include <vector>

class datasubchunk : public subchunk // assume 16 bits
{
public:
    using Sample_t=int16_t;
    
    datasubchunk(std::istream& in);
    
    datasubchunk(uint32_t size, const std::vector<Sample_t>& data);
    
    std::unique_ptr<subchunk> clone() const {
        return std::make_unique<datasubchunk>(SubchunkSize, Data);
    }
    
    std::ostream& textout(std::ostream& out) const;
    
    std::ostream& binout(std::ostream& out) const;
    
    uint32_t size() const {
        return SUBCHUNKID_SIZE + sizeof(uint32_t) + SubchunkSize;
    }
private:
    std::string SubchunkId;
    uint32_t SubchunkSize;
    std::vector<Sample_t> Data;
};

#endif /* defined(__wave__datasubchunk__) */
