/////////////////////////////////////////////////////////////////////////////
// Name:        imaglist.cpp
// Purpose:     wxImageList
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "imaglist.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if defined(__WIN95__)

#ifndef WX_PRECOMP
#include <stdio.h>
#include "wx/setup.h"
#include "wx/window.h"
#include "wx/icon.h"
#include "wx/dc.h"
#include "wx/string.h"
#endif

#include "wx/log.h"
#include "wx/intl.h"

#include "wx/msw/imaglist.h"
#include "wx/msw/private.h"

#if (defined(__WIN95__) && !defined(__GNUWIN32__)) || defined(__TWIN32__) || defined(wxUSE_NORLANDER_HEADERS)
#include <commctrl.h>
#endif

IMPLEMENT_DYNAMIC_CLASS(wxImageList, wxObject)

wxImageList::wxImageList(void)
{
    m_hImageList = 0;
}

wxImageList::~wxImageList(void)
{
	if ( m_hImageList )
		ImageList_Destroy((HIMAGELIST) m_hImageList);
	m_hImageList = 0;
}


// Attributes
////////////////////////////////////////////////////////////////////////////

// Returns the number of images in the image list.
int wxImageList::GetImageCount(void) const
{
	return ImageList_GetImageCount((HIMAGELIST) m_hImageList);
}

// Operations
////////////////////////////////////////////////////////////////////////////

// Creates an image list
bool wxImageList::Create(int width, int height, bool mask, int initial)
{
	UINT flags = 0;
	if ( mask )
		flags |= ILC_MASK;

	// Grow by 1, I guess this is reasonable behaviour most of the time
	m_hImageList = (WXHIMAGELIST) ImageList_Create(width, height, flags, initial, 1);
	return (m_hImageList != 0);
}

