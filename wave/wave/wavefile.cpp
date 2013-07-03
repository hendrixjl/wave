//
//  wavefile.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/27/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "wavefile.h"
#include "binutils.h"
#include "subchunk_factory.h"
#include "fmtsubchunk.h"
#include <iostream>

using namespace std;


wavefile::wavefile(const wavefile& wf) 
: hdr{wf.hdr},
numChannels{wf.numChannels},
bitsPerSample{wf.bitsPerSample},
sampleRate{wf.sampleRate}
{
    for (const auto&  sc : wf.subchunks) {
        subchunks.push_back( sc->clone() );
    }
}

wavefile& wavefile::operator=(const wavefile& wf) {
    hdr = wf.hdr;
    numChannels = wf.numChannels;
    bitsPerSample = wf.bitsPerSample;
    sampleRate = wf.sampleRate;
    for (const auto&  sc : wf.subchunks) {
        subchunks.push_back( sc->clone() );
    }
    return *this;
}

wavefile::wavefile(istream& in)
: hdr{in}, subchunks{} {
    auto bytes_left = hdr.filesize() - hdr.size();
    while (bytes_left > 0) {
        auto subchnk = subchunk_factory::instance().create(in, numChannels, bitsPerSample);
        extract_format_data(*subchnk);
        bytes_left -= subchnk->size();
        subchunks.push_back(std::move(subchnk));
    }
    
    if (numChannels == 0) {
        throw domain_error(string{__func__} + ": No format subchunk in file!");
    }
}

void wavefile::extract_format_data(const subchunk& sc) {
    const fmtsubchunk* fmt = dynamic_cast<const fmtsubchunk*>(&sc);
    if (fmt != nullptr) {
        numChannels = fmt->numChannels();
        bitsPerSample = fmt->bitsPerSample();
        sampleRate = fmt->sampleRate();
    }
}

std::ostream& wavefile::textout(std::ostream& out) {
    out << hdr << std::endl;
    for (const auto& d : subchunks) {
        out << *d << std::endl;
    }
    return out;
}

std::ostream& wavefile::binout(std::ostream& out) const {
    hdr.binout(out);
    for (const auto& d : subchunks) {
        d->binout(out);
    }
    return out;
}

bool wavefile::fix(std::istream& in) {
    hdr = wavhdr(in);
    auto bleft = hdr.filesize() - hdr.size();
    while (bleft > 0) {
        auto subchnk = subchunk_factory::instance().make_fixed_subchunk(bleft, in, bitsPerSample, numChannels);
        extract_format_data(*subchnk);
        bleft -= subchnk->size();
        subchunks.push_back(std::move(subchnk));
    }
    
    if (numChannels == 0) {
        cout << "No format subchunk!";
        return false;
    }
    return true;
}
