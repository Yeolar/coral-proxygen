/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include <coral/Random.h>
#include <coral/Range.h>
#include <coral/io/IOBuf.h>
#include <coral/io/Cursor.h>
#include <sys/resource.h>

#ifndef NDEBUG
#define EXPECT_DEATH_NO_CORE(token, regex) {    \
    rlimit oldLim;                              \
    getrlimit(RLIMIT_CORE, &oldLim);            \
    rlimit newLim{0, oldLim.rlim_max};          \
    setrlimit(RLIMIT_CORE, &newLim);            \
      EXPECT_DEATH(token, regex);               \
    setrlimit(RLIMIT_CORE, &oldLim);            \
  }
#else
#define EXPECT_DEATH_NO_CORE(tken, regex) {}
#endif


inline coral::StringPiece
getContainingDirectory(coral::StringPiece input) {
  auto pos = coral::rfind(input, '/');
  if (pos == std::string::npos) {
    pos = 0;
  } else {
    pos += 1;
  }
  return input.subpiece(0, pos);
}
