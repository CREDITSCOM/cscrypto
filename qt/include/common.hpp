#ifndef CSCRYPTO_GUI_COMMON_HPP
#define CSCRYPTO_GUI_COMMON_HPP

#include <vector>
#include <utility>

#include <cscrypto/cscrypto.hpp>

namespace cscrypto {
namespace gui {

using KeyPair = std::pair<cscrypto::PublicKey, cscrypto::PrivateKey>;

} // namespace gui
} // namespace cscrypto
#endif // CSCRYPTO_GUI_COMMON_HPP
