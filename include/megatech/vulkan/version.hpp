#ifndef MEGATECH_VULKAN_VERSION_HPP
#define MEGATECH_VULKAN_VERSION_HPP

#include <cinttypes>

#include <compare>

namespace megatech::vulkan {

  /**
   * @brief A Vulkan-compatible version number.
   * @details Vulkan separates version numbers into four fields (variant, major version, minor version, and patch
   *          version). The version object simplifies the construction of these values without the need to directly
   *          include Vulkan's header files.
   */
  class version final {
  private:
    std::uint32_t m_data{ };
  public:
    /**
     * @brief Retrieve the maximum possible variant number.
     * @return The max variant number (a/k/a 7).
     */
    static constexpr std::uint_least8_t max_variant() {
      return 0x7;
    }

    /**
     * @brief Retrieve the maxmimum possible major version number.
     * @return The max major version number (a/k/a 127).
     */
    static constexpr std::uint_least8_t max_major_version() {
      return 0x7f;
    }

    /**
     * @brief Retrieve the maximum possible minor version number.
     * @return The max minor version number (a/k/a 1023).
     */
    static constexpr std::uint_least16_t max_minor_version() {
      return 0x3ff;
    }

    /**
     * @brief Retrieve the maximum possible patch version number.
     * @return The max patch version number (a/k/a 4095).
     */
    static constexpr std::uint_least16_t max_patch_version() {
      return 0xfff;
    }

    /**
     * @brief Construct a version.
     * @details This is a converting constructor that decomposes the input into a version structure. Inputs should
     *          generally be Vulkan version numbers. If they aren't, you'll get weird outputs.
     * @param data An unsigned 32-bit value formatted as a Vulkan compatible version number.
     */
    explicit version(const std::uint32_t data) noexcept;

    /**
     * @brief Construct a version.
     * @param variant The variant version number.
     * @param major The major version number.
     * @param minor The minor version number.
     * @param patch The patch version number.
     */
    version(const std::uint_least8_t variant, const std::uint_least8_t major, const std::uint_least16_t minor,
            const std::uint_least16_t patch);

    /**
     * @brief Copy a version.
     * @param other The version to copy.
     */
    version(const version& other) = default;

    /**
     * @brief Move a version.
     * @param other The version to move.
     */
    version(version&& other) = default;

    /**
     * @brief Destroy a version.
     */
    ~version() noexcept = default;

    /**
     * @brief Copy-assign a version.
     * @param rhs The version to copy.
     * @return A reference to the copied-to version.
     */
    version& operator=(const version& rhs) = default;

    /**
     * @brief Move-assign a version.
     * @param rhs The version to copy.
     * @return A reference to the moved-to version.
     */
    version& operator=(version&& rhs) = default;

    /**
     * @brief Perform a three-way comparison between two versions.
     * @param rhs The version to compare to.
     * @return A std::strong_ordering constant indicating the relationship between the two versions.
     */
    std::strong_ordering operator<=>(const version& rhs) const noexcept;

    /**
     * @brief Compare two versions for equality.
     * @param rhs The version to compare to.
     * @return True if the two version numbers are equal. False otherwise.
     */
    bool operator==(const version& rhs) const = default;

    /**
     * @brief Convert a version into an unsigned 32-bit integer.
     * @return A Vulkan compatible 32-bit version number.
     */
    explicit operator std::uint32_t() const noexcept;

    /**
     * @brief Retrive a version's variant number.
     * @return An unsigned 8-bit variant number.
     */
    std::uint_least8_t variant() const noexcept;

    /**
     * @brief Retrive a version's major version number.
     * @return An unsigned 8-bit major version number.
     */
    std::uint_least8_t major() const noexcept;

    /**
     * @brief Retrive a version's minor version number.
     * @return An unsigned 16-bit minor version number.
     */
    std::uint_least16_t minor() const noexcept;


    /**
     * @brief Retrive a version's patch version number.
     * @return An unsigned 16-bit patch version number.
     */
    std::uint_least16_t patch() const noexcept;

  };

}

#endif
