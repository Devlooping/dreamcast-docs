/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dcalls.c
 *
 *  Calls to Direct3D objects needed for rendering.  Part of D3DApp.
 *
 *  D3DApp is a collection of helper functions for Direct3D applications.
 *  D3DApp consists of the following files:
 *	d3dapp.h    Main D3DApp header to be included by application
 *      d3dappi.h   Internal header
 *      d3dapp.c    D3DApp functions seen by application.
 *      ddcalls.c   All calls to DirectDraw objects except textures
 *      d3dcalls.c  All calls to Direct3D objects except textures
 *      texture.c   Texture loading and managing texture list
 *      misc.c	    Miscellaneous calls
 */

#define INITGUID
#include "d3dappi.h"

/***************************************************************************/
/*                            Creation of D3D                              */
/***************************************************************************/
BOOL
D3DAppICreateD3D(void)
{
    LastError = d3dappi.lpDD->lpVtbl->QueryInterface(d3dappi.lpDD,
				    &IID_IDirect3D2, (LPVOID*)&d3dappi.lpD3D);
    if (LastError != DD_OK) {
        D3DAppISetErrorString("Creation of IDirect3D failed.\n%s",
			      D3DAppErrorToString(LastError));
	goto exit_with_error;
    }
    return TRUE;
exit_with_error:
    return FALSE;
}

/***************************************************************************/
/*                           D3D Device Enumeration                        */
/***************************************************************************/
/*
 * enumDeviceFunc
 * Device enumeration callback.  Record information about the D3D device
 * reported by D3D.
 */
static BOOL bRampHasEnumerated = FALSE;
static HRESULT
WINAPI enumDeviceFunc(LPGUID lpGuid, LPSTR lpDeviceDescription,
		      LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc,
		      LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext)
{
    lpContext = lpContext;
/*	if (! bRampHasEnumerated)
	{
		bRampHasEnumerated = TRUE;
		return D3DENUMRET_OK;
	}*/
    /*
     * Don't accept any hardware D3D devices if emulation only option is set
     */
    if (lpHWDesc->dcmColorModel && d3dappi.bOnlyEmulation)
	return D3DENUMRET_OK;
    /*
     * Record the D3D driver's inforamation
     */
    memcpy(&d3dappi.Driver[d3dappi.NumDrivers].Guid, lpGuid, sizeof(GUID));
    lstrcpy(d3dappi.Driver[d3dappi.NumDrivers].About, lpDeviceDescription);
    lstrcpy(d3dappi.Driver[d3dappi.NumDrivers].Name, lpDeviceName);
    /*
     * Is this a hardware device or software emulation?  Checking the color
     * model for a valid model works.
     */
    if (lpHWDesc->dcmColorModel) {
	d3dappi.Driver[d3dappi.NumDrivers].bIsHardware = TRUE;
	memcpy(&d3dappi.Driver[d3dappi.NumDrivers].Desc, lpHWDesc,
	       sizeof(D3DDEVICEDESC));
    } else {
	d3dappi.Driver[d3dappi.NumDrivers].bIsHardware = FALSE;
	memcpy(&d3dappi.Driver[d3dappi.NumDrivers].Desc, lpHELDesc,
	       sizeof(D3DDEVICEDESC));
    }
    /*
     * Does this driver do texture mapping?
     */
    d3dappi.Driver[d3dappi.NumDrivers].bDoesTextures =
	(d3dappi.Driver[d3dappi.NumDrivers].Desc.dpcTriCaps.dwTextureCaps &
	 D3DPTEXTURECAPS_PERSPECTIVE) ? TRUE : FALSE;
    /*
     * Can this driver use a z-buffer?
     */
    d3dappi.Driver[d3dappi.NumDrivers].bDoesZBuffer =
	d3dappi.Driver[d3dappi.NumDrivers].Desc.dwDeviceZBufferBitDepth
		? TRUE : FALSE;
    /*
     * Can this driver render to the Windows display depth
     */
    d3dappi.Driver[d3dappi.NumDrivers].bCanDoWindow =
	(d3dappi.Driver[d3dappi.NumDrivers].Desc.dwDeviceRenderBitDepth &
	 D3DAppIBPPToDDBD(d3dappi.WindowsDisplay.bpp)) ? TRUE : FALSE;
    if (!d3dappi.bIsPrimary)
	d3dappi.Driver[d3dappi.NumDrivers].bCanDoWindow = FALSE;

    d3dappi.NumDrivers++;
    if (d3dappi.NumDrivers == D3DAPP_MAXD3DDRIVERS)
        return (D3DENUMRET_CANCEL);
    return (D3DENUMRET_OK);
}

/*
 * D3DAppIEnumDevices
 * Get the available drivers from Direct3D by enumeration.
 */
