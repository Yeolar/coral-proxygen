/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <proxygen/lib/http/codec/HTTPParallelCodec.h>

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <coral/Conv.h>
#include <coral/Memory.h>
#include <coral/ScopeGuard.h>
#include <coral/String.h>
#include <coral/io/Cursor.h>
#include <glog/logging.h>
#include <iostream>
#include <proxygen/lib/http/HTTPHeaderSize.h>
#include <proxygen/lib/http/HTTPMessage.h>
#include <proxygen/lib/http/codec/CodecDictionaries.h>

namespace proxygen {

HTTPParallelCodec::HTTPParallelCodec(TransportDirection direction)
  : transportDirection_(direction),
    sessionClosing_(ClosingState::OPEN) {
  switch (transportDirection_) {
  case TransportDirection::DOWNSTREAM:
    nextEgressStreamID_ = 2;
    break;
  case TransportDirection::UPSTREAM:
    nextEgressStreamID_ = 1;
    break;
  default:
    LOG(FATAL) << "Unknown transport direction.";
  }
}

HTTPCodec::StreamID HTTPParallelCodec::createStream() {
  auto ret = nextEgressStreamID_;
  nextEgressStreamID_ += 2;
  return ret;
}

bool HTTPParallelCodec::isWaitingToDrain() const {
  return sessionClosing_ == ClosingState::OPEN ||
    sessionClosing_ == ClosingState::FIRST_GOAWAY_SENT;
}

bool HTTPParallelCodec::isReusable() const {
  return (sessionClosing_ == ClosingState::OPEN ||
          sessionClosing_ == ClosingState::OPEN_WITH_GRACEFUL_DRAIN_ENABLED ||
          (transportDirection_ == TransportDirection::DOWNSTREAM &&
           isWaitingToDrain()))
    && (ingressGoawayAck_ == std::numeric_limits<uint32_t>::max())
    && (nextEgressStreamID_ <= std::numeric_limits<int32_t>::max() - 2);
}
}
