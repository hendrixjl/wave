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


std::unique_ptr<subchunk> subchunk_factory::create(std::istream& in)
{
    auto subchkId = std::string(subchunk::SUBCHUNKID_SIZE, ' ');
    in.read(&subchkId[0], subchunk::SUBCHUNKID_SIZE);
    
    if (subchkId == "fmt ")
    {
        return std::make_unique<fmtsubchunk>(in);
    }

    uint32_t chunksize;
    binread(in, chunksize);
    
    auto data = std::string(chunksize, '\0');
    in.read(&data[0], chunksize);
    
    if (subchkId == "FLLR") {
        return std::make_unique<fillrsubchunk>(chunksize, data);
    }
    
    if (subchkId == "data") {
        return std::make_unique<datasubchunk<int16_t>>(chunksize, data);
    }
    
    return nullptr;
}


std::unique_ptr<subchunk> subchunk_factory::make_fixed_subchunk(uint32_t size, std::istream& in) {
    auto subchkId = std::string(subchunk::SUBCHUNKID_SIZE, ' ');
    in.read(&subchkId[0], subchunk::SUBCHUNKID_SIZE);
    std::unordered_map<std::string, creator_t> mymap;
    
    auto i = mymap.find(subchkId);
    if (i != mymap.end())
    {
        return (i->second)(in);
    }
    
    return nullptr;
}

