#include "megatech/vulkan/debug_messenger_description.hpp"

namespace megatech::vulkan {

  debug_messenger_description::debug_messenger_description(const std::function<message_sink_fn>& sink) :
  debug_messenger_description{ default_message_types, default_message_severities, sink } { }

  debug_messenger_description::debug_messenger_description(const bitmask accepted_message_severities,
                                                           const std::function<message_sink_fn>& sink) :
  debug_messenger_description{ default_message_types, accepted_message_severities, sink } { }

  debug_messenger_description::debug_messenger_description(const bitmask accepted_message_types,
                                                           const bitmask accepted_message_severities,
                                                           const std::function<message_sink_fn>& sink) :
  m_accepted_message_types{ accepted_message_types },
  m_accepted_message_severities{ accepted_message_severities },
  m_sink{ sink } { }

  const std::function<debug_messenger_description::message_sink_fn>& debug_messenger_description::sink() const {
    return m_sink;
  }

  bitmask debug_messenger_description::accepted_message_types() const {
    return m_accepted_message_types;
  }

  bitmask debug_messenger_description::accepted_message_severities() const {
    return m_accepted_message_severities;
  }

}
