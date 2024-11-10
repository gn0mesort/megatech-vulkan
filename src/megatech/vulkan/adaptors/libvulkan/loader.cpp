#define MEGATECH_VULKAN_INCLUDE_VULKAN_H
#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include <utility>

#include "megatech/vulkan/adaptors/libvulkan/internal/base/loader_impl.hpp"

namespace megatech::vulkan::adaptors::libvulkan {

  loader::loader() : megatech::vulkan::loader{ new internal::base::loader_impl{ } } { }

  loader::loader(const loader& other) :
  megatech::vulkan::loader{
    new internal::base::loader_impl{ static_cast<internal::base::loader_impl&>(*other.m_impl) }
  } { }

  loader& loader::operator=(const loader& rhs) {
    if (this != &rhs)
    {
      auto cpy = loader{ rhs };
      std::swap(m_impl, cpy.m_impl);
    }
    return *this;
  }

}