// Adds a bitmap, and optionally a mask bitmap.
// Note that wxImageList creates new bitmaps, so you may delete
// 'bitmap' and 'mask'.
int wxImageList::Add(const wxBitmap& bitmap, const wxBitmap& mask)
{
	HBITMAP hBitmap1 = (HBITMAP) bitmap.GetHBITMAP();
	HBITMAP hBitmap2 = 0;
	if ( mask.Ok() )
	    hBitmap2 = (HBITMAP) mask.GetHBITMAP();
    else if (bitmap.GetMask())
        hBitmap2 = (HBITMAP) bitmap.GetMask()->GetMaskBitmap();

    HBITMAP hBitmapI=0;
    if(hBitmap2!=0) {
        // Microsoft imagelist masks are inverted from wxWindows mask standard (white is mask color)
        BITMAP bm;
        ::GetObject(hBitmap2,sizeof(BITMAP),(LPVOID)&bm);
        int w=bm.bmWidth;
        int h=bm.bmHeight;
        HDC hdc = ::CreateCompatibleDC(NULL);   
        HDC hdci = ::CreateCompatibleDC(NULL);  
        hBitmapI = ::CreateCompatibleBitmap(hdci, w, h);
        ::SelectObject(hdc, hBitmap2);
        ::SelectObject(hdci, hBitmapI);
        ::BitBlt(hdci, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
        ::DeleteDC(hdc);
        ::DeleteDC(hdci);
    }

    int index = ImageList_Add((HIMAGELIST) GetHIMAGELIST(), hBitmap1, hBitmapI);
	if ( index == -1 )
    {
        wxLogError(_("Couldn't add an image to the image list."));
    }

    // Clean up inverted mask
    if(hBitmapI!=0)
        ::DeleteObject(hBitmapI);

    return index;
}

// Adds a bitmap, using the specified colour to create the mask bitmap
// Note that wxImageList creates new bitmaps, so you may delete
// 'bitmap'.
int wxImageList::Add(const wxBitmap& bitmap, const wxColour& maskColour)
{
#ifdef __TWIN32__
        wxFAIL_MSG("ImageList_AddMasked not implemented in TWIN32");
        return -1;
#else
	HBITMAP hBitmap1 = (HBITMAP) bitmap.GetHBITMAP();
	COLORREF colorRef = PALETTERGB(maskColour.Red(), maskColour.Green(), maskColour.Blue());
	return ImageList_AddMasked((HIMAGELIST) GetHIMAGELIST(), hBitmap1, colorRef);
#endif
}

// Adds a bitmap and mask from an icon.
int wxImageList::Add(const wxIcon& icon)
{
	HICON hIcon = (HICON) icon.GetHICON();
	return ImageList_AddIcon((HIMAGELIST) GetHIMAGELIST(), hIcon);
}

// Replaces a bitmap, optionally passing a mask bitmap.
// Note that wxImageList creates new bitmaps, so you may delete
// 'bitmap' and 'mask'.
bool wxImageList::Replace(int index, const wxBitmap& bitmap, const wxBitmap& mask)
{
#ifdef __TWIN32__
        wxFAIL_MSG("ImageList_Replace not implemented in TWIN32");
        return FALSE;
#else
	HBITMAP hBitmap1 = (HBITMAP) bitmap.GetHBITMAP();
	HBITMAP hBitmap2 = 0;
	if ( mask.Ok() )
	    hBitmap2 = (HBITMAP) mask.GetHBITMAP();
	return (ImageList_Replace((HIMAGELIST) GetHIMAGELIST(), index, hBitmap1, hBitmap2) != 0);
#endif
}

/* Not supported by Win95
// Replacing a bitmap, using the specified colour to create the mask bitmap
// Note that wxImageList creates new bitmaps, so you may delete
// 'bitmap'.
bool wxImageList::Replace(int index, const wxBitmap& bitmap, const wxColour& maskColour)
{
	HBITMAP hBitmap1 = (HBITMAP) bitmap.GetHBITMAP();
	COLORREF colorRef = PALETTERGB(maskColour.Red(), maskColour.Green(), maskColour.Blue());
	return (bool) ImageList_ReplaceMasked((HIMAGELIST) GetHIMAGELIST(), index, hBitmap1, colorRef);
}
*/

// Replaces a bitmap and mask from an icon.
bool wxImageList::Replace(int index, const wxIcon& icon)
{
	HICON hIcon = (HICON) icon.GetHICON();
	return (ImageList_ReplaceIcon((HIMAGELIST) GetHIMAGELIST(), index, hIcon) != 0);
}

// Removes the image at the given index.
bool wxImageList::Remove(int index)
{
#ifdef __TWIN32__
        wxFAIL_MSG("ImageList_Replace not implemented in TWIN32");
        return FALSE;
#else
	return (ImageList_Remove((HIMAGELIST) GetHIMAGELIST(), index) != 0);
#endif
}

// Remove all images
bool wxImageList::RemoveAll(void)
{
	// TODO: Is this correct?
	while ( GetImageCount() > 0 )
	{
		Remove(0);
	}
	return TRUE;
}

// Draws the given image on a dc at the specified position.
// If 'solidBackground' is TRUE, Draw sets the image list background
// colour to the background colour of the wxDC, to speed up
// drawing by eliminating masked drawing where possible.
bool wxImageList::Draw(int index, wxDC& dc, int x, int y,
    int flags, bool solidBackground)
{
#ifdef __TWIN32__
        wxFAIL_MSG("ImageList_Replace not implemented in TWIN32");
        return FALSE;
#else
	HDC hDC = (HDC) dc.GetHDC();
	if ( !hDC )
		return FALSE;

	if ( solidBackground )
	{
		wxBrush *brush = & dc.GetBackground();
		if ( brush && brush->Ok())
		{
			wxColour col(brush->GetColour());
			ImageList_SetBkColor((HIMAGELIST) GetHIMAGELIST(),
					PALETTERGB(col.Red(), col.Green(), col.Blue()));
		}
		else
			ImageList_SetBkColor((HIMAGELIST) GetHIMAGELIST(),
				CLR_NONE);
	}
	else
		ImageList_SetBkColor((HIMAGELIST) GetHIMAGELIST(),
				CLR_NONE);

    UINT style = 0;
	if ( flags & wxIMAGELIST_DRAW_NORMAL )
		style |= ILD_NORMAL;
	if ( flags & wxIMAGELIST_DRAW_TRANSPARENT )
		style |= ILD_TRANSPARENT;
	if ( flags & wxIMAGELIST_DRAW_SELECTED )
		style |= ILD_SELECTED;
	if ( flags & wxIMAGELIST_DRAW_FOCUSED )
		style |= ILD_FOCUS;

	return (ImageList_Draw((HIMAGELIST) GetHIMAGELIST(), index, hDC,
		x, y, style) != 0);
#endif
}

#endif

