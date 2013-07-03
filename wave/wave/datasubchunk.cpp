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

enum {
    MONO = 1,
    STERIO = 2,
    BITS_PER_SAMPLE_8 = 8,
    BITS_PER_SAMPLE_16 = 16
};

REGISTER_SUBCHUNK_TYPE( datasubchunk<uint8_t>, FormattedDataSubchunkId(MONO, BITS_PER_SAMPLE_8) )

REGISTER_SUBCHUNK_TYPE( datasubchunk<int16_t>, FormattedDataSubchunkId(MONO, BITS_PER_SAMPLE_16) )


