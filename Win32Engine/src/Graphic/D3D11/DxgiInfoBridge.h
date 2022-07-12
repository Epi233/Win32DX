#pragma once

#include <d3d11.h>
#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>

#include "../../DefinwWindows.h"

class DxgiInfoBridge
{
public:
    typedef HRESULT (WINAPI *DXGIGetDebugInterface)(REFIID, void**);
    
public:
    DxgiInfoBridge();
    ~DxgiInfoBridge();
    DxgiInfoBridge(const DxgiInfoBridge&) = delete;
    DxgiInfoBridge& operator=(const DxgiInfoBridge&) = delete;

public:
    void setLogPosition();
    std::vector<std::wstring> getLog() const;

private:
    unsigned long long _logPosition = 0;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> _pDxgiInfoQueue;
};
