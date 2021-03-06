//
//  subchunk_factory.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "mymemory.h"
#include "subchunk_factory.h"
#include "fmtsubchunk.h" // temporary
#include "fillrsubchunk.h" // temporary
#include "datasubchunk.h" // temporary
#include "binutils.h"
#include <iostream>
using namespace std;

void subchunk_factory::announce(const std::string& s) const
{
    cout << "registering factory for " << s << endl;
}

std::unique_ptr<subchunk> subchunk_factory::create(std::istream& in, uint16_t numChannels, uint16_t bitsPerSample)
{
    auto subchkId = buffered_read(in, subchunk::SUBCHUNKID_SIZE);

    if (subchkId == "data") {
        subchkId = FormattedDataSubchunkId(numChannels, bitsPerSample);
    }

    auto i = mymap.find(subchkId);
    if (i == mymap.end())
    {
        throw domain_error(string{"chunk id "} + subchkId + " Not found!");
    }

    return (i->second)(in);
}


std::unique_ptr<subchunk> subchunk_factory::make_fixed_subchunk(uint32_t size, std::istream& in, uint16_t numChannels, uint16_t bitsPerSample) {
    auto subchkId = buffered_read(in, subchunk::SUBCHUNKID_SIZE);
    
    if (subchkId == "data") {
        subchkId = FormattedDataSubchunkId(numChannels, bitsPerSample);
    }
    
    std::unordered_map<std::string, creator_t> mymap;
    
    auto i = mymap.find(subchkId);
    if (i != mymap.end())
    {
        return (i->second)(in);
    }
    
    return nullptr;
}

