#pragma once

#include "dout.h"

#define DOUT tt::util::debug::dout::instance()
#define DBLOCK(name) DOUT << tt::util::debug::block{name}
#define DENDBLOCK DOUT << tt::util::debug::end_block{}
#define DEND tt::util::debug::end_row{}