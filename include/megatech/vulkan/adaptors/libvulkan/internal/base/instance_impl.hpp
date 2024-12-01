#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP

#include <megatech/vulkan/internal/base/instance_impl.hpp>

#include "../../instance.hpp"

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  class instance_impl final : public megatech::vulkan::internal::base::instance_impl {
  public:
    instance_impl() = delete;
    instance_impl(const adaptors::libvulkan::loader& parent_loader, const std::string& application_name,
                  const version& application_version, const std::unordered_set<std::string>& requested_layers);
    instance_impl(const instance_impl& other) = delete;
    instance_impl(instance_impl&& other) = delete;

    ~instance_impl() noexcept;

    instance_impl& operator=(const instance_impl& rhs) = delete;
    instance_impl& operator=(instance_impl&& rhs) = delete;
  };

  class debug_instance_impl final : public megatech::vulkan::internal::base::instance_impl {
  private:
    VkDebugUtilsMessengerEXT m_debug_messenger{ };
  public:
    debug_instance_impl() = delete;
    debug_instance_impl(const adaptors::libvulkan::loader& parent_loader, const std::string& application_name,
                        const version& application_version, const std::unordered_set<std::string>& requested_layers);
    debug_instance_impl(const debug_instance_impl& other) = delete;
    debug_instance_impl(debug_instance_impl&& other) = delete;

    ~debug_instance_impl() noexcept;

    debug_instance_impl& operator=(const debug_instance_impl& rhs) = delete;
    debug_instance_impl& operator=(debug_instance_impl&& rhs) = delete;
  };

}

#endif
