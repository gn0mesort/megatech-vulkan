#ifndef MEGATECH_VULKAN_ERROR_HPP
#define MEGATECH_VULKAN_ERROR_HPP

#include <cinttypes>

#include <exception>
#include <source_location>
#include <string>
#include <sstream>

#if defined(MEGATECH_VULKAN_INCLUDE_VULKAN_H) && __has_include(<vulkan/vulkan.h>)
  #include <vulkan/vulkan.h>
#endif

namespace megatech::vulkan {

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
    error(const std::string& message, const std::source_location& location = std::source_location::current());
#ifdef VK_VERSION_1_0
    error(const std::string& message, const VkResult vk_result,
          const std::source_location& location = std::source_location::current());
#endif
    error(const error& other) = default;
    error(error&& other) = default;

    virtual ~error() noexcept = default;

    error& operator=(const error& rhs) = default;
    error& operator=(error&& rhs) = default;

    virtual const std::string& message() const noexcept;
    virtual const char* what() const noexcept;
    virtual const std::source_location& location() const noexcept;
    virtual constexpr std::uint32_t result_code() const noexcept {
      return 0x01'00'00'01;
    }
#ifdef VK_VERSION_1_0
    virtual VkResult vk_result() const noexcept;
#endif
  };

}

#endif
