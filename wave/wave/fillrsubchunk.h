//
//  fillrsubchunk.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef __wave__fillrsubchunk__
#define __wave__fillrsubchunk__

#include "subchunk.h"
#include "mymemory.h"

class fillrsubchunk : public subchunk
{
public:
    fillrsubchunk(std::istream& in) : SubchunkId("FLLR") {
        binread(in, SubchunkSize);
        Data = std::string{' ', SubchunkSize};
        in.read(&Data[0], SubchunkSize);
    }
    
    fillrsubchunk(uint32_t size, const std::string& data) : SubchunkId("FLLR"), SubchunkSize(size), Data(data) {
    }
    
    std::unique_ptr<subchunk> clone() const {
        return std::make_unique<fillrsubchunk>(SubchunkSize, Data);
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

#endif /* defined(__wave__fillrsubchunk__) */
