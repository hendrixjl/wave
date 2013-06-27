//
//  wavefile.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_wavefile_h
#define wave_wavefile_h

#include "binutils.h"
#include "subchunk_factory.h"
#include <string>
#include <stdint.h>
#include <vector>
#include <iostream>

class wavefile
{
public:
    wavefile()=default;
    wavefile(wavefile&&)=default;
    
    wavefile(const wavefile& wf) : hdr{wf.hdr} {
        for (const auto&  sc : wf.subchunks) {
            subchunks.push_back( sc->clone() );
        }
    }
    
    wavefile(std::istream& in)
    : hdr{in}, subchunks{} {
        auto bleft = hdr.filesize() - hdr.size();
        while (bleft > 0) {
            auto subchnk = subchunk_factory::instance().create(in);
            bleft -= subchnk->size();
            subchunks.push_back(std::unique_ptr<subchunk>(subchnk.release()));
        }
    }
    
    std::ostream& textout(std::ostream& out) {
        out << hdr << std::endl;
        for (const auto& d : subchunks) {
            out << *d << std::endl;
        }
        return out;
    }
    
    std::ostream& binout(std::ostream& out) const {
        hdr.binout(out);
        for (const auto& d : subchunks) {
            d->binout(out);
        }
        return out;
    }
    
    bool fix(std::istream& in) {
        hdr = wavhdr(in);
        auto bleft = hdr.filesize() - hdr.size();
        while (bleft > 0) {
            auto subchnk = subchunk_factory::instance().make_fixed_subchunk(bleft, in);
            bleft -= subchnk->size();
            subchunks.push_back(std::unique_ptr<subchunk>(subchnk.release()));
        }
        return true;
    }
private:
    wavhdr hdr;
    std::vector<std::unique_ptr<subchunk>> subchunks;
};


#endif
