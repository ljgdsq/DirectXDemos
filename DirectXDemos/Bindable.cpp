#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
{
    return gfx.context.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
{
    return gfx.device.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept(!_DEBUG)
{
#if _DEBUG
    return gfx.infoManager;
#else
    throw std::logic_error("(tried to access gfx.infoManager in Release config)");
#endif
}
