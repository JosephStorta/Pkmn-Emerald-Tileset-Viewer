#include "gui/metatile_view.h"

#include <format>
#include <vector>

#include <spdlog/spdlog.h>

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

    wxImage tileset_image(tileset->image.width, tileset->image.height, tileset->image.data, true);
    m_metatile_image = wxImage(128, ((int)(tileset->metatiles.size() / 8) + 1) * 16, true);

    spdlog::debug("Metatile count: {}", tileset->metatiles.size());
    for (int i = 0; i < tileset->metatiles.size(); i++)
    {
        m_metatile_image.Paste(
            create_metatile(tileset->metatiles[i], tileset_image, tileset),
            (i % 8) * 16,
            (int)(i / 8) * 16
        );
    }

    m_metatile_image = m_metatile_image.Rescale(256, m_metatile_image.GetHeight() * 2);

    // wxWidgets gives an error if a wxStaticBitmap is defined without a valid image,
    // so we wait until a tileset is loaded to define it.
    if (!m_metatile_bitmap)
    {
        m_metatile_bitmap = new wxStaticBitmap(
            m_bitmap_scroll,
            wxID_ANY,
            m_metatile_image
        );
    }

    m_bitmap_scroll->SetVirtualSize(m_metatile_image.GetWidth(), m_metatile_image.GetHeight());
    m_bitmap_scroll->SetScrollRate(5, 5);
}

wxImage MetatileView::create_metatile(const data::Metatile& metatile_data, const wxImage& tileset_image, const data::Tileset* tileset_data)
{
    wxImage metatile_image(16, 16, true);

    for (int i = 0; i < metatile_data.background.size(); i++)
    {
        data::Tile tile_data { metatile_data.background[i] };

        wxImage tile = tileset_image.GetSubImage(
            wxRect(
                (tile_data.index % 16) * 8,
                (int)(tile_data.index / 16) * 8 + ( tile_data.bank ? 128 : 0 ),
                8,
                8
            )
        );

        data::Palette palette { tileset_data->palettes[tile_data.palette_index] };
        
        for (int i = 0; i < palette.colors.size(); i++)
        {
            int gray_value { data::grayscale_palette[i] };
            data::Color palette_color { palette.colors[i] };

            // Replace each grayscale value with the corresponding palette color.
            tile.Replace(
                gray_value, gray_value, gray_value,
                palette_color.red, palette_color.green, palette_color.blue
            );
        }

        if (tile_data.h_flip)
        {
            tile = tile.Mirror();
        }

        if (tile_data.v_flip)
        {
            tile = tile.Mirror(false);
        }

        metatile_image.Paste(tile, (i % 2) * 8, (int)(i / 2) * 8);
    }

    for (int i = 0; i < metatile_data.foreground.size(); i++)
    {
        data::Tile tile_data { metatile_data.foreground[i] };

        wxImage tile = tileset_image.GetSubImage(
            wxRect(
                (tile_data.index % 16) * 8,
                (int)(tile_data.index / 16) * 8 + ( tile_data.bank ? 128 : 0 ),
                8,
                8
            )
        );

        data::Palette palette { tileset_data->palettes[tile_data.palette_index] };
        
        for (int i = 0; i < palette.colors.size(); i++)
        {
            int gray_value { data::grayscale_palette[i] };
            data::Color palette_color { palette.colors[i] };

            // Replace each grayscale value with the corresponding palette color.
            tile.Replace(
                gray_value, gray_value, gray_value,
                palette_color.red, palette_color.green, palette_color.blue
            );

            // The first palette color is used for transparency
            if (i == 0)
            {
                tile.SetMaskColour(palette_color.red, palette_color.green, palette_color.blue);
            }
        }

        if (tile_data.h_flip)
        {
            tile = tile.Mirror();
        }

        if (tile_data.v_flip)
        {
            tile = tile.Mirror(false);
        }

        metatile_image.Paste(tile, (i % 2) * 8, (int)(i / 2) * 8);
    }

    return metatile_image;
}

void MetatileView::create_gui()
{
   // --- Top-level --- //

    SetMinSize( wxSize(270, 570) );
    SetMaxSize( wxSize(270, 570) );

    m_metatile_view_sizer = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, "Metatiles"),
        wxVERTICAL
    );
    SetSizer(m_metatile_view_sizer);

    // --- Image Display --- //

    // Panel to contain the metatile image
	m_bitmap_scroll = new wxScrolledWindow(
        this,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxBORDER_SUNKEN | wxVSCROLL
    );
    m_bitmap_scroll->SetBackgroundColour(*wxBLACK);

    m_bitmap_scroll->SetMinSize( wxSize(260, 560) );
    m_bitmap_scroll->SetMaxSize( wxSize(260, 560) );

    // Add to sizer
    m_metatile_view_sizer->Add(m_bitmap_scroll, 1, wxEXPAND);
}

} // namespace gui
} // namespace viewer
