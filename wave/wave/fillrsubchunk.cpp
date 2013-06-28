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

fillrsubchunk::fillrsubchunk(uint32_t size, const std::string& data)
: SubchunkId("FLLR"), SubchunkSize(size), Data(data) {
}
    
    
std::ostream& fillrsubchunk::textout(std::ostream& out) const {
    out << "SubchunkId=" << SubchunkId;
    out << " SubchunkSize=" << SubchunkSize;
    out << " {" << size() << "}";
    return out;
}
    
std::ostream& fillrsubchunk::binout(std::ostream& out) const {
    out.write(SubchunkId.c_str(), SUBCHUNKID_SIZE);
    binwrite(out, SubchunkSize);
    out.write(Data.c_str(), Data.size());
    return out;
}
