#include <windows.h>

#include "dxgi-sample.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define RESET_OBJECT(obj) { if(obj) obj->Release(); obj = NULL; }

VideoDXGICaptor::VideoDXGICaptor()
{
    m_bInit = FALSE;

    m_hDevice = NULL;
    m_hContext = NULL;
    m_hDeskDupl = NULL;

    ZeroMemory(&m_dxgiOutDesc, sizeof(m_dxgiOutDesc));
}
VideoDXGICaptor::~VideoDXGICaptor()
{
    Deinit();
}
BOOL VideoDXGICaptor::Init()
{
    HRESULT hr = S_OK;

    if (m_bInit)
    {
        return FALSE;
    }

    // Driver types supported
    D3D_DRIVER_TYPE DriverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

    // Feature levels supported
    D3D_FEATURE_LEVEL FeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_1
    };
    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

    D3D_FEATURE_LEVEL FeatureLevel;

    //
    // Create D3D device
    //
    for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
    {
        hr = D3D11CreateDevice(NULL, DriverTypes[DriverTypeIndex], NULL, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &m_hDevice, &FeatureLevel, &m_hContext);
        if (SUCCEEDED(hr))
        {
            break;
        }
    }
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // Get DXGI device
    //
    IDXGIDevice *hDxgiDevice = NULL;
    hr = m_hDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&hDxgiDevice));
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // Get DXGI adapter
    //
    IDXGIAdapter *hDxgiAdapter = NULL;
    hr = hDxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&hDxgiAdapter));
    RESET_OBJECT(hDxgiDevice);
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // Get output
    //
    INT nOutput = 0;
    IDXGIOutput *hDxgiOutput = NULL;
    hr = hDxgiAdapter->EnumOutputs(nOutput, &hDxgiOutput);
    RESET_OBJECT(hDxgiAdapter);
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // get output description struct
    //
    hDxgiOutput->GetDesc(&m_dxgiOutDesc);

    //
    // QI for Output 1  // IDXGIOutput1 have DuplicateOutput function
    //
    IDXGIOutput1 *hDxgiOutput1 = NULL;
    hr = hDxgiOutput->QueryInterface(__uuidof(hDxgiOutput1), reinterpret_cast<void**>(&hDxgiOutput1));
    RESET_OBJECT(hDxgiOutput);
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // Create desktop duplication
    //
    hr = hDxgiOutput1->DuplicateOutput(m_hDevice, &m_hDeskDupl);
    RESET_OBJECT(hDxgiOutput1);
    if (FAILED(hr))
    {
        return FALSE;
    }

    m_bInit = TRUE;
    return TRUE;
}


VOID VideoDXGICaptor::Deinit()
{
    if (!m_bInit)
    {
        return;
    }

    m_bInit = FALSE;

    if (m_hDeskDupl)
    {
        m_hDeskDupl->Release();
        m_hDeskDupl = NULL;
    }

    if (m_hDevice)
    {
        m_hDevice->Release();
        m_hDevice = NULL;
    }

    if (m_hContext)
    {
        m_hContext->Release();
        m_hContext = NULL;
    }
}

BOOL VideoDXGICaptor::CaptureImage(void *pData)
{
    return QueryFrame(pData);
}
BOOL VideoDXGICaptor::ResetDevice()
{
    Deinit();
    return Init();
}
BOOL VideoDXGICaptor::QueryFrame(void *pImgData)
{

    if (!m_dxgiOutDesc.AttachedToDesktop)
    {
        return FALSE;
    }

    IDXGIResource *hDesktopResource = NULL;
    DXGI_OUTDUPL_FRAME_INFO FrameInfo;
    HRESULT hr = m_hDeskDupl->AcquireNextFrame(200, &FrameInfo, &hDesktopResource);
    if (FAILED(hr))
    {
        return TRUE;
    }

    //
    // query next frame staging buffer
    //
    ID3D11Texture2D *hAcquiredDesktopImage = NULL;
    hr = hDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&hAcquiredDesktopImage));
    RESET_OBJECT(hDesktopResource);
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // copy old description
    //
    D3D11_TEXTURE2D_DESC frameDescriptor;
    hAcquiredDesktopImage->GetDesc(&frameDescriptor);

    //
    // create a new staging buffer for fill frame image
    //
    ID3D11Texture2D *hNewDesktopImage = NULL;
    frameDescriptor.Usage = D3D11_USAGE_STAGING;
    frameDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    frameDescriptor.BindFlags = 0;
    frameDescriptor.MiscFlags = 0;
    frameDescriptor.MipLevels = 1;
    frameDescriptor.ArraySize = 1;
    frameDescriptor.SampleDesc.Count = 1;
    hr = m_hDevice->CreateTexture2D(&frameDescriptor, NULL, &hNewDesktopImage);
    if (FAILED(hr))
    {
        RESET_OBJECT(hAcquiredDesktopImage);
        m_hDeskDupl->ReleaseFrame();
        return FALSE;
    }

    //
    // copy next staging buffer to new staging buffer
    //
    m_hContext->CopyResource(hNewDesktopImage, hAcquiredDesktopImage);
    //m_hContext->Draw(0, 0);
    RESET_OBJECT(hAcquiredDesktopImage);
    m_hDeskDupl->ReleaseFrame();

    //
    // create staging buffer for map bits
    //
    IDXGISurface *hStagingSurf = NULL;
    hr = hNewDesktopImage->QueryInterface(__uuidof(IDXGISurface), (void **)(&hStagingSurf));
    RESET_OBJECT(hNewDesktopImage);
    if (FAILED(hr))
    {
        return FALSE;
    }

    //
    // copy bits to user space
    //
    DXGI_MAPPED_RECT mappedRect;
    hr = hStagingSurf->Map(&mappedRect, DXGI_MAP_READ);
    if (SUCCEEDED(hr))
    {
        memcpy((BYTE*)pImgData, mappedRect.pBits, m_dxgiOutDesc.DesktopCoordinates.right * m_dxgiOutDesc.DesktopCoordinates.bottom * 4);
        hStagingSurf->Unmap();
    }

    RESET_OBJECT(hStagingSurf);
    return SUCCEEDED(hr);
}


void * CaptureInternal(void)
{
    VideoDXGICaptor *CaptureTest;
    CaptureTest = new VideoDXGICaptor();
    CaptureTest->Init();
    void *pImgData = nullptr;
    pImgData = malloc(9999999);

    CaptureTest->CaptureImage(pImgData);
    delete CaptureTest;
    //free(pImgData);
    return pImgData;
}