BOOL
D3DAppIEnumDevices(void)
{
    d3dappi.NumDrivers = 0;
    LastError = d3dappi.lpD3D->lpVtbl->EnumDevices(d3dappi.lpD3D,
						   enumDeviceFunc, NULL);
    if (LastError != DD_OK) {
        D3DAppISetErrorString("Enumeration of drivers failed.\n%s",
			      D3DAppErrorToString(LastError));
	return FALSE;
    }
    d3dappi.CurrDriver = 0;
    return TRUE;
}

/***************************************************************************/
/*                    Enumeration of texure format                         */
/***************************************************************************/
/*
 * EnumTextureFormatsCallback
 * Record information about each texture format the current D3D driver can
 * support. Choose one as the default format (paletted formats are prefered)
 * and return it through lpContext.
 */
static HRESULT
CALLBACK EnumTextureFormatsCallback(LPDDSURFACEDESC lpDDSD, LPVOID lpContext)
{
    unsigned long m;
    int r, g, b;
    int *lpStartFormat = (int *)lpContext;
    /*
     * Record the DDSURFACEDESC of this texture format
     */
    memset(&d3dappi.TextureFormat[d3dappi.NumTextureFormats], 0,
	   sizeof(D3DAppTextureFormat));
    memcpy(&d3dappi.TextureFormat[d3dappi.NumTextureFormats].ddsd, lpDDSD,
	   sizeof(DDSURFACEDESC));
    /*
     * Is this format palettized?  How many bits?  Otherwise, how many RGB
     * bits?
     */
    if (lpDDSD->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
        d3dappi.TextureFormat[d3dappi.NumTextureFormats].bPalettized = TRUE;
	d3dappi.TextureFormat[d3dappi.NumTextureFormats].IndexBPP = 8;
    } else if (lpDDSD->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED4) {
        d3dappi.TextureFormat[d3dappi.NumTextureFormats].bPalettized = TRUE;
	d3dappi.TextureFormat[d3dappi.NumTextureFormats].IndexBPP = 4;
    } else if (lpDDSD->ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS) {
		/*
		 * The sample apps don't currently understand
		 * the alpha bit - just filter this format
		 * away for now.
		 */

		return DDENUMRET_OK;
    } else
    {
        d3dappi.TextureFormat[d3dappi.NumTextureFormats].bPalettized = FALSE;
	d3dappi.TextureFormat[d3dappi.NumTextureFormats].IndexBPP = 0;
        for (r = 0, m = lpDDSD->ddpfPixelFormat.dwRBitMask; !(m & 1);
							       r++, m >>= 1);
        for (r = 0; m & 1; r++, m >>= 1);
        for (g = 0, m = lpDDSD->ddpfPixelFormat.dwGBitMask; !(m & 1);
							       g++, m >>= 1);
        for (g = 0; m & 1; g++, m >>= 1);
        for (b = 0, m = lpDDSD->ddpfPixelFormat.dwBBitMask; !(m & 1);
							       b++, m >>= 1);
        for (b = 0; m & 1; b++, m >>= 1);
	d3dappi.TextureFormat[d3dappi.NumTextureFormats].RedBPP = r;
	d3dappi.TextureFormat[d3dappi.NumTextureFormats].GreenBPP = g;
	d3dappi.TextureFormat[d3dappi.NumTextureFormats].BlueBPP = b;
    }
    /*
     * If lpStarFormat is -1, this is the first format.  Select it.
     */
    if (*lpStartFormat == -1)
        *lpStartFormat = d3dappi.NumTextureFormats;
    /* 
     * If this format is paletted, select it.
     */
    if (d3dappi.TextureFormat[d3dappi.NumTextureFormats].bPalettized) {
        *lpStartFormat = d3dappi.NumTextureFormats;
    }
    d3dappi.NumTextureFormats++;
    return DDENUMRET_OK;
}

/*
 * D3DAppIEnumTextureFormats
 * Get a list of available texture map formats from the Direct3D driver by
 * enumeration.  Choose a default format (paletted is prefered).
 */
BOOL
D3DAppIEnumTextureFormats(void)
{
    int StartFormat;
    /*
     * Set the default format to -1 to let the callback know it's being 
     * called for the first time.
     */
    StartFormat = -1;
    d3dappi.NumTextureFormats = 0;
    LastError =
	 d3dappi.lpD3DDevice->lpVtbl->EnumTextureFormats(d3dappi.lpD3DDevice,
						  EnumTextureFormatsCallback,
                                                  (LPVOID)&StartFormat);
    if (LastError != DD_OK) {
	D3DAppISetErrorString("Enumeration of texture formats failed.\n%s",
			      D3DAppErrorToString(LastError));
	return FALSE;
    }
    memcpy(&d3dappi.ThisTextureFormat, &d3dappi.TextureFormat[StartFormat],
	   sizeof(D3DAppTextureFormat));
    d3dappi.CurrTextureFormat = StartFormat;
    return TRUE;
}

/***************************************************************************/
/*                               Device creation                           */
/***************************************************************************/
/*
 * D3DAppICreateDevice
 * Create the D3D device and enumerate the texture formats
 */
