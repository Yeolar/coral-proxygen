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

#include <coral/io/async/TimeoutManager.h>
#include <chrono>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <glog/logging.h>

namespace proxygen {

class MockTimeoutManager : public coral::TimeoutManager {
 public:

  MOCK_METHOD2(
      attachTimeoutManager,
      void(coral::AsyncTimeout*, coral::TimeoutManager::InternalEnum));

  MOCK_METHOD1(detachTimeoutManager, void(coral::AsyncTimeout*));

  MOCK_METHOD2(
      scheduleTimeout,
      bool(coral::AsyncTimeout*, std::chrono::milliseconds));

  MOCK_METHOD1(cancelTimeout, void(coral::AsyncTimeout*));

  MOCK_METHOD0(bumpHandlingTime, bool());
  MOCK_METHOD0(isInTimeoutManagerThread, bool());
};

} // proxygen
