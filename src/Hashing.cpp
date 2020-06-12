#include "Hashing.hpp"

#include <array>
#include <cassert>
#include <stdexcept>

#include <openssl/evp.h>

namespace WayoutPlayer {
static constexpr auto BitsInByte = 8U;

U64 hashString(const std::string &string) {
  const std::string DigestName = "SHA512";
  auto *messageDigestContext = EVP_MD_CTX_new();
  const auto *messageDigest = EVP_get_digestbyname(DigestName.c_str());
  if (messageDigest == nullptr) {
    throw std::runtime_error("Unknown message digest " + DigestName + ".");
  }
  EVP_DigestInit_ex(messageDigestContext, messageDigest, nullptr);
  EVP_DigestUpdate(messageDigestContext, string.c_str(), string.size());
  std::array<unsigned char, EVP_MAX_MD_SIZE> messageDigestValue{};
  unsigned int messageDigestLength = 0;
  EVP_DigestFinal_ex(messageDigestContext, messageDigestValue.data(), &messageDigestLength);
  assert(messageDigestLength >= sizeof(U64));
  EVP_MD_CTX_free(messageDigestContext);
  U64 truncatedDigest = 0;
  for (unsigned int i = 0; i < sizeof(U64); i++) {
    truncatedDigest = (truncatedDigest << BitsInByte) | messageDigestValue[i];
  }
  return truncatedDigest;
}
} // namespace WayoutPlayer