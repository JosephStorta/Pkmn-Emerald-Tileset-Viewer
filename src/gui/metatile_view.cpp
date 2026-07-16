#include "gui/metatile_view.h"

#include <vector>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "data/tileset.h"

namespace viewer {
namespace gui {

MetatileView::MetatileView(wxWindow* parent)
    : wxPanel(parent)
{
    create_gui();
}

void MetatileView::load_metatiles(const data::Tileset* tileset)
{
    if (!tileset)
    {
        return;
    }

    for (data::Metatile metatile : tileset->metatiles)
    {
        wxImage background { create_metatile_layer(metatile.background) };
        wxImage foreground { create_metatile_layer(metatile.foreground) };
    }

    m_metatile_image = wxImage(tileset->image.width, tileset->image.height, tileset->image.data, true);

    // wxWidgets gives an error if a wxStaticBitmap is defined without a valid image,
    // so we wait until a tileset is loaded to define it.
    if (!m_metatile_bitmap)
    {
        m_metatile_bitmap = new wxStaticBitmap(
            m_bitmap_panel,
            wxID_ANY,
            m_metatile_image
        );
    }
}

wxImage MetatileView::create_metatile_layer(std::vector<data::Tile> tiles)
{
    return wxNullImage;
}

void MetatileView::create_gui()
{
   // --- Top-level --- //

    SetMinSize( wxSize(270, 572) );
    SetMaxSize( wxSize(270, 572) );

    m_metatile_view_sizer = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, "Metatiles"),
        wxVERTICAL
    );
    SetSizer(m_metatile_view_sizer);

    // --- Image Display --- //

    // Panel to contain the tileset image
	m_bitmap_panel = new wxPanel(
        this,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxBORDER_SUNKEN
    );
    m_bitmap_panel->SetBackgroundColour(*wxBLACK);

    // Sunken border adds a 2px margin.
    m_bitmap_panel->SetMinSize( wxSize(260, 562) );
    m_bitmap_panel->SetMaxSize( wxSize(260, 562) );

    // Add to sizer
    m_metatile_view_sizer->Add(m_bitmap_panel, 1, wxEXPAND);
}

} // namespace gui
} // namespace viewer
