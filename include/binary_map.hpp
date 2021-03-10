#pragma once

#include "base_types.hpp"
#include "huffman.hpp"

#include <array>
#include <memory>
#include <vector>

struct Binary;

/**
 * Shared pointer for not storing empty elements.
 * nullptr in this case. So less memory usage.
 */
typedef std::shared_ptr<Binary> BinaryRef;

/**
 * About 2KiB unused memory in worst case for high performance.
 * In this case selected array again instead of map or any other.
 * - Array index is normal byte representation.
 * - Array value is new byte representation
 */
typedef std::array<BinaryRef, BYTE_SIZE> BinaryMap;

/**
 * Type represents data for storing new archived data.
 */
struct Binary {
  /**
   * Count of bits in this archive representation.
   */
  u64 size;
  /**
   * Bytes for storing new data.
   */
  std::vector<u8> bytes;
};

/**
 * Converts tree of nodes into plain map of data.
 */
BinaryMap map_from_tree(const Node &node);
