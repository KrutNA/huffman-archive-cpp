#include "archive.hpp"

constexpr std::array<u8, 8> BYTES {
  0x01, 0x02, 0x04, 0x08,
  0x10, 0x20, 0x40, 0x80,
};

const u8 MAX_BITS = BYTES.size() - 1;

u64 decode(std::istream &data,
	   std::ostream &buffer,
	   NodeRef node) {
  Byte last;
  u64 written = 0;

  while (!data.eof()) {

    auto &tmp_node = node;

    // While we have pair node lookup
    while (auto pair = node->getPair()) {

      // Setup if byte finished or not initialized
      if (last.count == 0) {
	last.data = data.get();
	last.count = MAX_BITS;
      }
      
      // Select node by bit (1 - left, 0 - right)
      tmp_node = (last.data & BYTES[last.count])
	? pair->first
	: pair->second;

      --last.count;
    }

    // Here we have data in `tmp_node`
    buffer << tmp_node->getData()->data;

  }

  return written;
}
