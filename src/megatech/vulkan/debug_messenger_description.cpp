#include "megatech/vulkan/debug_messenger_description.hpp"

#include <megatech/assertions.hpp>

#include "megatech/vulkan/error.hpp"

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
  m_sink{ sink } {
    if (sink == nullptr)
    {
      throw error{ "The message sink function cannot be empty." };
    }
    if ((m_accepted_message_types & ~default_message_types) != bitmask{ 0 })
    {
      throw error{ "The type bitmask contains invalid bits." };
    }
    if ((m_accepted_message_severities & ~default_message_severities) != bitmask{ 0 })
    {
      throw error{ "The severity bitmask contains invalid bits." };
    }
    MEGATECH_POSTCONDITION(m_sink != nullptr);
    MEGATECH_POSTCONDITION((m_accepted_message_types & ~default_message_types) == bitmask{ 0 });
    MEGATECH_POSTCONDITION((m_accepted_message_severities & ~default_message_severities) == bitmask{ 0 });
  }

  const std::function<debug_messenger_description::message_sink_fn>& debug_messenger_description::sink() const {
    MEGATECH_PRECONDITION(m_sink != nullptr);
    return m_sink;
  }

  bitmask debug_messenger_description::accepted_message_types() const {
    MEGATECH_PRECONDITION((m_accepted_message_types & ~default_message_types) == bitmask{ 0 });
    return m_accepted_message_types;
  }

  bitmask debug_messenger_description::accepted_message_severities() const {
    MEGATECH_PRECONDITION((m_accepted_message_severities & ~default_message_severities) == bitmask{ 0 });
    return m_accepted_message_severities;
  }

}
