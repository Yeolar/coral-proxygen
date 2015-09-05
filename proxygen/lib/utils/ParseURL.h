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

#include <coral/Conv.h>
#include <coral/String.h>
#include <string>

namespace proxygen {

// ParseURL can handle non-fully-formed URLs. This class must not persist beyond
// the lifetime of the buffer underlying the input StringPiece

class ParseURL {
 public:
  ParseURL() {}
  explicit ParseURL(coral::StringPiece urlVal) noexcept {
    init(urlVal);
  }

  void init(coral::StringPiece urlVal) {
    CHECK(!initialized_);
    url_ = urlVal;
    parse();
    initialized_ = true;
  }

  coral::StringPiece url() const {
    return url_;
  }

  coral::StringPiece scheme() const {
    return scheme_;
  }

  std::string authority() const {
    return authority_;
  }

  bool hasHost() const {
    return valid() && !host_.empty();
  }

  coral::StringPiece host() const {
    return host_;
  }

  uint16_t port() const {
    return port_;
  }

  std::string hostAndPort() const {
    std::string rc = host_.str();
    if (port_ != 0) {
      coral::toAppend(":", port_, &rc);
    }
    return rc;
  }

  coral::StringPiece path() const {
    return path_;
  }

  coral::StringPiece query() const {
    return query_;
  }

  coral::StringPiece fragment() const {
    return fragment_;
  }

  bool valid() const {
    return valid_;
  }

  coral::StringPiece hostNoBrackets() {
    stripBrackets();
    return hostNoBrackets_;
  }

  bool hostIsIPAddress();

  void stripBrackets() noexcept;

 private:
  void parse() noexcept;

  void parseNonFully() noexcept;

  bool parseAuthority() noexcept;

  coral::StringPiece url_;
  coral::StringPiece scheme_;
  std::string authority_;
  coral::StringPiece host_;
  coral::StringPiece hostNoBrackets_;
  coral::StringPiece path_;
  coral::StringPiece query_;
  coral::StringPiece fragment_;
  uint16_t port_{0};
  bool valid_{false};
  bool initialized_{false};
};

}
