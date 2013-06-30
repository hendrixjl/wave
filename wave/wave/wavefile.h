//
//  wavefile.h
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#ifndef wave_wavefile_h
#define wave_wavefile_h

#include "wavehdr.h"
#include "subchunk.h"
#include <vector>
#include <memory>

class wavefile
{
public:
    wavefile()=default;
    wavefile(wavefile&&)=default;
    
    wavefile(const wavefile& wf);
    wavefile& operator=(const wavefile& wf);
    wavefile(std::istream& in);
    
    std::ostream& textout(std::ostream& out);
    
    std::ostream& binout(std::ostream& out) const;
    
    bool fix(std::istream& in);
    
private:
    wavhdr hdr;
    std::vector<std::unique_ptr<subchunk>> subchunks;
};


#endif
