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

#include <proxygen/lib/http/codec/SPDYCodec.h>
#include <proxygen/lib/http/session/HTTPDownstreamSession.h>
#include <proxygen/lib/http/session/HTTPErrorPage.h>
#include <proxygen/lib/http/session/SimpleController.h>
#include <proxygen/lib/services/HTTPAcceptor.h>
#include <coral/io/async/AsyncSSLSocket.h>

namespace proxygen {

class HTTPSessionStats;

/**
 * Specialization of Acceptor that serves as an abstract base for
 * acceptors that support HTTP and related protocols.
 */
class HTTPSessionAcceptor:
  public HTTPAcceptor,
  private HTTPSession::InfoCallback {
public:
  explicit HTTPSessionAcceptor(const AcceptorConfiguration& accConfig);
  ~HTTPSessionAcceptor() override;

  /**
   * Set the default error page generator.
   */
  void setDefaultErrorPage(std::unique_ptr<HTTPErrorPage> generator) {
    defaultErrorPage_ = std::move(generator);
  }

  /**
   * Access the default error page generator.
   */
  const HTTPErrorPage* getDefaultErrorPage() const {
    return defaultErrorPage_.get();
  }

  /**
   * Set an alternate error page generator to use for internal clients.
   */
  void setDiagnosticErrorPage(std::unique_ptr<HTTPErrorPage> generator) {
    diagnosticErrorPage_ = std::move(generator);
  }

  /**
   * Access the diagnostic error page generator.
   */
  const HTTPErrorPage* getDiagnosticErrorPage() const {
    return diagnosticErrorPage_.get();
  }

  /**
   * Access the right error page generator for a connection.
   * @param   localAddr  Address of the local end of the connection.
   * @return  The diagnostic error page generator if one has been
   *          set AND the connection is to an internal VIP, or
   *          else the default error page generator if one has
   *          been set, or else nullptr.
   */
  virtual const HTTPErrorPage* getErrorPage(
      const coral::SocketAddress& addr) const;

  /**
   * Create a Handler for a new transaction.  The transaction and HTTP message
   * (request) are passed so the implementation can construct different
   * handlers based on these.  The transaction will be explicitly set on the
   * handler later via setTransaction.  The request message will be passed
   * in onHeadersComplete.
   */
  virtual HTTPTransaction::Handler* newHandler(
    HTTPTransaction& txn, HTTPMessage* msg) noexcept = 0;

protected:
  /**
   * This function is invoked when a new session is created to get the
   * controller to associate with the new session. Child classes may
   * override this function to provide their own more sophisticated
   * controller here.
   */
  virtual HTTPSessionController* getController() {
    return &simpleController_;
  }

  HTTPSessionStats* downstreamSessionStats_{nullptr};

  // Acceptor methods
  void onNewConnection(
    coral::AsyncSocket::UniquePtr sock,
    const coral::SocketAddress* address,
    const std::string& nextProtocol,
    SecureTransportType secureTransportType,
    const wangle::TransportInfo& tinfo) override;

  coral::AsyncSocket::UniquePtr makeNewAsyncSocket(coral::EventBase* base,
                                                   int fd) override {
    return coral::AsyncSocket::UniquePtr(
      new coral::AsyncSocket(base, fd));
  }

  virtual size_t dropIdleConnections(size_t num);

private:
  HTTPSessionAcceptor(const HTTPSessionAcceptor&) = delete;
  HTTPSessionAcceptor& operator=(const HTTPSessionAcceptor&) = delete;

  // HTTPSession::InfoCallback methods
  void onCreate(const HTTPSession&) override {}
  void onIngressError(const HTTPSession&, ProxygenError error) override {}
  void onRead(const HTTPSession&, size_t bytesRead) override {}
  void onWrite(const HTTPSession&, size_t bytesWritten) override {}
  void onRequestBegin(const HTTPSession&) override {}
  void onRequestEnd(const HTTPSession&,
                    uint32_t maxIngressQueueSize) override {}
  void onActivateConnection(const HTTPSession&) override {}
  void onDeactivateConnection(const HTTPSession&) override {}
  void onDestroy(const HTTPSession&) override {}
  void onIngressMessage(const HTTPSession&, const HTTPMessage&) override {}
  void onIngressLimitExceeded(const HTTPSession&) override {}
  void onIngressPaused(const HTTPSession&) override {}
  void onTransactionDetached(const HTTPSession&) override {}
  void onPingReplySent(int64_t latency) override {}
  void onPingReplyReceived() override {}
  void onSettingsOutgoingStreamsFull(const HTTPSession&) override {}
  void onSettingsOutgoingStreamsNotFull(const HTTPSession&) override {}

  /** General-case error page generator */
  std::unique_ptr<HTTPErrorPage> defaultErrorPage_;

  /** Generator of more detailed error pages for internal clients */
  std::unique_ptr<HTTPErrorPage> diagnosticErrorPage_;

  coral::Optional<SPDYVersion> alwaysUseSPDYVersion_{};

  SimpleController simpleController_;

  /**
   * 0.0.0.0:0, a valid address to use if getsockname() or getpeername() fails
   */
  static const coral::SocketAddress unknownSocketAddress_;
};

} // proxygen
