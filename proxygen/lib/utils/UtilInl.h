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

#include <coral/Range.h>

namespace proxygen {

// Case-insensitive string comparison
inline bool caseInsensitiveEqual(coral::StringPiece s, coral::StringPiece t) {
  if (s.size() != t.size()) {
    return false;
  }
  return std::equal(s.begin(), s.end(), t.begin(), coral::asciiCaseInsensitive);
}

}
