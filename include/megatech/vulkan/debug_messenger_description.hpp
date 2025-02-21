/**
 * @file debug_messenger_description.hpp
 * @brief Debug Messenger Descriptions
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_DEBUG_MESSENGER_DESCRIPTION_HPP
#define MEGATECH_VULKAN_DEBUG_MESSENGER_DESCRIPTION_HPP

#include <string>
#include <functional>

#include "bitmask.hpp"

namespace megatech::vulkan {

/**
 * @brief Constants for describing types of debug messages.
 */
namespace debug_message_type {

  /**
   * @brief An empty message type.
   */
  MEGATECH_VULKAN_DECLARE_ZERO_BIT(none);

  /**
   * @brief General diagnostic messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(general, 0);

  /**
   * @brief Validation messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(validation, 1);

  /**
   * @brief Performance messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(performance, 2);

}

/**
 * @brief Constants for describing the severity of debug messages.
 */
namespace debug_message_severity {

  /**
   * @brief An empty message severity.
   */
  MEGATECH_VULKAN_DECLARE_ZERO_BIT(none);

  /**
   * @brief Verbose messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(verbose, 0);

  /**
   * @brief Informative messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(info, 4);

  /**
   * @brief Warning messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(warning, 8);

  /**
   * @brief Error messages.
   */
  MEGATECH_VULKAN_DECLARE_BIT(error, 12);

}

  /**
   * @brief A description of a debug messenger.
   * @details Debug messengers are used to instrument Vulkan instances. They're usually combined with validation layers
   *          to deliver diagnostics from the Vulkan API. Nonetheless, you can use a debug messenger without any layers
   *          enabled. Doing so limits the functionality to receiving messages from Megatech-Vulkan.
   */
  class debug_messenger_description final {
  public:
    /**
     * @brief A function type which can recieve debug messages.
     * @details It's crucial that sink functions do not capture-by-reference any value whose lifetime won't exceed
     *          the lifetime of the sink function. Since descriptions are passed to the debug_instance constructor,
     *          their lifetime must exceed the entire lifetime of the instance object. Failure to ensure this can
     *          result in dangling references.
     *
     *          Parameters are passed as-if from debug_instance::send_debug_message.
     */
    using message_sink_fn = void(const bitmask, const bitmask, const std::string&);
  private:
    bitmask m_accepted_message_types;
    bitmask m_accepted_message_severities;
    std::function<message_sink_fn> m_sink;
  public:
    /**
     * @brief The default set of enabled message types.
     * @details All message types are enabled by default.
     */
    constexpr static bitmask default_message_types{ debug_message_type::general_bit |
                                                    debug_message_type::validation_bit |
                                                    debug_message_type::performance_bit };

    /**
     * @brief The default set of enabled message severity levels.
     * @details All severity levels are enabled by default.
     */
    constexpr static bitmask default_message_severities{ debug_message_severity::verbose_bit |
                                                         debug_message_severity::info_bit |
                                                         debug_message_severity::warning_bit |
                                                         debug_message_severity::error_bit };

    /// @cond
    debug_messenger_description() = delete;
    /// @endcond

    /**
     * @brief Construct a debug_messenger_description.
     * @param sink A message sink function-object. This is where messages will be directed.
     */
    explicit debug_messenger_description(const std::function<message_sink_fn>& sink);

    /**
     * @brief Construct a debug_messenger_description.
     * @param accepted_message_severities The set of message severities that should trigger a debug message.
     * @param sink A message sink function-object. This is where messages will be directed.
     */
    debug_messenger_description(const bitmask accepted_message_severities, const std::function<message_sink_fn>& sink);

    /**
     * @brief Construct a debug_messenger_description.
     * @param accepted_message_types The set of message types that should trigger a debug message.
     * @param accepted_message_severities The set of message severities that should trigger a debug message.
     * @param sink A message sink function-object. This is where messages will be directed.
     */
    debug_messenger_description(const bitmask accepted_message_types, const bitmask accepted_message_severities,
                                const std::function<message_sink_fn>& sink);

    /**
     * @brief Copy a debug_messenger_description.
     * @param other The debug_messenger_description to copy.
     */
    debug_messenger_description(const debug_messenger_description& other) = default;

    /**
     * @brief Move a debug_messenger_description.
     * @param other The debug_messenger_description to move.
     */
    debug_messenger_description(debug_messenger_description&& other) = default;

    /**
     * @brief Destroy a debug_messenger_description.
     */
    ~debug_messenger_description() noexcept = default;

    /**
     * @brief Copy-assign a debug_messenger_description.
     * @param rhs The debug_messenger_description to copy.
     * @return A reference to the copied-to debug_messenger_description.
     */
    debug_messenger_description& operator=(const debug_messenger_description& rhs) = default;

    /**
     * @brief Copy-assign a debug_messenger_description.
     * @param rhs The debug_messenger_description to copy.
     * @return A reference to the copied-to debug_messenger_description.
     */
    debug_messenger_description& operator=(debug_messenger_description&& rhs) = default;

    /**
     * @brief Retrieve the debug_messenger_description's sink function-object.
     * @return A read-only reference to the sink function-object.
     */
    const std::function<message_sink_fn>& sink() const;

    /**
     * @brief Retrieve the set of debug_message_types that the debug_message_description accepts.
     * @return A bitmask of the accepted message type constants.
     */
    bitmask accepted_message_types() const;

    /**
     * @brief Retrieve the set of debug_message_severitys that the debug_message_description accepts.
     * @return A bitmask of the accepted message severity constants.
     */
    bitmask accepted_message_severities() const;
  };

}

#endif
