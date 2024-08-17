#ifndef MEGATECH_VULKAN_ERROR_HPP
#define MEGATECH_VULKAN_ERROR_HPP
#include <cinttypes>

#include <exception>
#include <source_location>
#include <string>
#include <sstream>

namespace megatech::vulkan {

  class error : public std::exception {
  private:
    std::string m_message{ };
    std::source_location m_location{ };
  public:
    error(const std::string& message, const std::source_location& location = std::source_location::current());
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
  };

}

#endif
