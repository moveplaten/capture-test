#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
bool moving;
class VideoDXGICaptor
{
public:
    VideoDXGICaptor();
    ~VideoDXGICaptor();

public:
    BOOL Init();
    VOID Deinit();

public:
    virtual BOOL CaptureImage(void *pData);
    virtual BOOL ResetDevice();

private:
    BOOL  QueryFrame(void *pImgData);

private:
    BOOL                    m_bInit;

    ID3D11Device           *m_hDevice;
    ID3D11DeviceContext    *m_hContext;

    IDXGIOutputDuplication *m_hDeskDupl;
    DXGI_OUTPUT_DESC        m_dxgiOutDesc;
};
