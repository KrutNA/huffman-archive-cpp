#include "base_types.hpp"
#include "binary_map.hpp"

#include <istream>
#include <ostream>

/**
 * Encodes specified stream with data into buffer stream.
 * @param data input stream with data.
 * @param stream for encoded output.
 * @param map array of new bytes mapped to old bytes.
 * @return count of bytes in input stream which are encoded.
 */
u64 encode(std::istream &data,
	   std::ostream &buffer,
	   BinaryMap &map);

/**
 * Decodes specified stream with data into buffer stream.
 * @param data input stream with data.
 * @param stream for encoded output.
 * @param tree elements encoded.
 * @return count of bytes in input stream which are encoded.
 */
u64 decode(std::istream &data,
	   std::ostream &buffer,
	   BinaryMap &map);
