#ifndef MEGATECH_VULKAN_DEBUG_MESSENGER_DESCRIPTION_HPP
#define MEGATECH_VULKAN_DEBUG_MESSENGER_DESCRIPTION_HPP

#include <string>
#include <functional>

#include "bitmask.hpp"

namespace megatech::vulkan {

namespace debug_message_type {

  MEGATECH_VULKAN_DECLARE_ZERO_BIT(none);
  MEGATECH_VULKAN_DECLARE_BIT(general, 0);
  MEGATECH_VULKAN_DECLARE_BIT(validation, 1);
  MEGATECH_VULKAN_DECLARE_BIT(performance, 2);

}

namespace debug_message_severity {

  MEGATECH_VULKAN_DECLARE_ZERO_BIT(none);
  MEGATECH_VULKAN_DECLARE_BIT(verbose, 0);
  MEGATECH_VULKAN_DECLARE_BIT(info, 4);
  MEGATECH_VULKAN_DECLARE_BIT(warning, 8);
  MEGATECH_VULKAN_DECLARE_BIT(error, 12);

}

  class debug_messenger_description final {
  public:
    // It's crucial that sink functions do not capture-by-reference any value whose lifetime won't exceed the lifetime
    // of the sink function. Since descriptions are passed to the debug_instance constructor, their lifetime must
    // exceed the entire lifetime of the instance object. Failure to ensure this can result in dangling references.
    using message_sink_fn = void(const bitmask, const bitmask, const std::string&);
  private:
    bitmask m_accepted_message_types;
    bitmask m_accepted_message_severities;
    std::function<message_sink_fn> m_sink;
  public:
    constexpr static bitmask default_message_types{ debug_message_type::general_bit |
                                                    debug_message_type::validation_bit |
                                                    debug_message_type::performance_bit };
    constexpr static bitmask default_message_severities{ debug_message_severity::verbose_bit |
                                                         debug_message_severity::info_bit |
                                                         debug_message_severity::warning_bit |
                                                         debug_message_severity::error_bit };

    debug_messenger_description() = delete;
    explicit debug_messenger_description(const std::function<message_sink_fn>& sink);
    debug_messenger_description(const bitmask accepted_message_severities, const std::function<message_sink_fn>& sink);
    debug_messenger_description(const bitmask accepted_message_types, const bitmask accepted_message_severities,
                                const std::function<message_sink_fn>& sink);
    debug_messenger_description(const debug_messenger_description& other) = default;
    debug_messenger_description(debug_messenger_description&& other) = default;

    ~debug_messenger_description() noexcept = default;

    debug_messenger_description& operator=(const debug_messenger_description& rhs) = default;
    debug_messenger_description& operator=(debug_messenger_description&& rhs) = default;

    const std::function<message_sink_fn>& sink() const;
    bitmask accepted_message_types() const;
    bitmask accepted_message_severities() const;
  };

}

#endif
