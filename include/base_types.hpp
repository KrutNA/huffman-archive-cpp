#pragma once

#include <cstdint>
#include <cstdlib>

/**
 * Type represents byte = 8 bits
 * which is minimal for most of popular architectures.
 */
typedef uint8_t  u8;

/**
 * Type represents 64 bit values which is most efficient to use
 * on current processors. And mostly enough for any counters
 * and/or data indexes.
 */
typedef uint64_t u64;

/**
 * Type represents size of pointers and max size of any possible object.
 */
typedef size_t usize;

/**
 * Count of bits in normal byte.
 */
const usize BITS_IN_BYTE = 8;

/**
 * Size of byte, normally is 2^8 = 256.
 */
const usize BYTE_SIZE = 1 << BITS_IN_BYTE;

/**
 * Type preresents temporary not full byte.
 */
struct Byte {
  u8 count;
  u8 data;
};
