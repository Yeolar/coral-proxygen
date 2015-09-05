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

#include <coral/io/IOBuf.h>
#include <ostream>
#include <string>

namespace proxygen {

class IOBufPrinter {
 public:
  enum class Format : uint8_t {
    HEX_CORAL = 0,
    HEX_16 = 1,
    CHAIN_INFO = 2,
    BIN = 3,
  };

  static std::string printChain(const coral::IOBuf* buf,
                                Format format,
                                bool coalesce);

  static std::string printHexCoral(const coral::IOBuf* buf,
                                   bool coalesce=false) {
    return printChain(buf, Format::HEX_CORAL, coalesce);
  }

  static std::string printHex16(const coral::IOBuf* buf, bool coalesce=false) {
    return printChain(buf, Format::HEX_16, coalesce);
  }

  static std::string printChainInfo(const coral::IOBuf* buf) {
    return printChain(buf, Format::CHAIN_INFO, false);
  }

  static std::string printBin(const coral::IOBuf* buf, bool coalesce=false) {
    return printChain(buf, Format::BIN, coalesce);
  }

  IOBufPrinter() {}
  virtual ~IOBufPrinter() {}

  virtual std::string print(const coral::IOBuf* buf) = 0;
};

class Hex16Printer : public IOBufPrinter {
 public:
  std::string print(const coral::IOBuf* buf) override;
};

class HexCoralPrinter : public IOBufPrinter {
 public:
  std::string print(const coral::IOBuf* buf) override;
};

class ChainInfoPrinter : public IOBufPrinter {
 public:
  std::string print(const coral::IOBuf* buf) override;
};

class BinPrinter : public IOBufPrinter {
 public:
  std::string print(const coral::IOBuf* buf) override;
};

/**
 * write the entire binary content from all the buffers into a binary file
 */
void dumpBinToFile(const std::string& filename, const coral::IOBuf* buf);

}
