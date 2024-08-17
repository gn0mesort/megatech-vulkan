#include "megatech/vulkan/error.hpp"

namespace megatech::vulkan {

  error::error(const std::string& message, const std::source_location& location) :
  m_message{ message }, m_location{ location } { }

  const std::string& error::message() const noexcept {
    return m_message;
  }

  const char* error::what() const noexcept {
    return m_message.data();
  }

  const std::source_location& error::location() const noexcept {
    return m_location;
  }

}
