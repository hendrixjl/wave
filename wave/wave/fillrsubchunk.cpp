//
//  fillrsubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "fillrsubchunk.h"
#include "subchunk_factory.h"
#include <iostream>
using namespace std;

static auto b = subchunk_factory::instance().register_type<fillrsubchunk>("FLLR", create_subchunk<fillrsubchunk>());

fillrsubchunk::fillrsubchunk(std::istream& in) : SubchunkId("FLLR") {
    binread(in, SubchunkSize);
    Data = std::string(SubchunkSize, ' ');
    in.read(&Data[0], SubchunkSize);
}
