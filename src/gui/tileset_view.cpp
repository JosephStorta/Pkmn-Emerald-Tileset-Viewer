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

TilesetView::TilesetView(wxWindow* parent)
    : wxPanel(parent)
{
    // Enable image support
    wxInitAllImageHandlers();

    create_gui();

    //Bind events
    m_apply_palette_check->Bind(wxEVT_CHECKBOX, &TilesetView::on_palette_check, this);
    m_palette_num_spinbox->Bind(wxEVT_SPINCTRL, &TilesetView::on_palette_spin, this);
}

/**
 * @brief Prompts the user for a tileset folder to load.
 */
void TilesetView::load_tileset()
{
    LOG_DEBUG("Showing file dialog...");

    wxDirDialog dialog(
        this,
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

    m_tileset_view_sizer->GetStaticBox()->SetLabelText("Tileset: " + tileset_path.filename().string());
}

/**
 * @brief Triggered when the "Apply Palette" checkbox is toggled.
 * @param event 
 */
void TilesetView::on_palette_check(wxCommandEvent& event)
{
    LOG_INFO("Palette application toggled");
}

/**
 * @brief Triggered when the "Palette" spin control is spun.
 * @param event 
 */
void TilesetView::on_palette_spin(wxSpinEvent& event)
{
    LOG_INFO("Palette number changed");
}

/**
 * @brief Creates and arranges GUI widgets.
 */
void TilesetView::create_gui()
{
    // --- Top-level --- //

    SetMinSize( wxSize(270, 571) );
    SetMaxSize( wxSize(270, 571) );

    m_tileset_view_sizer = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, "Tileset"),
        wxVERTICAL
    );
    SetSizer(m_tileset_view_sizer);

    // --- Image Display --- //

    // Window to allow image scrolling
	m_tileset_scroll = new wxScrolledWindow(
        this,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxBORDER_SUNKEN | wxHSCROLL | wxVSCROLL
    );
    m_tileset_scroll->SetBackgroundColour(*wxBLACK);
    m_tileset_scroll->SetMinSize( wxSize(260, 516) );
    m_tileset_scroll->SetMaxSize( wxSize(260, 516) );
    m_tileset_scroll->SetScrollRate(5, 5);

    // Add to sizer
    m_tileset_view_sizer->Add(m_tileset_scroll, 1, wxEXPAND);

    // --- View Options --- //

    // Panel to contain option widgets
    wxPanel* tileset_options_panel {
        new wxPanel(
            this,
            wxID_ANY
        )
    };

    // Add to sizer
    m_tileset_view_sizer->Add(tileset_options_panel, 0, wxEXPAND);

    // Sizer for option widgets
    wxBoxSizer* tileset_options_sizer { new wxBoxSizer(wxHORIZONTAL) };
    tileset_options_panel->SetSizer(tileset_options_sizer);

    // Checkbox for showing palette colors
    m_apply_palette_check = new wxCheckBox(
        tileset_options_panel,
        wxID_ANY,
        "Apply Palette?",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_RIGHT
    );

    // Add to sizer
    tileset_options_sizer->Add(m_apply_palette_check, 2, wxEXPAND);

    // Add a separator to the sizer
	tileset_options_sizer->Add(0, 0, 1, wxEXPAND);

    // --- Palette Select --- //

    // Sizer for palette selector widgets
    wxBoxSizer* palette_num_sizer { new wxBoxSizer(wxHORIZONTAL) };

    // Add to sizer
    tileset_options_sizer->Add(palette_num_sizer, 2, wxALIGN_CENTER_VERTICAL | wxRIGHT);

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
    palette_num_label->Wrap(-1); // Disable text wrapping

    // Add to sizer
    palette_num_sizer->Add(palette_num_label, 1, wxALIGN_CENTER_VERTICAL);

    // Spinbox to display palette number
    m_palette_num_spinbox = new wxSpinCtrl(
        tileset_options_panel,
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxSP_ARROW_KEYS,
        0,
        15,
        0
    );
    m_palette_num_spinbox->SetMinSize( wxSize(48, -1) );

    // Add to sizer
	palette_num_sizer->Add(m_palette_num_spinbox, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
}

} // namespace gui
} // namespace viewer
