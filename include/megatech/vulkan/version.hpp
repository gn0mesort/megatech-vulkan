#ifndef MEGATECH_VULKAN_VERSION_HPP
#define MEGATECH_VULKAN_VERSION_HPP

#include <cinttypes>

#include <compare>

namespace megatech::vulkan {

  class version final {
  private:
    std::uint32_t m_data{ };
  public:
    static constexpr std::uint_least8_t max_variant() {
      return 0x7;
    }

    static constexpr std::uint_least8_t max_major_version() {
      return 0x7f;
    }

    static constexpr std::uint_least16_t max_minor_version() {
      return 0x3ff;
    }

    static constexpr std::uint_least16_t max_patch_version() {
      return 0xfff;
    }

    explicit version(const std::uint32_t data) noexcept;
    version(const std::uint_least8_t variant, const std::uint_least8_t major, const std::uint_least16_t minor,
            const std::uint_least16_t patch);
    version(const version& other) = default;
    version(version&& other) = default;

    ~version() noexcept = default;

    version& operator=(const version& rhs) = default;
    version& operator=(version&& rhs) = default;
    std::strong_ordering operator<=>(const version& rhs) const noexcept;
    bool operator==(const version& rhs) const = default;

    explicit operator std::uint32_t() const noexcept;

    std::uint_least8_t variant() const noexcept;
    std::uint_least8_t major() const noexcept;
    std::uint_least16_t minor() const noexcept;
    std::uint_least16_t patch() const noexcept;

  };

}

#endif
