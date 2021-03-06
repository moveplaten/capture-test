#include <windows.h>
#include <stdio.h>
#include "dxgi-sample.h"
extern VideoDXGICaptor* dxgi_capt;

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

    numAdp = 0;
    pAdapterIN = NULL;
    selAdpName = NULL;
    ZeroMemory(&frameInfo, sizeof(DXGI_OUTDUPL_FRAME_INFO));
}

VideoDXGICaptor::~VideoDXGICaptor()
{
    Deinit();
}

BOOL VideoDXGICaptor::SelectAdapters(void)
{
    HRESULT hr = S_OK;
    IDXGIFactory1 *pFactory = NULL;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory));
    if (FAILED(hr))
    {
        return FALSE;
    }

    IDXGIAdapter *pAdapter = NULL;

    for (UINT i = 0; hr != DXGI_ERROR_NOT_FOUND; numAdp = i++)
    {
        hr = pFactory->EnumAdapters(i, &pAdapter);
        DXGI_ADAPTER_DESC desc;
        if (hr != DXGI_ERROR_NOT_FOUND)
        {
            pAdapter->GetDesc(&desc);
            printf("\n%d: %ls", i, desc.Description);
        }
    }
    
    while (printf("\nEnter the Adapter number: "))
    {
        UINT a = 0;
        scanf_s("%d", &a);
        hr = pFactory->EnumAdapters(a, &pAdapterIN);
        if (a >= 0 && SUCCEEDED(hr))
        {
            DXGI_ADAPTER_DESC desc;
            pAdapterIN->GetDesc(&desc);
            selAdpName = (char*) malloc(200);
            ZeroMemory(selAdpName, 200);
            sprintf(selAdpName, "%ls", desc.Description);
            break;
        }
    }
    return TRUE;
}

CHAR* VideoDXGICaptor::GetSelAdpName()
{
    return selAdpName;
}

UINT VideoDXGICaptor::GetAccumulatedFrames()
{
    return frameInfo.AccumulatedFrames;
}

UINT VideoDXGICaptor::GetTotalMetadataBufferSize()
{
    return frameInfo.TotalMetadataBufferSize;
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
        if (numAdp <= 1)  hr = D3D11CreateDevice(NULL, DriverTypes[DriverTypeIndex], NULL, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &m_hDevice, &FeatureLevel, &m_hContext);
        else hr = D3D11CreateDevice(pAdapterIN, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, &m_hDevice, &FeatureLevel, &m_hContext);

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
    hr = hDxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&hDxgiOutput1));
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

    if(m_hDeskDupl == NULL)
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

BOOL VideoDXGICaptor::CaptureImage(void **pData)
{
    return QueryFrame(pData);
}
BOOL VideoDXGICaptor::ResetDevice()
{
    Deinit();
    return Init();
}
BOOL VideoDXGICaptor::QueryFrame(void **pImgData)
{

    if (!m_dxgiOutDesc.AttachedToDesktop)
    {
        return FALSE;
    }

    IDXGIResource *hDesktopResource = NULL;
    //DXGI_OUTDUPL_FRAME_INFO FrameInfo;
    HRESULT hr = m_hDeskDupl->AcquireNextFrame(200, &frameInfo, &hDesktopResource);
    if (FAILED(hr))
    {
        return FALSE;
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
        //memcpy((BYTE*)pImgData, mappedRect.pBits, m_dxgiOutDesc.DesktopCoordinates.right * m_dxgiOutDesc.DesktopCoordinates.bottom * 4);
        hStagingSurf->Unmap();
        *pImgData = mappedRect.pBits;
    }

    RESET_OBJECT(hStagingSurf);
    return SUCCEEDED(hr);
}


void * CaptureInternal(void)
{
    static bool first = TRUE;
    static bool dc_capture = FALSE;
    static void *pImgData = nullptr;
    if(first == TRUE)
    {
        BOOL result = dxgi_capt->Init();
        if (result == FALSE)
        {
            printf("\nDirect3D Initial Failed\nMissing Desktop Duplication API\n");
            dc_capture = TRUE;
            first = FALSE;
            return NULL;
        }
        //pImgData = malloc(9999999);
        first = FALSE;
        printf("\nFirst Initial Succeed\n");
        Sleep(200);  //perhaps found some reasons, but still not a perfect solution
        printf("First Sleeping\n");
    }

    if(dc_capture == TRUE)
    {
        return NULL;
    }

    if(!dxgi_capt->CaptureImage(&pImgData))
    {
        return NULL;
    }
    //delete CaptureTest;
    //free(pImgData);
    return pImgData;
}