#ifndef MEGATECH_VULKAN_INTERNAL_BASE_LAYER_SETTINGS_BUILDER_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_LAYER_SETTINGS_BUILDER_HPP

#include <cinttypes>

#include <vector>
#include <string>
#include <unordered_map>
#include <concepts>

#include "../../concepts/builder_object.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan::concepts {

  template <typename Type>
  concept layer_setting_type = std::same_as<Type, VkBool32> || std::same_as<Type, std::int32_t> ||
                               std::same_as<Type, std::int64_t> || std::same_as<Type, std::uint32_t> ||
                               std::same_as<Type, std::uint64_t> || std::same_as<Type, float> ||
                               std::same_as<Type, double> || std::same_as<Type, const char*>;

}

namespace megatech::vulkan::internal::base {

  class layer_settings_builder final {
  private:
    std::string m_layer{ };
    VkLayerSettingsCreateInfoEXT m_info{ };
    std::vector<VkLayerSettingEXT> m_settings{ };
    std::unordered_map<std::string, std::size_t> m_mapping{ };
  public:
    layer_settings_builder() = delete;
    explicit layer_settings_builder(const std::string& layer);
    layer_settings_builder(const layer_settings_builder& other);
    layer_settings_builder(layer_settings_builder&& other) = default;

    ~layer_settings_builder() noexcept = default;

    layer_settings_builder& operator=(const layer_settings_builder& rhs);
    layer_settings_builder& operator=(layer_settings_builder&& rhs) = default;

    template <concepts::layer_setting_type Type>
    void set(const char *const name, const Type *const value);
    template <concepts::layer_setting_type Type>
    void set(const char *const name, const Type *const values, const std::uint32_t count);
    void set_next(const void *const);
    const VkLayerSettingsCreateInfoEXT& build() const;
  };

  template <concepts::layer_setting_type Type>
  void layer_settings_builder::set(const char *const name, const Type *const value) {
    set(name, value, 1);
  }

  template <concepts::layer_setting_type Type>
  void layer_settings_builder::set(const char *const name, const Type *const values, const std::uint32_t count) {
    auto setting = static_cast<VkLayerSettingEXT*>(nullptr);
    if (m_mapping.contains(name))
    {
      setting = &m_settings[m_mapping.at(name)];
    }
    else
    {
      m_mapping[name] = m_settings.size();
      m_settings.push_back({ });
      setting = &m_settings.back();
      m_info.pSettings = m_settings.data();
      m_info.settingCount = m_settings.size();
    }
    setting->pLayerName = m_layer.data();
    setting->pSettingName = name;
    // Some compile-time devil-worship. I don't think there is a constexpr switch, so I have to do this instead.
    // The alternative is 8 explicit specializations of this method. In practice, combined with
    // concepts::layer_settings_type, this provides a guarantee that Type is a valid setting type and setting.type
    // always has the correct enum value.
    if constexpr (std::same_as<Type, VkBool32>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_BOOL32_EXT;
    }
    else if constexpr (std::same_as<Type, std::int32_t>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_INT32_EXT;
    }
    else if constexpr (std::same_as<Type, std::int64_t>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_INT64_EXT;
    }
    else if constexpr (std::same_as<Type, std::uint32_t>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_UINT32_EXT;
    }
    else if constexpr (std::same_as<Type, std::uint64_t>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_UINT64_EXT;
    }
    else if constexpr (std::same_as<Type, float>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_FLOAT32_EXT;
    }
    else if constexpr (std::same_as<Type, double>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_FLOAT64_EXT;
    }
    else if constexpr (std::same_as<Type, const char*>)
    {
      setting->type = VK_LAYER_SETTING_TYPE_STRING_EXT;
    }
    setting->pValues = values;
    setting->valueCount = count;
  }



  static_assert(concepts::builder_object<layer_settings_builder, const VkLayerSettingsCreateInfoEXT&>);

}

#endif
