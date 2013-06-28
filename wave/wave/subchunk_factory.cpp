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

#include <iostream>
using namespace std;

void subchunk_factory::announce(const std::string& s) const
{
    cout << "registering factory for " << s << endl;
}

std::unique_ptr<subchunk> subchunk_factory::create(std::istream& in)
{
    auto subchkId = buffered_read(in, subchunk::SUBCHUNKID_SIZE;

    cout << "create chunk for id=" << subchkId << endl;
    
    auto i = mymap.find(subchkId);
    if (i != mymap.end())
    {
        return (i->second)(in);
    }
    else
    {
        cout << "Not found!" << endl;
    }

    return nullptr;
}


std::unique_ptr<subchunk> subchunk_factory::make_fixed_subchunk(uint32_t size, std::istream& in) {
    auto subchkId = buffered_read(in, subchunk::SUBCHUNKID_SIZE);
    
    std::unordered_map<std::string, creator_t> mymap;
    
    auto i = mymap.find(subchkId);
    if (i != mymap.end())
    {
        return (i->second)(in);
    }
    
    return nullptr;
}