BOOL
D3DAppICreateDevice(int driver)
{
    RELEASE(d3dappi.lpD3DDevice);

    if (d3dappi.Driver[driver].bIsHardware && !d3dappi.bBackBufferInVideo) {
	D3DAppISetErrorString("Could not fit the rendering surfaces in video memory for this hardware device.\n");
	goto exit_with_error;
    }

    d3dappi.CurrDriver = driver;
    memcpy(&d3dappi.ThisDriver, &d3dappi.Driver[driver], sizeof(D3DAppD3DDriver));
#if 1
    LastError = d3dappi.lpD3D->lpVtbl->CreateDevice(d3dappi.lpD3D, 
		&d3dappi.Driver[driver].Guid,
        d3dappi.lpBackBuffer, &d3dappi.lpD3DDevice);
#else
    LastError =
	   d3dappi.lpBackBuffer->lpVtbl->QueryInterface(d3dappi.lpBackBuffer,
						&d3dappi.Driver[driver].Guid,
					      (LPVOID*)&d3dappi.lpD3DDevice);
#endif
    if (LastError != DD_OK) {
    	D3DAppISetErrorString("Create D3D device failed.\n%s",
			      D3DAppErrorToString(LastError));
        goto exit_with_error;
    }
    d3dappi.CurrDriver = driver;
    d3dappi.NumTextureFormats = 0;
    if (d3dappi.Driver[driver].bDoesTextures) {
	if (!D3DAppIEnumTextureFormats())
	    goto exit_with_error;
    }

    return TRUE;
exit_with_error:
    RELEASE(d3dappi.lpD3DDevice);
    return FALSE;
}

/***************************************************************************/
/*                      Setting the render state                           */
/***************************************************************************/
/*
 * D3DAppISetRenderState
 * Create and execute an execute buffer which will set the render state and
 * light state for the current viewport.
 */
BOOL
D3DAppISetRenderState()
{
	LPDIRECT3DDEVICE2 pDev = d3dappi.lpD3DDevice;

    /*
     * If there is no D3D Viewport, we must return true because it is not
     * required.
     */
    if (!d3dappi.lpD3DViewport)
	return TRUE;
    LastError = pDev->lpVtbl->BeginScene(pDev);
    if (LastError != D3D_OK) {
        D3DAppISetErrorString("BeginScene failed in SetRenderState.\n%s",
                              D3DAppErrorToString(LastError));
        goto exit_with_error;
    }
	LastError = pDev->lpVtbl->SetCurrentViewport(pDev, d3dappi.lpD3DViewport);
	if (LastError !=D3D_OK) {
        D3DAppISetErrorString("SetViewport failed in SetRenderState.\n%s",
			      D3DAppErrorToString(LastError));
	goto exit_with_error;
    }
    /*
     * Set render state
     */
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_SHADEMODE, (DWORD)d3dapprs.ShadeMode);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_TEXTUREPERSPECTIVE, (DWORD)d3dapprs.bPerspCorrect);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_ZENABLE, (DWORD)(d3dapprs.bZBufferOn &&
                                  d3dappi.ThisDriver.bDoesZBuffer));
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_ZWRITEENABLE, (DWORD)d3dapprs.bZBufferOn);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_TEXTUREMAG, (DWORD)d3dapprs.TextureFilter);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_TEXTUREMIN, (DWORD)d3dapprs.TextureFilter);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_TEXTUREMAPBLEND, (DWORD)d3dapprs.TextureBlend);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_FILLMODE, (DWORD)d3dapprs.FillMode);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_DITHERENABLE, (DWORD)d3dapprs.bDithering);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_SPECULARENABLE, (DWORD)d3dapprs.bSpecular);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_ANTIALIAS, (DWORD)d3dapprs.bAntialiasing);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_FOGENABLE, (DWORD)d3dapprs.bFogEnabled);
    pDev->lpVtbl->SetRenderState(pDev, D3DRENDERSTATE_FOGCOLOR, (DWORD)d3dapprs.FogColor);
    /*
     * Set light state
     */
    pDev->lpVtbl->SetLightState(pDev, D3DLIGHTSTATE_FOGMODE, (DWORD)d3dapprs.bFogEnabled ?
                 d3dapprs.FogMode : D3DFOG_NONE);
    pDev->lpVtbl->SetLightState(pDev, D3DLIGHTSTATE_FOGSTART, *(unsigned long*)&d3dapprs.FogStart);
    pDev->lpVtbl->SetLightState(pDev, D3DLIGHTSTATE_FOGEND, *(unsigned long*)&d3dapprs.FogEnd);

    LastError = pDev->lpVtbl->EndScene(pDev);
    if (LastError != D3D_OK) {
        D3DAppISetErrorString("EndScene failed in SetRenderState.\n%s",
			      D3DAppErrorToString(LastError));
	goto exit_with_error;
    }
	return TRUE;
exit_with_error:
    return FALSE;
}

