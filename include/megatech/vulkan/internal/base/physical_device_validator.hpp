#ifndef MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_VALIDATOR_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_VALIDATOR_HPP

namespace megatech::vulkan {

  class physical_device_description;

}

namespace megatech::vulkan::internal::base {

  class physical_device_validator {
  private:
    bool has_core_requirements(const physical_device_description& physical_device) const noexcept;
  protected:
    virtual bool has_extended_requirements(const physical_device_description& physical_device) const noexcept;
  public:
    physical_device_validator() = default;
    physical_device_validator(const physical_device_validator& other) = default;
    physical_device_validator(physical_device_validator&& other) = default;

    virtual ~physical_device_validator() noexcept = default;

    physical_device_validator& operator=(const physical_device_validator& rhs) = default;
    physical_device_validator& operator=(physical_device_validator&& rhs) = default;

    bool is_valid(const physical_device_description& physical_device) const noexcept;
    bool operator()(const physical_device_description& physical_device) const noexcept;
  };

}

#endif
