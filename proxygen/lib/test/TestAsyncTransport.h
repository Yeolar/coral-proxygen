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

#include <deque>
#include <coral/SocketAddress.h>
#include <coral/io/IOBufQueue.h>
#include <coral/io/async/AsyncTimeout.h>
#include <proxygen/lib/utils/Time.h>
#include <coral/io/async/AsyncTransport.h>

class TestAsyncTransport : public coral::AsyncTransportWrapper,
                           private coral::AsyncTimeout {
 public:
  class WriteEvent {
   public:
    static std::shared_ptr<WriteEvent> newEvent(const struct iovec* vec,
                                                  size_t count);

    proxygen::TimePoint getTime() const {
      return time_;
    }
    const struct iovec* getIoVec() const {
      return vec_;
    }
    size_t getCount() const {
      return count_;
    }

   private:
    static void destroyEvent(WriteEvent* event);

    WriteEvent(proxygen::TimePoint time, size_t count);
    ~WriteEvent();

    proxygen::TimePoint time_;
    size_t count_;
    struct iovec vec_[];
  };

  explicit TestAsyncTransport(coral::EventBase* eventBase);

  // AsyncTransport methods
  void setReadCB(AsyncTransportWrapper::ReadCallback* callback) override;
  ReadCallback* getReadCallback() const override;
  void write(AsyncTransportWrapper::WriteCallback* callback,
             const void* buf, size_t bytes,
             coral::WriteFlags flags =
             coral::WriteFlags::NONE) override;
  void writev(AsyncTransportWrapper::WriteCallback* callback,
              const struct iovec* vec, size_t count,
              coral::WriteFlags flags =
              coral::WriteFlags::NONE) override;
  void writeChain(AsyncTransportWrapper::WriteCallback* callback,
                  std::unique_ptr<coral::IOBuf>&& iob,
                  coral::WriteFlags flags =
                  coral::WriteFlags::NONE) override;
  void close() override;
  void closeNow() override;
  void shutdownWrite() override;
  void shutdownWriteNow() override;
  void getPeerAddress(coral::SocketAddress* addr)
    const override;
  void getLocalAddress(coral::SocketAddress* addr)
    const override;
  bool good() const override;
  bool readable() const override;
  bool connecting() const override;
  bool error() const override;
  void attachEventBase(coral::EventBase* eventBase) override;
  void detachEventBase() override;
  bool isDetachable() const override;
  coral::EventBase* getEventBase() const override;
  void setSendTimeout(uint32_t milliseconds) override;
  uint32_t getSendTimeout() const override;

  // Methods to control read events
  void addReadEvent(const void* buf, size_t buflen,
                    std::chrono::milliseconds delayFromPrevious);
  void addReadEvent(coral::IOBufQueue& chain,
                    std::chrono::milliseconds delayFromPrevious);
  void addReadEvent(const char* buf,
                    std::chrono::milliseconds delayFromPrevious);
  void addReadEOF(std::chrono::milliseconds delayFromPrevious);
  void addReadError(const coral::AsyncSocketException& ex,
                    std::chrono::milliseconds delayFromPrevious);
  void startReadEvents();

  void pauseWrites();
  void resumeWrites();

  // Methods to get the data written to this transport
  std::deque< std::shared_ptr<WriteEvent> >* getWriteEvents() {
    return &writeEvents_;
  }

  uint32_t getEORCount() {
    return eorCount_;
  }

  uint32_t getCorkCount() {
    return corkCount_;
  }

  size_t getAppBytesWritten() const override { return 0; }
  size_t getRawBytesWritten() const override { return 0; }
  size_t getAppBytesReceived() const override { return 0; }
  size_t getRawBytesReceived() const override { return 0; }
  bool isEorTrackingEnabled() const override { return false; }
  void setEorTracking(bool) override { return; }

 private:
  enum StateEnum {
    kStateOpen,
    kStatePaused,
    kStateClosed,
    kStateError,
  };

  class ReadEvent;

  bool writesAllowed() const { return writeState_ == kStateOpen ||
      writeState_ == kStatePaused; }

  // Forbidden copy constructor and assignment opererator
  TestAsyncTransport(TestAsyncTransport const&);
  TestAsyncTransport& operator=(TestAsyncTransport const&);

  void addReadEvent(const std::shared_ptr<ReadEvent>& event);
  void scheduleNextReadEvent(proxygen::TimePoint now);
  void fireNextReadEvent();
  void fireOneReadEvent();
  void failPendingWrites();

  // AsyncTimeout methods
  void timeoutExpired() noexcept override;

  coral::EventBase* eventBase_;
  coral::AsyncTransportWrapper::ReadCallback* readCallback_;
  uint32_t sendTimeout_;

  proxygen::TimePoint prevReadEventTime_{};
  proxygen::TimePoint nextReadEventTime_{};
  StateEnum readState_;
  StateEnum writeState_;
  std::deque< std::shared_ptr<ReadEvent> > readEvents_;
  std::deque< std::shared_ptr<WriteEvent> > writeEvents_;
  std::deque< std::pair<std::shared_ptr<WriteEvent>, AsyncTransportWrapper::WriteCallback*>>
    pendingWriteEvents_;

  uint32_t eorCount_{0};
  uint32_t corkCount_{0};
};
