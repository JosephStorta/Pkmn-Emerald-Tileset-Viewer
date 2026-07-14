#include "gui/tileset_view.h"

#include <filesystem>

#include <logger/logger.h>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dirdlg.h>
#include <wx/spinctrl.h>

#include "data/parser.h"
#include "data/tileset.h"

namespace viewer {
namespace gui {

SET_LOG_MODULE("GUI");

TilesetView::TilesetView(wxStaticBox* box, int orient)
    : wxStaticBoxSizer(box, orient)
{
    create_gui();
}

void TilesetView::load_tileset()
{
    LOG_DEBUG("Showing file dialog...");

    wxDirDialog dialog(
        this->GetContainingWindow(),
        "Select tileset folder",
        std::filesystem::canonical("./res").string(),
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST
    );
    dialog.CenterOnParent();

    if (dialog.ShowModal() != wxID_OK)
    {   LOG_DEBUG("File select cancelled");
        return;
    }

    std::filesystem::path tileset_path { dialog.GetPath().ToStdString() };
    data::Tileset tileset { *data::Parser::parse_tileset(tileset_path) };

    LOG_DEBUG("Tileset loading complete");

    wxImage tileset_image(tileset.image.width, tileset.image.height, tileset.image.data, true);
    tileset_image = tileset_image.Scale(tileset.image.width * 2, tileset.image.height * 2);

    if (!m_tileset_bitmap)
    {
        m_tileset_bitmap = new wxStaticBitmap(m_tileset_scroll, wxID_ANY, tileset_image);
    }

    GetStaticBox()->SetLabelText("Tileset: " + tileset_path.filename().string());
}

void TilesetView::create_gui()
{
    // --- Image Display --- //

    // Window to allow image scrolling
	m_tileset_scroll = new wxScrolledWindow(
        GetStaticBox(),
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxBORDER_SUNKEN | wxHSCROLL | wxVSCROLL
    );
    m_tileset_scroll->SetMinSize( wxSize(256, -1) );
    m_tileset_scroll->SetScrollRate(5, 5);
    m_tileset_scroll->SetBackgroundColour(*wxBLACK);

    // Add to sizer
    Add(m_tileset_scroll, 1, wxALL | wxEXPAND);

    // --- View Options --- //

    // Panel to contain option widgets
    wxPanel* tileset_options_panel {
        new wxPanel(
            GetStaticBox(),
            wxID_ANY
        )
    };

    // Add to sizer
    Add(tileset_options_panel, 0, wxEXPAND, 5);

    // Sizer for option widgets
    wxBoxSizer* tileset_options_sizer { new wxBoxSizer(wxHORIZONTAL) };
    tileset_options_panel->SetSizer(tileset_options_sizer);

    // Checkbox for showing palette colors
    wxCheckBox* apply_palette_check {
        new wxCheckBox(
            tileset_options_panel,
            wxID_ANY,
            "Apply Palette?",
            wxDefaultPosition,
            wxDefaultSize,
            wxALIGN_RIGHT
        )
    };

    // Add to sizer
    tileset_options_sizer->Add(apply_palette_check, 2, wxALL | wxEXPAND, 5);

    // Add a separator to the sizer
	tileset_options_sizer->Add(0, 0, 1, wxEXPAND, 5);

    // --- Palette Select --- //

    // Sizer for palette selector widgets
    wxBoxSizer* palette_num_sizer { new wxBoxSizer(wxHORIZONTAL) };

    // Add to sizer
    tileset_options_sizer->Add(palette_num_sizer, 2, wxEXPAND, 5);

    // Label for the spinbox
    wxStaticText* palette_num_label {
        new wxStaticText(
            tileset_options_panel,
            wxID_ANY,
            "Palette:",
            wxDefaultPosition,
            wxDefaultSize,
            wxALIGN_RIGHT
        )
    };
    palette_num_label->Wrap(-1);

    // Add to sizer
    palette_num_sizer->Add(palette_num_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    // Spinbox to display palette number
    wxSpinCtrl* palette_num_spinbox {
        new wxSpinCtrl(
            tileset_options_panel,
            wxID_ANY,
            wxEmptyString,
            wxDefaultPosition,
            wxDefaultSize,
            wxSP_ARROW_KEYS,
            0,
            10,
            0
        )
    };
    palette_num_spinbox->SetMinSize( wxSize(48, -1) );

    // Add to sizer
	palette_num_sizer->Add(palette_num_spinbox, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
}

} // namespace gui
} // namespace viewer
