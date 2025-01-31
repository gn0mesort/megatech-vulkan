#ifndef MEGATECH_VULKAN_APPLICATION_DESCRIPTION_HPP
#define MEGATECH_VULKAN_APPLICATION_DESCRIPTION_HPP

#include <string>

#include "version.hpp"

namespace megatech::vulkan {

  /**
   * @brief A description of a Vulkan application.
   * @details This contains roughly the same information as VkApplicationInfo would. Its purpose is to provide the
   *          library and driver with information about your application.
   */
  class application_description final {
  private:
    class version m_version{ 0, 1, 0, 0 };
    std::string m_name{ "" } ;
  public:
    /**
     * @brief Construct an application_description.
     */
    application_description() = default;

    /**
     * @brief Construct an application_description.
     * @param name The application's name.
     * @param version The application's version.
     */
    application_description(const std::string& name, const class version& ver);

    /**
     * @brief Copy an application_description.
     * @param other The application_description to copy.
     */
    application_description(const application_description& other) = default;

    /**
     * @brief Move an application_description.
     * @param other The application_description to move.
     */
    application_description(application_description&& other) = delete;

    /**
     * @brief Destroy an application_description.
     */
    ~application_description() noexcept = default;


    /**
     * @brief Copy-assign an application_description.
     * @param rhs The application_description to copy.
     * @return A reference to the copied-to application_description.
     */
    application_description& operator=(const application_description& rhs) = default;

    /**
     * @brief Move-assign an application_description.
     * @param rhs The application_description to move.
     * @return A reference to the moved-to application_description.
     */
    application_description& operator=(application_description&& rhs) = default;

    /**
     * @brief Retrieve the application's name.
     * @return A read-only reference to the application's name.
     */
    const std::string& name() const;

    /**
     * @brief Retrieve the application's version.
     * @return A read-only reference to the application's version.
     */
    const class version& version() const;
  };

}

#endif
