#ifndef MEGATECH_VULKAN_LOADER_HPP
#define MEGATECH_VULKAN_LOADER_HPP

#include <memory>
#include <vector>

#if defined(MEGATECH_VULKAN_INCLUDE_VULKAN_H) && __has_include(<vulkan/vulkan.h>)
  #include <vulkan/vulkan.h>
#endif

#if defined(MEGATECH_VULKAN_INCLUDE_MEGATECH_DISPATCH_HPP) && __has_include(<megatech/vulkan/dispatch.hpp>)
  #include <megatech/vulkan/dispatch.hpp>
#endif

#include "command.hpp"
#include "error.hpp"

#include "concepts/command_tag.hpp"

namespace megatech::vulkan::internal::base {

  class loader_impl;
  struct loader_impl_dtor final {
    void operator()(loader_impl* p) const noexcept;
  };

}

#ifndef MEGATECH_VULKAN_DISPATCH_HPP
namespace megatech::vulkan::dispatch::global {

  class table;

}
#endif

namespace megatech::vulkan {

  class layer_description;

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

    const std::vector<layer_description>& available_layers() const;
    const dispatch::global::table& dispatch_table() const;

    const implementation_type& implementation() const;
    implementation_type& implementation();
    std::shared_ptr<const implementation_type> share_implementation() const;
#if defined(VK_VERSION_1_0) && defined(MEGATECH_VULKAN_DISPATCH_HPP)
    template <concepts::command_tag Tag>
    inline command<loader, typename Tag::function_pointer_type> resolve() const {
      constexpr auto hash = megatech::vulkan::dispatch::internal::base::fnv_1a_cstr(Tag::name());
      const auto ppfn = dispatch_table()(hash);
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
