#ifndef MEGATECH_VULKAN_BITMASK_HPP
#define MEGATECH_VULKAN_BITMASK_HPP

#include <cinttypes>

#include <concepts>

#define MEGATECH_VULKAN_DECLARE_ZERO_BIT(name) \
  constexpr megatech::vulkan::bitmask name##_bit{ 0 };

#define MEGATECH_VULKAN_DECLARE_BIT(name, position) \
  constexpr megatech::vulkan::bitmask name##_bit{ 1 << position }

namespace megatech::vulkan {

  enum class bitmask : std::uint64_t { };

  template <std::integral Type>
  constexpr Type to_integer(const bitmask mask) noexcept {
    return Type{ mask };
  }

  template <std::unsigned_integral Type>
  constexpr bitmask operator<<(const bitmask lhs, const Type rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) << rhs };
  }

  template <std::unsigned_integral Type>
  constexpr bitmask operator>>(const bitmask lhs, const Type rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) >> rhs };
  }

  template <std::unsigned_integral Type>
  constexpr bitmask& operator<<=(bitmask& lhs, const Type rhs) noexcept {
    return lhs = lhs << rhs;
  }

  template <std::unsigned_integral Type>
  constexpr bitmask& operator>>=(bitmask& lhs, const Type rhs) noexcept {
    return lhs = lhs >> rhs;
  }

  constexpr bitmask operator|(const bitmask lhs, const bitmask rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) | static_cast<std::uint64_t>(rhs) };
  }

  constexpr bitmask operator&(const bitmask lhs, const bitmask rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) & static_cast<std::uint64_t>(rhs) };
  }

  constexpr bitmask operator^(const bitmask lhs, const bitmask rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) ^ static_cast<std::uint64_t>(rhs) };
  }

  constexpr bitmask& operator|=(bitmask& lhs, const bitmask rhs) noexcept {
    return lhs = lhs | rhs;
  }

  constexpr bitmask& operator&=(bitmask& lhs, const bitmask rhs) noexcept {
    return lhs = lhs & rhs;
  }

  constexpr bitmask& operator^=(bitmask& lhs, const bitmask rhs) noexcept {
    return lhs = lhs ^ rhs;
  }

}

#endif
