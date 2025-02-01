/// @cond INTERNAL
/**
 * @file physical_device_allocator.hpp
 * @brief Indirect Physical Device Object Allocator
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_ALLOCATOR_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_ALLOCATOR_HPP

#include <memory>

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class physical_device_description_impl;

  /**
   * @brief A base allocator for default physical_device_description_impls.
   * @details Adaptor authors must specialize this if they have specialized physical_device_description_impl.
   *          The specialized allocator must return pointers to the matching specialization of
   *          physical_device_description_impl via the allocate method.
   */
  class physical_device_allocator {
  public:
    /**
     * @brief The type of Vulkan handle required to allocate a physical_device_description_impl.
     */
    using handle_type = VkPhysicalDevice;

    /**
     * @brief The type of parent object required to allocate a physical_device_description_impl.
     */
    using parent_type = instance_impl;

    /**
     * @brief The type of object allocated by the physical_device_allocator.
     */
    using result_type = physical_device_description_impl;

    /**
     * @brief Construct a physical_device_allocator.
     */
    physical_device_allocator() = default;

    /**
     * @brief Copy a physical_device_allocator.
     * @param other The physical_device_allocator to copy.
     */
    physical_device_allocator(const physical_device_allocator& other) = default;


    /**
     * @brief Move a physical_device_allocator.
     * @param other The physical_device_allocator to move.
     */
    physical_device_allocator(physical_device_allocator&& other) = default;

    /**
     * @brief Destroy a physical_device_allocator.
     */
    virtual ~physical_device_allocator() noexcept = default;

    /**
     * @brief Copy-assign a physical_device_allocator.
     * @param rhs The physical_device_allocator to copy.
     * @return A reference to the copied-to physical_device_allocator.
     */
    physical_device_allocator& operator=(const physical_device_allocator& rhs) = default;

    /**
     * @brief Move-assign a physical_device_allocator.
     * @param rhs The physical_device_allocator to move.
     * @return A reference to the moved-to physical_device_allocator.
     */
    physical_device_allocator& operator=(physical_device_allocator&& rhs) = default;

    /**
     * @brief Allocate a physical_device_description_impl.
     * @details Adaptor implementators must override this method if they extend the physical_device_description_impl.
     *          physical_device_list::physical_device_list always calls physical_device_allocator::operator() to
     *          allocate physical device descriptions. That means, you must provide a correctly specialized allocator
     *          to instance_impl to use your specialized physical_device_description_impl.
     * @param parent A shared_ptr to a read-only instance_impl. This must not be null.
     * @param handle A VkPhysicalDevice representing the underlying physical device. This must not be VK_NULL_HANDLE.
     * @return A pointer to a physical_device_description_impl.
     */
    virtual result_type* allocate(const std::shared_ptr<const parent_type>& parent, handle_type handle) const;

    /**
     * @brief Allocate a physical_device_description_impl.
     * @param parent A shared_ptr to a read-only instance_impl. This must not be null.
     * @param handle A VkPhysicalDevice representing the underlying physical device. This must not be VK_NULL_HANDLE.
     * @return A pointer to a physical_device_description_impl.
     */
    result_type* operator()(const std::shared_ptr<const parent_type>& parent, handle_type handle) const;
  };

}

#endif
/// @endcond
