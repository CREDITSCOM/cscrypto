#ifndef CSCRYPTO_GUI_COMMON_HPP
#define CSCRYPTO_GUI_COMMON_HPP

#include <cstddef>
#include <utility>
#include <vector>

#include <cscrypto/cscrypto.hpp>

namespace cscrypto {
namespace gui {

using KeyPair = cscrypto::keys_derivation::KeyPair;

static const size_t kMinPswdSize = 3;

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_GUI_COMMON_HPP
