#include <memory>
#include "DxgiInfoBridge.h"
#include "../../Exception/EngineException.h"
#include "../../Util/StringUtil.h"

#pragma comment(lib, "dxguid.lib")

DxgiInfoBridge::DxgiInfoBridge()
{
    HMODULE hDxgiDebugDll = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hDxgiDebugDll == nullptr)
    {
        
    }

    DXGIGetDebugInterface dxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(
            hDxgiDebugDll, "DXGIGetDebugInterface")));
    if (dxgiGetDebugInterface == nullptr)
    {
        
    }

    HRESULT hr = dxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&_pDxgiInfoQueue));
    HResultException::check(hr, __LINE__, __FILE__);
}

DxgiInfoBridge::~DxgiInfoBridge()
{
    if (_pDxgiInfoQueue != nullptr)
        _pDxgiInfoQueue->Release();
}

void DxgiInfoBridge::setLogPosition()
{
    _logPosition = _pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::wstring> DxgiInfoBridge::getLog() const
{
    std::vector<std::wstring> log;
    auto totalNumber = _pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (auto i = _logPosition; i < totalNumber; i++)
    {
        SIZE_T logLen;
        HRESULT hr = _pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &logLen);
        HResultException::check(hr, __LINE__, __FILE__);

        auto bytes = std::make_unique<byte[]>(logLen);
        auto pLog = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

        hr = _pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pLog, &logLen);
        HResultException::check(hr, __LINE__, __FILE__);

        log.emplace_back(StringUtil::charPtrToWString(pLog->pDescription));
    }

    return log;
}
