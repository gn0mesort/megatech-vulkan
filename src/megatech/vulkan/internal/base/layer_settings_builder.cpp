#include "megatech/vulkan/internal/base/layer_settings_builder.hpp"

namespace megatech::vulkan::internal::base {

  layer_settings_builder::layer_settings_builder(const std::string& layer) : m_layer{ layer } {
    m_info.sType = VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT;
  }

  layer_settings_builder::layer_settings_builder(const layer_settings_builder& other) :
  m_layer{ other.m_layer },
  m_info{ other.m_info },
  m_settings{ other.m_settings },
  m_mapping{ other.m_mapping } {
    m_info.pSettings = m_settings.data();
    m_info.settingCount = m_settings.size();
  }

  layer_settings_builder& layer_settings_builder::operator=(const layer_settings_builder& rhs) {
    if (this != &rhs)
    {
      auto cpy = layer_settings_builder{ rhs };
      std::swap(m_layer, cpy.m_layer);
      std::swap(m_info, cpy.m_info);
      std::swap(m_settings, cpy.m_settings);
      std::swap(m_mapping, cpy.m_mapping);
      m_info.pSettings = m_settings.data();
      m_info.settingCount = m_settings.size();
    }
    return *this;
  }

  void layer_settings_builder::set_next(const void *const next) {
    m_info.pNext = next;
  }

  const VkLayerSettingsCreateInfoEXT& layer_settings_builder::build() const {
    return m_info;
  }

}
