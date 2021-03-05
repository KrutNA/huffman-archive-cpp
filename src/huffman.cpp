#include "huffman.hpp"

template<typename T>
inline const std::optional<T> get_holder(const NodeHolder &holder) {
  return std::holds_alternative<T>(holder)
    // if contains value of type T then returns it
    ? std::get<T>(holder)
    // overwise returns empty
    : std::optional<T>();
}

std::optional<Data> Node::getData() const {
  return get_holder<Data>(this->holder);;
}

std::optional<PairNode> Node::getPair() const {
  return get_holder<PairNode>(this->holder);
}

std::optional<NodeRef> Node::getLeft() const {
  if (auto pair = get_holder<PairNode>(this->holder)) {
    return pair->first;
  }
  return { };
}

std::optional<NodeRef> Node::getRight() const {
  if (auto pair = get_holder<PairNode>(this->holder)) {
    return pair->second;
  }
  return { };
}



u64 Node::getWeight() const {
  if (auto data = this->getData()) {

    return data->weight;

  } else {

    return getLeft().value()->getWeight()
      + getRight().value()->getWeight();

  }
}

Node::~Node() {}



inline NodeHolder Node::Holder::fromData(const Data &data) {
  return data;
}

inline NodeHolder Node::Holder::fromRefs(const NodeRef &left,
					 const NodeRef &right) {
  return fromPair({ left, right });
}

inline NodeHolder Node::Holder::fromPair(const PairNode &pair) {
  return pair;
}



void update_with_buf(Buffer &data, std::basic_istream<u8> &stream) {
  while (!stream.eof()) {
    ++data[stream.get()];
  }
}


/**
 * Removes minimal value from provided node list.
 * @return removed minimal element.
 */
inline Node& remove_minimal(std::list<Node> &nodes) {
  auto min = std::min_element(nodes.begin(), nodes.end());
  nodes.erase(min);
  return *min;
}

/**
 * Removes 2 minimal values from list and makes node
 * from them which saves into same list.
 */
void update_with_minimal(std::list<Node> &nodes) {

  auto &min1 = remove_minimal(nodes);
  auto &min2 = remove_minimal(nodes);

  nodes.push_back({
      Node::Holder::fromRefs(std::make_shared<Node>(min1),
			     std::make_shared<Node>(min2))
    });
}

Node generate_tree(const Buffer &data) {
  std::list<Node> nodes;

  for (u8 i = 0; i < data.size(); ++i) {
    // Skip elements with no weight (value of array = 0)
    if (data[i] != 0) {
      // Create data from every non empty element.
      // As mentioned in `Buffer`: index is byte and value is weght.
      nodes.push_back({
	  Node::Holder::fromData({
	      .data = i,
	      .weight = data[i]
	    })
	});
    }
  }

  // In any situatuation if 2 elements converts into 1 on iteration
  // here will be initiale size - 1 iterations before 1 element reached.
  for (auto i = 0; i < nodes.size() - 1; ++i) {
    update_with_minimal(nodes);
  }
  
  return nodes.front();
}

