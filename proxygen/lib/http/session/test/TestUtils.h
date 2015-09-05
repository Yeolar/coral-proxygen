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

#include <coral/io/async/EventBase.h>
#include <coral/io/async/TimeoutManager.h>
#include <gtest/gtest.h>
#include <proxygen/lib/http/session/HTTPSession.h>
#include <coral/io/async/test/MockAsyncTransport.h>

namespace proxygen {

extern const wangle::TransportInfo mockTransportInfo;
extern const coral::SocketAddress localAddr;
extern const coral::SocketAddress peerAddr;

AsyncTimeoutSet::UniquePtr
makeInternalTimeoutSet(coral::EventBase* evb);

AsyncTimeoutSet::UniquePtr
makeTimeoutSet(coral::EventBase* evb);

testing::NiceMock<coral::test::MockAsyncTransport>*
newMockTransport(coral::EventBase* evb);

}
