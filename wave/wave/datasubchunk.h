//
//  datasubchunk.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_datasubchunk_h
#define wave_datasubchunk_h

#include "subchunk.h"
#include "mymemory.h"
#include <vector>

template<typename Sample_t>
class datasubchunk : public subchunk // assume 16 bits
{
public:
    datasubchunk(std::istream& in);
    
    datasubchunk(uint32_t size, const std::vector<int16_t>& data);
    
    std::unique_ptr<subchunk> clone() const {
        return std::make_unique<datasubchunk>(SubchunkSize, Data);
    }
    
    std::ostream& textout(std::ostream& out) const {
        out << "SubchunkId=" << SubchunkId;
        out << " SubchunkSize=" << SubchunkSize;
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


#endif
