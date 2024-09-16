#include "megatech/vulkan/version.hpp"

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include "megatech/vulkan/error.hpp"

namespace megatech::vulkan {

  version::version(const std::uint32_t data) noexcept : m_data{ data } { }

  version::version(const std::uint_least8_t variant, const std::uint_least8_t major, const std::uint_least16_t minor,
                   const std::uint_least16_t patch) {
    if (variant & ~max_variant())
    {
      throw error{ "The variant cannot exceed 7." };
    }
    if (major & ~max_major_version())
    {
      throw error{ "The major version cannot exceed 127." };
    }
    if (minor & ~max_minor_version())
    {
      throw error{ "The minor version cannot exceed 1023." };
    }
    if (patch & ~max_patch_version())
    {
      throw error{ "The patch version cannot exceed 4095." };
    }
    m_data = VK_MAKE_API_VERSION(variant, major, minor, patch);
  }

  std::strong_ordering version::operator<=>(const version& rhs) const noexcept {
    return m_data <=> rhs.m_data;
  }

  version::operator std::uint32_t() const noexcept {
    return m_data;
  }

  std::uint_least8_t version::variant() const noexcept {
    return VK_API_VERSION_VARIANT(m_data);
  }

  std::uint_least8_t version::major() const noexcept {
    return VK_API_VERSION_MAJOR(m_data);
  }

  std::uint_least16_t version::minor() const noexcept {
    return VK_API_VERSION_MINOR(m_data);
  }

  std::uint_least16_t version::patch() const noexcept {
    return VK_API_VERSION_PATCH(m_data);
  }

}
