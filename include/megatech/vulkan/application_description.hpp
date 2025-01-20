#ifndef MEGATECH_VULKAN_APPLICATION_DESCRIPTION_HPP
#define MEGATECH_VULKAN_APPLICATION_DESCRIPTION_HPP

#include <string>

#include "version.hpp"

namespace megatech::vulkan {

  class application_description final {
  private:
    class version m_version;
    std::string m_name;
  public:
    application_description() = delete;
    application_description(const std::string& name, const class version& ver);
    application_description(const application_description& other) = default;
    application_description(application_description&& other) = delete;

    ~application_description() noexcept = default;

    application_description& operator=(const application_description& rhs) = default;
    application_description& operator=(application_description&& rhs) = default;

    const std::string& name() const;
    const class version& version() const;
  };

}

#endif
