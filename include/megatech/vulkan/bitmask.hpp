#ifndef MEGATECH_VULKAN_BITMASK_HPP
#define MEGATECH_VULKAN_BITMASK_HPP

#include <cinttypes>

#include <concepts>

/**
 * @def MEGATECH_VULKAN_DECLARE_ZERO_BIT
 * @brief Declare a bit constant with a value of 0.
 * @param name The name of the declared constant. Bit constants always have "_bit" appended to the name.
 */
#define MEGATECH_VULKAN_DECLARE_ZERO_BIT(name) \
  constexpr megatech::vulkan::bitmask name##_bit{ 0 };

/**
 * @def MEGATECH_VULKAN_DECLARE_BIT
 * @brief Declare a bit constant with a value of 2^position.
 * @param name The name of the declared constant. Bit constants always have "_bit" appended to the name.
 * @param position The bit position to declare. This must be in the range [0, 63]. This means that 0 is 2^0 (a/k/a 1),
 *                 1 is 2^1 (a/k/a 2), and so on.
 */
#define MEGATECH_VULKAN_DECLARE_BIT(name, position) \
  constexpr megatech::vulkan::bitmask name##_bit{ 1 << position }

namespace megatech::vulkan {

  /**
   * @brief An unsigned 64-bit integral type that only supports bit manipulation operations.
   */
  enum class bitmask : std::uint64_t { };

  /**
   * @brief Convert a bitmask to its underlying integer representation.
   * @tparam Type The desired conversion type (e.g., std::uint64_t).
   * @param mask The bitmask to convert.
   * @return A value of Type with the same bit representation as mask (after possible truncation).
   */
  template <std::integral Type>
  constexpr Type to_integer(const bitmask mask) noexcept {
    return Type{ mask };
  }

  /**
   * @brief Shift a bitmask to the left.
   * @tparam Type An unsigned integral type that represents the shift distance.
   * @param lhs The bitmask to shift.
   * @param rhs The number of places to shift the bitmask by.
   * @return The value of lhs logically shifted left by rhs places.
   */
  template <std::unsigned_integral Type>
  constexpr bitmask operator<<(const bitmask lhs, const Type rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) << rhs };
  }

  /**
   * @brief Shift a bitmask to the right.
   * @tparam Type An unsigned integral type that represents the shift distance.
   * @param lhs The bitmask to shift.
   * @param rhs The number of places to shift the bitmask by.
   * @return The value of lhs logically shifted right by rhs places.
   */
  template <std::unsigned_integral Type>
  constexpr bitmask operator>>(const bitmask lhs, const Type rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) >> rhs };
  }

  /**
   * @brief Shift-assign a bitmask to the left.
   * @tparam Type An unsigned integral type that represents the shift distance.
   * @param lhs The bitmask to shift.
   * @param rhs The number of places to shift the bitmask by.
   * @return A reference to lhs.
   */
  template <std::unsigned_integral Type>
  constexpr bitmask& operator<<=(bitmask& lhs, const Type rhs) noexcept {
    return lhs = lhs << rhs;
  }

  /**
   * @brief Shift-assign a bitmask to the right.
   * @tparam Type An unsigned integral type that represents the shift distance.
   * @param lhs The bitmask to shift.
   * @param rhs The number of places to shift the bitmask by.
   * @return A reference to lhs.
   */
  template <std::unsigned_integral Type>
  constexpr bitmask& operator>>=(bitmask& lhs, const Type rhs) noexcept {
    return lhs = lhs >> rhs;
  }

  /**
   * @brief Perform a bit-wise OR operation between two bitmasks.
   * @param lhs The left hand operand mask.
   * @param rhs The right hand operand mask.
   * @return The value of lhs OR rhs.
   */
  constexpr bitmask operator|(const bitmask lhs, const bitmask rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) | static_cast<std::uint64_t>(rhs) };
  }

  /**
   * @brief Perform a bit-wise AND operation between two bitmasks.
   * @param lhs The left hand operand mask.
   * @param rhs The right hand operand mask.
   * @return The value of lhs AND rhs.
   */
  constexpr bitmask operator&(const bitmask lhs, const bitmask rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) & static_cast<std::uint64_t>(rhs) };
  }

  /**
   * @brief Perform a bit-wise XOR operation between two bitmasks.
   * @param lhs The left hand operand mask.
   * @param rhs The right hand operand mask.
   * @return The value of lhs XOR rhs.
   */
  constexpr bitmask operator^(const bitmask lhs, const bitmask rhs) noexcept {
    return bitmask{ static_cast<std::uint64_t>(lhs) ^ static_cast<std::uint64_t>(rhs) };
  }

  /**
   * @brief OR-assign a bitmask.
   * @param lhs The bitmask being assigned.
   * @param rhs The right hand operand mask.
   * @return A reference to lhs.
   */
  constexpr bitmask& operator|=(bitmask& lhs, const bitmask rhs) noexcept {
    return lhs = lhs | rhs;
  }

  /**
   * @brief AND-assign a bitmask.
   * @param lhs The bitmask being assigned.
   * @param rhs The right hand operand mask.
   * @return A reference to lhs.
   */
  constexpr bitmask& operator&=(bitmask& lhs, const bitmask rhs) noexcept {
    return lhs = lhs & rhs;
  }

  /**
   * @brief XOR-assign a bitmask.
   * @param lhs The bitmask being assigned.
   * @param rhs The right hand operand mask.
   * @return A reference to lhs.
   */
  constexpr bitmask& operator^=(bitmask& lhs, const bitmask rhs) noexcept {
    return lhs = lhs ^ rhs;
  }

}

#endif
