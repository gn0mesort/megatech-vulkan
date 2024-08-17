#ifndef MEGATECH_VULKAN_LOADER_HPP
#define MEGATECH_VULKAN_LOADER_HPP
#include <cinttypes>

#include <memory>

#ifdef MEGATECH_VULKAN_INCLUDE_VULKAN_H
  #include <vulkan/vulkan.h>
#endif

#include "command.hpp"
#include "error.hpp"

#include "concepts/command_tag.hpp"

#include "internal/base/fnv_1a.hpp"


namespace megatech::vulkan::internal::base {

  class loader_impl;
  struct loader_impl_dtor final {
    void operator()(loader_impl* p) const noexcept;
  };

}

namespace megatech::vulkan::dispatch::global {

  class table;

}

namespace megatech::vulkan {

  class loader {
  public:
    using implementation_type = internal::base::loader_impl;
  private:
    const void* resolve(const std::uint_least64_t hash) const;
  protected:
    loader(internal::base::loader_impl*&& impl);


    std::shared_ptr<internal::base::loader_impl> m_impl;
  public:
    loader() = delete;
    loader(const loader& other) = delete;
    loader(loader&& other) = delete;

    virtual ~loader() noexcept = default;

    loader& operator=(const loader& rhs) = delete;
    loader& operator=(loader&& rhs) = delete;

    const implementation_type& implementation() const;
    implementation_type& implementation();
    std::shared_ptr<const implementation_type> share_implementation() const;
    const dispatch::global::table& dispatch_table() const;
#ifdef VK_VERSION_1_0
    template <concepts::command_tag Tag>
    inline command<loader, typename Tag::function_pointer_type> resolve() const {
      constexpr auto hash = internal::base::fnv_1a_cstr(Tag::name());
      const auto ppfn = resolve(hash);
      if (!ppfn) [[unlikely]]
      {
        throw error{ "The Vulkan command could not be resolved." };
      }
      const auto pfn = *reinterpret_cast<const typename Tag::function_pointer_type*>(ppfn);
      if (!pfn) [[unlikely]]
      {
        throw error{ "The Vulkan command is not available in the current configuration." };
      }
      return command<loader, typename Tag::function_pointer_type>{ *this, pfn };
    }
#endif
  };

}

#endif
