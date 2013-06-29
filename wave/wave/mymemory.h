//
//  mymemory.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_mymemory_h
#define wave_mymemory_h

#include <memory>

namespace std
{
    template <typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&&... args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}

#endif
