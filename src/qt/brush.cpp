/////////////////////////////////////////////////////////////////////////////
// Name:        brush.cpp
// Purpose:     wxBrush
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "brush.h"
#endif

#include "wx/setup.h"
#include "wx/utils.h"
#include "wx/brush.h"

IMPLEMENT_DYNAMIC_CLASS(wxBrush, wxGDIObject)

wxBrushRefData::wxBrushRefData()
{
    m_style = wxSOLID;
// TODO: null data
}

wxBrushRefData::wxBrushRefData(const wxBrushRefData& data)
{
  m_style = data.m_style;
  m_stipple = data.m_stipple;
  m_colour = data.m_colour;
/* TODO: null data
  m_hBrush = 0;
*/
}

wxBrushRefData::~wxBrushRefData()
{
// TODO: delete data
}

// Brushes
wxBrush::wxBrush()
{
    if ( wxTheBrushList )
        wxTheBrushList->AddBrush(this);
}

wxBrush::~wxBrush()
{
    if ( wxTheBrushList )
        wxTheBrushList->RemoveBrush(this);
}

wxBrush::wxBrush(const wxColour& col, int Style)
{
    m_refData = new wxBrushRefData;

    M_BRUSHDATA->m_colour = col;
    M_BRUSHDATA->m_style = Style;

    RealizeResource();

    if ( wxTheBrushList )
        wxTheBrushList->AddBrush(this);
}

wxBrush::wxBrush(const wxString& col, int Style)
{
    m_refData = new wxBrushRefData;

    // Implicit conversion from string to wxColour via colour database
    M_BRUSHDATA->m_colour = col;
    M_BRUSHDATA->m_style = Style;

    RealizeResource();

    if ( wxTheBrushList )
        wxTheBrushList->AddBrush(this);
}

wxBrush::wxBrush(const wxBitmap& stipple)
{
    m_refData = new wxBrushRefData;

    M_BRUSHDATA->m_style = wxSTIPPLE;
    M_BRUSHDATA->m_stipple = stipple;

    RealizeResource();

    if ( wxTheBrushList )
        wxTheBrushList->AddBrush(this);
}

void wxBrush::Unshare()
{
	// Don't change shared data
	if (!m_refData)
    {
		m_refData = new wxBrushRefData();
	}
    else
    {
		wxBrushRefData* ref = new wxBrushRefData(*(wxBrushRefData*)m_refData);
		UnRef();
		m_refData = ref;
	}
}

void wxBrush::SetColour(const wxColour& col)
{
    Unshare();

    M_BRUSHDATA->m_colour = col;

    RealizeResource();
}

void wxBrush::SetColour(const wxString& col)
{
    Unshare();

    M_BRUSHDATA->m_colour = col;

    RealizeResource();
}

void wxBrush::SetColour(const unsigned char r, const unsigned char g, const unsigned char b)
{
    Unshare();

    M_BRUSHDATA->m_colour.Set(r, g, b);

    RealizeResource();
}

void wxBrush::SetStyle(int Style)
{
    Unshare();

    M_BRUSHDATA->m_style = Style;

    RealizeResource();
}

void wxBrush::SetStipple(const wxBitmap& Stipple)
{
    Unshare();

    M_BRUSHDATA->m_stipple = Stipple;

    RealizeResource();
}

void wxBrush::RealizeResource()
{
// TODO: create the brush
}

