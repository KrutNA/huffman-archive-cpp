#include "binary_map.hpp"

Binary from_bits(std::vector<u8> &bits) {
  Binary binary { .size = bits.size() };

  for (auto i = 0; i < bits.size(); ++i) {
    if (i % BITS_IN_BYTE == 0) {
      binary.bytes.push_back(0);
    }
    binary.bytes.back() |= bits[i] << (BITS_IN_BYTE - 1 - (i % BITS_IN_BYTE));
  }

  return binary;
}

void node_to_map(const Node &node,
		 BinaryMap &map,
		 std::vector<u8> &bits) {
  if (auto data = node.getData()) {

    map[data->data] = std::make_shared<Binary>(from_bits(bits));

  } else {

    bits.push_back(1);
    node_to_map(*node.getLeft().value(), map, bits);
    bits.pop_back();

    bits.push_back(0);
    node_to_map(*node.getRight().value(), map, bits);
    bits.pop_back();

  }
}

BinaryMap map_from_tree(const Node &node) {
  BinaryMap map;
  std::vector<u8> bits;

  node_to_map(node, map, bits);

  return map;
}
