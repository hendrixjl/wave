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

/**
 * @brief Read "raw" bytes from an istream directly into
 *        a variable.
 * @param in - The istream
 * @param t - The variable to read into
 * @return The value read
 */
template<typename T>
T binread(std::istream& in, T& t)
{
    in.read(reinterpret_cast<char*>(&t), sizeof(T));
    return t;
}


/**
 * @brief Read raw bytes from an istream into a buffer.
 * @note - Using a string as a handy managered buffer type
 * @param in - The istream
 * @param bytesToRead - The number of bytes to read.
 * @return A string containing the data.
 */
inline std::string buffered_read(std::istream& in, size_t bytesToRead)
{
    auto buffer = std::string(bytesToRead, '\0');
    in.read(&buffer[0], bytesToRead);
    return buffer;
}


/**
 * @brief Write "binary" data to an ostream.
 * @note The output data is written "raw", without formatting.
 * @param out - The ostream
 * @param t - The output variable
 */
template<typename T>
std::ostream& binwrite(std::ostream& out, const T& t) {
    return out.write(reinterpret_cast<const char*>(&t), sizeof(T));
}

/**
 * @brief Encode the number of channels and bits per sample into the Id
 * for a data subchunk.
 * @param numChannels
 * @param bitsPerSample
 * @return a std::string
 */
inline std::string FormattedDataSubchunkId(uint16_t numChannels, uint16_t bitsPerSample) {
    return std::string{"data_"} + std::to_string(numChannels) + "_" + std::to_string(bitsPerSample);
}

#endif
