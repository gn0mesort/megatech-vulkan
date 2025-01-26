#ifndef MEGATECH_VULKAN_LAYER_DESCRIPTION_HPP
#define MEGATECH_VULKAN_LAYER_DESCRIPTION_HPP

#include <cstddef>

#include <string>
#include <functional>

#include "version.hpp"

namespace megatech::vulkan::internal::base {

  struct layer_description_proxy;

}

namespace megatech::vulkan {

  /**
   * @brief A description of a Vulkan API layer.
   */
  class layer_description final {
  public:
    using proxy_type = internal::base::layer_description_proxy;
  private:
    version m_specification_version{ 0, 1, 0, 0 };
    std::uint32_t m_implementation_version{ 1 };
    std::string m_name{ };
    std::string m_description{ };
  public:
    /**
     * @brief Construct a layer_description.
     * @details Although this constructor is public, there is no way for it to be invoked without accessing the
     *          internal Megatech-Vulkan namespaces. Systems only have a specific set of Vulkan API layers available
     *          at run-time, and constructing more is meaningless. Basically, unless you know what you're doing, you
     *          don't need to invoke this.
     * @param proxy An opaque proxy returned by the Vulkan API.
     */
    explicit layer_description(const proxy_type& proxy);

    /**
     * @brief Copy a layer_description.
     * @param other The layer_description to copy.
     */
    layer_description(const layer_description& other) = default;

    /**
     * @brief Move a layer_description.
     * @param other The layer_description to move.
     */
    layer_description(layer_description&& other) = default;

    /**
     * @brief Destroy a layer_description.
     */
    ~layer_description() noexcept = default;

    /**
     * @brief Copy-assign a layer_description.
     * @param rhs The layer_description to copy.
     * @return A reference to the copied-to layer_description.
     */
    layer_description& operator=(const layer_description& rhs) = default;

    /**
     * @brief Move-assign a layer_description.
     * @param rhs The layer_description to move.
     * @return A reference to the moved-to layer_description.
     */
    layer_description& operator=(layer_description&& rhs) = default;

    /**
     * @brief Compare two layer_descriptions for equality.
     * @details Although layer_descriptions contain much more than just name information, Vulkan considers two layers
     *          equal based on their names alone. Essentially, the layer name is a unique id.
     * @param rhs The layer_description to compare to.
     * @return True it the described layers have the same name. False otherwise.
     */
    bool operator==(const layer_description& rhs) const;

    /**
     * @brief Compare a layer_description to a string for equality.
     * @param rhs The string to compare to.
     * @return True if the layer's name matches rhs. False otherwise.
     */
    bool operator==(const std::string& name) const;

    /**
     * @brief Retrieve a read-only reference to a layer_description's specification version.
     * @return The version of the Vulkan specification that the layer was written for.
     */
    const version& specification_version() const;

    /**
     * @brief Retrieve a 32-bit unsigned integer representing the implementation version of a layer_description.
     * @return A layer specific 32-bit version number.
     */
    std::uint32_t implementation_version() const;

    /**
     * @brief Retrieve a read-only reference to a layer_description's name.
     * @return The name of the described layer.
     */
    const std::string& name() const;

    /**
     * @brief Retrieve a read-only reference to a layer_descriptions descriptive text.
     * @return A string containing additional details about the described layer.
     */
    const std::string& description() const;
  };

}

// It's weird, but one of the few times it's acceptable to open the std namespace is to specialize template structs.
namespace std {

  // These templates are provided to enable heterogenous access in std::unordered_set<layer_description>.
  // In essence, you can search using strings in a set of layer_descriptions. This is because Vulkan uses the layer
  // name as a unique ID for the layer.
  //
  // The "is_transparent" symbol must be defined to enable heterogenous access even with the correct method overloads
  // declared.
  template <>
  struct hash<megatech::vulkan::layer_description> final {
    using is_transparent = void;

    std::size_t operator()(const megatech::vulkan::layer_description& layer) const noexcept;
    std::size_t operator()(const std::string& name) const noexcept;
  };

  template <>
  struct equal_to<megatech::vulkan::layer_description> final {
    using is_transparent = void;

    bool operator()(const megatech::vulkan::layer_description& lhs, const megatech::vulkan::layer_description& rhs) const noexcept;
    bool operator()(const megatech::vulkan::layer_description& lhs, const std::string& rhs) const noexcept;
    bool operator()(const std::string& lhs, const megatech::vulkan::layer_description& rhs) const noexcept;
  };

}

#endif
