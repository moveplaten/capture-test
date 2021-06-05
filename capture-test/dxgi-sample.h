#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>

class VideoDXGICaptor
{
public:
    VideoDXGICaptor();
    ~VideoDXGICaptor();

public:
    BOOL Init();
    VOID Deinit();
    CHAR* GetSelAdpName();
    UINT GetAccumulatedFrames();
    UINT GetTotalMetadataBufferSize();

public:
    virtual BOOL CaptureImage(void **pData);
    virtual BOOL SelectAdapters(void);
    virtual BOOL ResetDevice();

private:
    BOOL  QueryFrame(void **pImgData);

private:
    BOOL                    m_bInit;

    ID3D11Device           *m_hDevice;
    ID3D11DeviceContext    *m_hContext;

    IDXGIOutputDuplication *m_hDeskDupl;
    DXGI_OUTPUT_DESC        m_dxgiOutDesc;

    UINT                    numAdp;
    IDXGIAdapter           *pAdapterIN;
    CHAR                   *selAdpName;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;
};
