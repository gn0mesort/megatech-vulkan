/**
 * @file instance.cpp
 * @brief Vulkan Instances
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/instance.hpp"

#include <utility>

#include <megatech/assertions.hpp>

#include "megatech/vulkan/loader.hpp"
#include "megatech/vulkan/window_system.hpp"
#include "megatech/vulkan/debug_messenger_description.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan {

  instance::instance(const std::shared_ptr<implementation_type>& impl) :
  m_impl{ impl } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  instance::instance(const loader& parent, const application_description& app_description) :
  instance{ parent, app_description, { } } { }

  instance::instance(const loader& parent, const window_system& wsi, const application_description& app_description) :
  instance{ parent, wsi, app_description, { } } { }

  instance::instance(const loader& parent, const application_description& app_description,
                     const std::unordered_set<std::string>& requested_layers) :
  m_impl{ new implementation_type{ parent.share_implementation(), app_description, requested_layers } } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  instance::instance(const loader& parent, const window_system& wsi, const application_description& app_description,
                     const std::unordered_set<std::string>& requested_layers) :
  m_impl{
    new implementation_type{ parent.share_implementation(), wsi.share_implementation(), app_description,
                             requested_layers }
  } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  const instance::implementation_type& instance::implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  instance::implementation_type& instance::implementation() {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  std::shared_ptr<const instance::implementation_type> instance::share_implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl;
  }

  debug_instance::debug_instance(const std::shared_ptr<extended_implementation_type>& impl) :
  instance{ impl } { }

  debug_instance::debug_instance(const loader& parent, const application_description& app_description) :
  debug_instance{ parent, app_description, { } } { }

  debug_instance::debug_instance(const loader& parent, const window_system& wsi,
                                 const application_description& app_description) :
  debug_instance{ parent, wsi, app_description, { } } { }

  debug_instance::debug_instance(const loader& parent, const application_description& app_description,
                                 const std::unordered_set<std::string>& requested_layers) :
  debug_instance{ std::shared_ptr<extended_implementation_type>{
    new extended_implementation_type{ parent.share_implementation(), app_description, requested_layers }
  } } { }

  debug_instance::debug_instance(const loader& parent, const window_system& wsi,
                                 const application_description& app_description,
                                 const std::unordered_set<std::string>& requested_layers) :
  debug_instance{ std::shared_ptr<extended_implementation_type>{
    new extended_implementation_type{ parent.share_implementation(), wsi.share_implementation(), app_description,
                                      requested_layers }
  } } { }

  debug_instance::debug_instance(const loader& parent, const application_description& app_description,
                                 const debug_messenger_description& messenger_description,
                                 const std::unordered_set<std::string>& requested_layers) :
  debug_instance{ std::shared_ptr<extended_implementation_type>{
    new extended_implementation_type{ parent.share_implementation(), app_description, messenger_description,
                                      requested_layers }
  } } { }

  debug_instance::debug_instance(const loader& parent, const window_system& wsi,
                                 const application_description& app_description,
                                 const debug_messenger_description& messenger_description,
                                 const std::unordered_set<std::string>& requested_layers) :
  debug_instance{ std::shared_ptr<extended_implementation_type>{
    new extended_implementation_type{ parent.share_implementation(), wsi.share_implementation(), app_description,
                                      messenger_description, requested_layers }
  } } { }

  void debug_instance::submit_debug_message(const bitmask types, const bitmask severity,
                                            const std::string& message) const {
    MEGATECH_PRECONDITION((types & ~debug_messenger_description::default_message_types) == bitmask{ 0 });
    MEGATECH_PRECONDITION((types & debug_messenger_description::default_message_types) != bitmask{ 0 });
    MEGATECH_PRECONDITION(std::has_single_bit(static_cast<std::uint64_t>(severity)));
    MEGATECH_POSTCONDITION((severity & ~debug_messenger_description::default_message_severities) == bitmask{ 0 });
    MEGATECH_PRECONDITION((severity & debug_messenger_description::default_message_severities) != bitmask{ 0 });
    static_cast<const extended_implementation_type&>(implementation()).submit_debug_message(types, severity, message);
  }

}
