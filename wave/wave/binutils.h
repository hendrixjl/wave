//
//  binutils.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_binutils_h
#define wave_binutils_h

#include <iostream>

template<typename T>
std::istream& binread(std::istream& in, T& t)
{
    return in.read(reinterpret_cast<char*>(&t), sizeof(T));
}


inline std::string buffered_read(std::istream& in, size_t bytesToRead)
{
    auto buffer = std::string(bytesToRead, '\0');
    in.read(&buffer[0], bytesToRead);
    return buffer;
}

template<typename T>
std::ostream& binwrite(std::ostream& out, const T& t) {
    return out.write(reinterpret_cast<const char*>(&t), sizeof(T));
}


#endif
