/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <proxygen/lib/http/codec/HTTPCodecPrinter.h>

namespace proxygen {

void HTTPCodecPrinter::onFrameHeader(
    uint32_t stream_id,
    uint8_t flags,
    uint32_t length,
    uint16_t version) {
  switch (call_->getProtocol()) {
    case CodecProtocol::SPDY_2:
    case CodecProtocol::SPDY_3:
    case CodecProtocol::SPDY_3_1:
    case CodecProtocol::SPDY_3_1_HPACK:
      if (version > 0) {
        // Print frame header info of SPDY control frames
        std::cout << "[CTRL FRAME] version=" << version << ", flags="
                  <<  std::hex << coral::to<unsigned int>(flags) << std::dec
                  << ", length=" << length << std::endl;
      } else {
        // Print frame header info of SPDY data frames and HTTP/2 frames
        std::cout << "[DATA FRAME] stream_id=" << stream_id << ", flags="
                  << std::hex << coral::to<unsigned int>(flags) << std::dec
                  << ", length=" << length << std::endl;
      }
      break;
    case CodecProtocol::HTTP_2:
      std::cout << "[FRAME] stream_id=" << stream_id << ", flags="
                << std::hex << coral::to<unsigned int>(flags) << std::dec
                << ", length=" << length << std::endl;
      break;
    case CodecProtocol::HTTP_1_1:
    default:
      break;
  }
  callback_->onFrameHeader(stream_id, flags, length, version);
}

void HTTPCodecPrinter::onError(StreamID stream,
                               const HTTPException& error,
                               bool newStream) {
  std::cout << "[Exception] " << error.what() << std::endl;
  callback_->onError(stream, error, newStream);
}

void HTTPCodecPrinter::onBody(StreamID stream,
                              std::unique_ptr<coral::IOBuf> chain,
                              uint16_t padding) {
  std::cout << "DataChunk: stream_id=" << stream
            << ", length=" << chain->length()
            << ", padding=" << padding << std::endl;
  callback_->onBody(stream, std::move(chain), padding);
}

void HTTPCodecPrinter::onMessageComplete(StreamID stream, bool upgrade) {
  std::cout << "DataComplete: stream_id=" << stream << std::endl;
  callback_->onMessageComplete(stream, upgrade);
}

void HTTPCodecPrinter::onHeadersComplete(
    StreamID stream,
    std::unique_ptr<HTTPMessage> msg) {
  std::cout << "HEADERS: stream_id=" << stream
            << ", numHeaders=" << msg->getHeaders().size() << std::endl;
  msg->getHeaders().forEach([&] (
        const std::string& header,
        const std::string& val) {
      std::cout << "\t" << header << ": " << val << std::endl;
  });
  callback_->onHeadersComplete(stream, std::move(msg));
}

void HTTPCodecPrinter::onAbort(StreamID stream, ErrorCode code) {
  std::cout << "RST_STREAM: stream_id=" << stream << ", error="
            << getErrorCodeString(code) << std::endl;
  callback_->onAbort(stream, code);
}

void HTTPCodecPrinter::onGoaway(uint64_t lastGoodStream, ErrorCode code) {
  std::cout << "GOAWAY: lastGoodStream=" << lastGoodStream
            << ", error=" << getErrorCodeString(code) << std::endl;
  callback_->onGoaway(lastGoodStream, code);
}

void HTTPCodecPrinter::onWindowUpdate(StreamID stream, uint32_t amount) {
  std::cout << "WINDOW_UPDATE: stream_id=" << stream
            << ", delta_window_size=" << amount << std::endl;
  callback_->onWindowUpdate(stream, amount);
}

void HTTPCodecPrinter::onSettings(const SettingsList& settings) {
  std::cout << "SETTINGS: num=" << settings.size() << std::endl;
  for (const auto& setting: settings) {
    std::cout << "\tid=" << coral::to<uint8_t>(setting.id)
              << ", value=" << setting.value << std::endl;
  }
  callback_->onSettings(settings);
}

void HTTPCodecPrinter::onSettingsAck() {
  std::cout << "SETTINGS_ACK" << std::endl;
  callback_->onSettingsAck();
}

void HTTPCodecPrinter::onPingRequest(uint64_t unique_id) {
  printPing(unique_id);
  callback_->onPingRequest(unique_id);
}

void HTTPCodecPrinter::onPingReply(uint64_t unique_id) {
  printPing(unique_id);
  callback_->onPingReply(unique_id);
}

void HTTPCodecPrinter::printPing(uint64_t unique_id) {
  std::cout << "PING: unique_id=" << unique_id << std::endl;
}

}
