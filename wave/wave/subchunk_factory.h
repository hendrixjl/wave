//
//  subchunk_factory.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef __wave__subchunk_factory__
#define __wave__subchunk_factory__

#include "subchunk.h"
#include "mymemory.h"
#include <string>
#include <iostream>
#include <functional>
#include <unordered_map>

class subchunk_factory
{
public:
    static subchunk_factory& instance() {
        static subchunk_factory factory;
        return factory;
    }
    
    using creator_t = std::function<std::unique_ptr<subchunk>(std::istream&)>;
    
    template<typename T>
    bool register_type(const std::string& name,  creator_t create_function) {
        announce(name);
        mymap.insert(std::make_pair(name, create_function));
        return true;
    }
    
    std::unique_ptr<subchunk> create(std::istream& in);
    
    std::unique_ptr<subchunk> make_fixed_subchunk(uint32_t size, std::istream& in);
    
private:
    subchunk_factory()=default;
    subchunk_factory(const subchunk_factory&)=delete;
    
    void announce(const std::string& s) const;
    
    std::unordered_map<std::string, creator_t> mymap;
};


template<typename T>
class create_subchunk
{
public:
    std::unique_ptr<subchunk> operator()(std::istream& in)
    {
        return std::make_unique<T>(in);
    }
private:
};


#endif /* defined(__wave__subchunk_factory__) */
