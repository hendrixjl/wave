//
//  fillrsubchunk.cpp
//  wave
//
//  Created by Jonathan Hendrix on 6/26/13.
//  Copyright (c) 2013 Jonathan Hendrix. All rights reserved.
//

#include "fillrsubchunk.h"

#include "subchunk_factory.h"

static auto b = subchunk_factory::instance().register_type<fillrsubchunk>("fmt ", create_subchunk<fillrsubchunk>());
