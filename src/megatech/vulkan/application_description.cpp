#include "megatech/vulkan/application_description.hpp"

namespace megatech::vulkan {

  application_description::application_description(const std::string& name, const class version& ver) :
  m_version{ ver },
  m_name{ name } { }

  const std::string& application_description::name() const {
    return m_name;
  }

  const class version& application_description::version() const {
    return m_version;
  }

}
