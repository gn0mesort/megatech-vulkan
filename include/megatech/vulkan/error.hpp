/**
 * @file error.hpp
 * @brief Vulkan Errors
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_ERROR_HPP
#define MEGATECH_VULKAN_ERROR_HPP

#include <cinttypes>

#include <exception>
#include <source_location>
#include <string>

namespace megatech::vulkan {

  /**
   * @brief A Megatech-Vulkan error.
   * @details This can be an error specifically generated by Megatech-Vulkan, or it can be an error generated from
   *          the Vulkan API directly.
   */
  class error : public std::exception {
  private:
    // Since Vulkan is not included by default, errors need a way to store VkResult values without changing their
    // run-time structure. It's safe to use std::int_least16_t (a/k/a int) for this, but VkResult could exceed that
    // range. Therefore, std::int_least64_t (a/k/a long long int) is the safest future-proof type here.
    //
    // -13 is VK_ERROR_UNKNOWN.
    std::int_least64_t m_vk_result{ -13 };
    std::string m_message{ };
    std::source_location m_location{ };
  public:
    /**
     * @brief Construct an error.
     * @param message A message associated with the root cause of the error.
     * @param location The source location at which the error occurred. This defaults to the current source location.
     */
    error(const std::string& message, const std::source_location& location = std::source_location::current());

    /**
     * @brief Construct an error.
     * @param message A message associated with the root cause of the error.
     * @param vk_result An integer representing a Vulkan API error.
     * @param location The source location at which the error occurred. This defaults to the current source location.
     */
    error(const std::string& message, std::int_least64_t vk_result,
          const std::source_location& location = std::source_location::current());

    /**
     * Copy an error.
     * @param other The error to copy.
     */
    error(const error& other) = default;

    /**
     * @brief Move an error.
     * @param other The error to move.
     */
    error(error&& other) = default;

    /**
     * @brief Destroy an error.
     */
    virtual ~error() noexcept = default;

    /**
     * Copy-assign an error.
     * @param rhs The error to copy.
     * @return A reference to the copied-to error.
     */
    error& operator=(const error& rhs) = default;

    /**
     * Move-assign an error.
     * @param rhs The error to move.
     * @return A reference to the moved-to error.
     */
    error& operator=(error&& rhs) = default;

    /**
     * @brief Retrieve the message associated with an error.
     * @return A message describing the cause of the error.
     */
    virtual const std::string& message() const noexcept;

    /**
     * @brief Retrieve the message associated with an error.
     * @details This is equivalent to err.message().data(). The standard C++ API uses this accessor.
     * @return A message describing the cause of the error.
     */
    virtual const char* what() const noexcept;

    /**
     * @brief Retrieve the source location that generated an error.
     * @return A source_location object, indicating where the error occurred.
     */
    virtual const std::source_location& location() const noexcept;

    /**
     * @brief Retrieve a result code representing an error.
     * @details This is useful if, for example, you want to return an error code from main(). Different systems handle
     *          main's return value differently. Some systems (e.g., Linux) don't return the entire int value. As a
     *          result, this value should always end in a non-zero byte.
     * @return An unsigned 32-bit integer result code.
     */
    virtual constexpr std::uint32_t result_code() const noexcept {
      return 0x01'00'00'01;
    }

    /**
     * @brief Retrieve a Vulkan specific result code representing an error.
     * @details This may or may not be useful, depending on how the error was generated. Errors emitted by the Vulkan
     *          API should, generally, carry their VkResult value. Library errors should, generally, have a value of
     *          VK_ERROR_UNKNOWN (a/k/a -13). However, library errors may choose to provide a VkResult if appropriate.
     * @return A signed integer representing an underlying Vulkan API error.
     */
    virtual std::int_least64_t vk_result() const noexcept;
  };

}

#endif
