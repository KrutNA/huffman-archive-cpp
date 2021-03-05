#pragma once

#include "base_types.hpp"

#include <istream>
#include <variant>
#include <optional>
#include <memory>
#include <list>
#include <array>
#include <algorithm>


class Node;
struct Data;

/**
 * Using shared pointer because of moving and sharing this data
 * while accessing to Nodes.
 */
typedef std::shared_ptr<Node> NodeRef;

/**
 * Using `pair` data type instead of creating structure
 * for better usability.
 */
typedef std::pair<NodeRef, NodeRef> PairNode;

/**
 * `variant` is Modern C++ style for representing something
 * like Rust's or Swift's Enums.
 * Instead of old C-style: enums + unions.
 */
typedef std::variant<Data, PairNode> NodeHolder;

/**
 * Type represents weight of each byte where:
 * - array index is byte value (2^8 = 256);
 * - array value is byte weight.
 *
 * Used pre-initialized array for high performance because of access
 * to each element on every data byte. So 1KiB mememory is worth.
 * Instead of search of element in map or any other container on
 * each update to container. 
 */
typedef std::array<u64, BYTE_SIZE> Buffer;

/**
 * Type represents information of count of specified char.
 * - `data` is byte of data;
 * - `weight` is count of symbols represented in provided data.
 */
struct Data {
  /**
   * Byte of data.
   */
  u8 data;
  /**
   * Count of symbols represented in provided data.
   */
  u64 weight = 1;
};


/**
 * Type represents tree structure when `Node` could be
 * - Leaf (`Data`);
 * - Pair of 2 nodes/
 */
class Node {
  /**
   * Holder of node data.
   */
  NodeHolder holder;

public:
  /**
   * Basic constructor for 
   */
  Node(NodeHolder holder)
    : holder(holder) {};

  /**
   * @return data if holder contains `Data`.
   */
  std::optional<Data>     getData()  const;
  
  /**
   * @return pair of Node if holder contains `PairNode`.
   */
  std::optional<PairNode> getPair()  const;
  
  /**
   * @return first Node of pair if holder contains `PairNode`.
   */
  std::optional<NodeRef>  getLeft()  const;
  
  /**
   * @return second Node of pair if holder contains `PairNode`.
   */
  std::optional<NodeRef>  getRight() const;

  /**
   * Caculates weight based on holder, if holder:
   * - `Data` then returns value of `weight` field;
   * - `PairNode` then returns sum of calculated weights.
   * @return calculated weight of Node.
   */
  u64 getWeight() const;

  /**
   * Struct contains some conversions of holder insights to `Node`'s holder.
   */
  struct Holder {
    /**
     * Boxes `data` into `NodeHolder`.
     * @param data passed `Data`.
     * @return `NodeHolder` with boxed value.
     */
    static inline NodeHolder fromData(const Data &data);

    /**
     * Boxes 2 Nodes into `pair` and after that into `NodeHolder`.
     * @param left element which will be `first` in `pair`.
     * @param right element which will be `second` in `pair`.
     * @return `NodeHolder` with boxed values.
     */
    static inline NodeHolder fromRefs(const NodeRef &left,
				      const NodeRef &right);
    
    /**
     * Boxes `pair` of `Node`s into `NodeHolder`.
     * @param pair passed `PairNode`.
     * @return `NodeHolder` with boxed value.
     */
    static inline NodeHolder fromPair(const PairNode &pair);
  };

  virtual ~Node();

  /**
   * Compares weight of nodes.
   */
  friend bool operator< (const Node &lhs, const Node &rhs) {
    return lhs.getWeight() < rhs.getWeight();
  }
  friend bool operator> (const Node &lhs, const Node &rhs) { return rhs < lhs; }
  friend bool operator<=(const Node &lhs, const Node &rhs) { return !(lhs > rhs); }
  friend bool operator>=(const Node &lhs, const Node &rhs) { return !(lhs < rhs); }
};

/**
 * Reads data from stream as byte stream and saves information
 * about bytes into data buffer.
 * @param stream provided stream of bytes.
 * @param data buffer which contains info about symbols.
 */
void update_with_buf(std::ifstream &stream, Buffer &data);

/**
 * Converts `Buffer` into single `Node`.
 *
 * Skips all bytes with no weight (value = 0).
 */
Node generate_tree(const Buffer &data);
