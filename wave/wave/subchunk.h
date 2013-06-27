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
#include <iostream>
#include <stdint.h>

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

inline std::ostream& operator<<(std::ostream& out, const subchunk& o) {
    o.textout(out);
    return out;
}

#endif
