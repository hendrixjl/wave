//
//  subchunk.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_subchunk_h
#define wave_subchunk_h

/**
 * After the header "chunk", wave files consist of subchunks.
 * This is the base class for all subchunks.
 */

#include "binutils.h"
#include <iostream>
#include <stdint.h>

class subchunk
{
public:
    enum { SUBCHUNKID_SIZE=4 };
    virtual ~subchunk() {}
    
    /**
     * @brief Clone this subchunk.
     * @return An identical copy of the subchunk.
     */
    virtual std::unique_ptr<subchunk> clone() const=0;
    
    /**
     * @brief Provide a human readable description of the subchunk.
     * @param out - an ostream
     * @return std::ostream&
     */
    virtual std::ostream& textout(std::ostream& out) const=0;
    
    /**
     * @brief Serialize the subchunk to an ostream.
     * @note The data is serialized in a form correct for a wave file.
     * @param out - an ostream
     * @return std::ostream
     */
    virtual std::ostream& binout(std::ostream& out) const=0;
    
    /**
     * @brief Return the number of bytes in this subchunk.
     * @return The number of bytes in the subchunk.
     */
    virtual uint32_t size() const=0;
};

/**
 * @brief Overload operator<< for text output of a subchunk.
 * @param out - an ostream
 * @param o - a subchunk
 * @return std::ostream&
 */
inline std::ostream& operator<<(std::ostream& out, const subchunk& o) {
    o.textout(out);
    return out;
}

#endif
