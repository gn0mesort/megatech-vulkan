/**
 * @file application_description.cpp
 * @brief Client Application Descriptions
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
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
