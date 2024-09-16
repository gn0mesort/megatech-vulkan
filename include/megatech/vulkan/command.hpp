#ifndef MEGATECH_VULKAN_COMMAND_HPP
#define MEGATECH_VULKAN_COMMAND_HPP

#include <concepts>
#include <memory>
#include <type_traits>

#if defined(MEGATECH_VULKAN_INCLUDE_VULKAN_H) && __has_include(<vulkan/vulkan.h>)
  #include <vulkan/vulkan.h>
#endif

#include "concepts/opaque_object.hpp"
#include "concepts/vulkan_function.hpp"

#ifdef VK_VERSION_1_0
namespace megatech::vulkan {

  template <concepts::readonly_sharable_opaque_object CommandOwner, typename FunctionPointer>
  class command final {
  private:
    using owner_type = CommandOwner;
    friend owner_type;

    typename std::shared_ptr<const typename CommandOwner::implementation_type> m_owner{ };
    FunctionPointer m_pfn{ };

    command(const CommandOwner& owner, FunctionPointer pfn) : m_owner{ owner.share_implementation() }, m_pfn{ pfn } { }
  public:
    command() = delete;
    command(const command& other) = default;
    command(command&& other) = delete;

    ~command() noexcept = default;

    command& operator=(const command& rhs) = default;
    command& operator=(command&& rhs) = delete;

    const FunctionPointer* target() const {
      return &m_pfn;
    }

    FunctionPointer* target() {
      return &m_pfn;
    }

    operator bool() const noexcept {
      return !!m_pfn;
    }

    template <typename... Args>
    std::invoke_result_t<FunctionPointer, Args...>
    operator()(Args&&... args) const noexcept(std::is_nothrow_invocable_v<FunctionPointer, Args...>)
    requires concepts::vulkan_function<FunctionPointer> {
      if constexpr (std::is_same_v<std::invoke_result_t<FunctionPointer, Args...>, void>)
      {
        m_pfn(std::forward<Args>(args)...);
      }
      else
      {
        return m_pfn(std::forward<Args>(args)...);
      }
    }
  };

}
#endif

#endif
