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
#include <string>
#include <vector>
#include <stdexcept>

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

// Convert a block of bytes to 8-bit sample data
inline uint8_t bytesToSample(const char *b, uint8_t& sample) {
    sample = static_cast<uint8_t>(*b);
    return sample;
}

// Convert a block of bytes to 16-bit sample data
inline int16_t bytesToSample(const char *b, int16_t& sample) {
    sample = static_cast<int16_t>(uint16_t(b[0]) + uint16_t(b[1])*0x0100);
    return sample;
}

// Convert a block of bytes to 32-bit sample data
inline int32_t bytesToSample(const char *b, int32_t& sample) {
    sample = static_cast<int32_t>(uint32_t(b[0]) +
                                  uint32_t(b[1])*0x0100 +
                                  uint32_t(b[2])*0x010000 +
                                  uint32_t(b[2])*0x01000000);
    return sample;
}

// Convert a block of bytes to a vector of sample data
template<typename T>
std::vector<T> from_bytebuffer(const std::string& buffer, std::vector<T>& result) {
    for (auto i=0; i<buffer.size()/sizeof(T); ++i) {
        T sample{};
        bytesToSample(&buffer[i*sizeof(T)], sample);
        result.push_back( sample );
    }
    return result;
}

// Read a file stream into a vector of sample data
//    Assumption: bytes is a whole multiple of sizeof(T)
template<typename T>
std::vector<T> from_stream(std::istream& in, uint32_t bytes, std::vector<T>& result) {
    if ((bytes % sizeof(T)) != 0) {
        throw std::length_error(std::string(__func__) +
                                ": Data size is not a multiple of sample type " +
                                typeid(T).name() + " size!");
    }
    auto data = buffered_read(in, bytes);
    result = from_bytebuffer(data, result);
    return result;
}

#endif
