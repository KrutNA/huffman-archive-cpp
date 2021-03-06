#include "archive.hpp"
#include "base_types.hpp"

constexpr std::array<u8, 8> BYTES {
  0x01, 0x03, 0x07, 0x0F,
  0x1F, 0x3F, 0x7F, 0xFF,
};

u64 encode(std::istream &data,
	    std::ostream &buffer,
	    BinaryMap &map) {
  Byte last;
  u64 written = 0;
  
  while (!data.eof()) {
    u8 byte = data.get();
    ++written;

    // Select data by byte
    auto binary = map[byte];

    // Count of bits to be writed with current symbol
    u8 lasts = binary->size - last.count;

    usize index = 0;

    // While exists more than BITS_IN_BYTE bits
    while (lasts > BITS_IN_BYTE) {
      u8 tmp = last.data;

      // Shift previous data
      tmp <<= (BITS_IN_BYTE - last.count);

      // Add part of current byte
      tmp |= binary->bytes[index] >> last.count;

      // Write byte to stream
      buffer << tmp;

      // Update last with binary
      last.data = binary->bytes[index] & BYTES[last.count];

      ++index;
      lasts -= BITS_IN_BYTE;
    }
    
    // Here is 0 <= lasts < 8 bits
    // lasts = N + K where
    // - N is last.count
    // - K is bits in last element of binary (K = binary.size % BITS_IN_BYTE)
    auto k = binary->size % BITS_IN_BYTE;

    // Shift data by K
    last.data <<= k;

    // Add K bytes
    last.data |= (binary->bytes.back() >> k) & BYTES[k];

    // Setup count in last
    last.count = lasts;
  }

  // Write last byte into stream if i exists
  if (last.count != 0) {
    buffer << (last.data << (BITS_IN_BYTE - last.count));
    ++written;
  }

  return written;
}
