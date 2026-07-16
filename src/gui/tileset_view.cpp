#include "gui/tileset_view.h"

#include <filesystem>
#include <format>
#include <vector>

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
    m_palette_check->Bind(wxEVT_CHECKBOX, &TilesetView::on_palette_check, this);
    m_palette_spinbox->Bind(wxEVT_SPINCTRL, &TilesetView::on_palette_spin, this);
    
    update_spinbox();  
}

/**
 * @brief Prompts the user for a tileset folder to load.
 */
data::Tileset* TilesetView::load_tileset()
{
    LOG_DEBUG("Showing file dialog...");

    // wxDirDialog selects a directory rather than a file
    wxDirDialog dialog(
        this,
        "Select tileset folder",
        std::filesystem::canonical("./res").string(),
        wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST
    );
    dialog.CenterOnParent();

    if (dialog.ShowModal() != wxID_OK)
    {   LOG_DEBUG("File select cancelled");
        return nullptr;
    }

    // Parse the selected tileset
    std::filesystem::path tileset_path { dialog.GetPath().ToStdString() };
    m_tileset = data::Parser::parse_tileset(tileset_path);

    LOG_DEBUG("Tileset loading complete");

    // Retrieve the parsed tileset image
    m_tileset_image = wxImage(m_tileset->image.width, m_tileset->image.height, m_tileset->image.data, true);
    m_tileset_image = m_tileset_image.Scale(m_tileset->image.width * 2, m_tileset->image.height * 2);

    // wxWidgets gives an error if a wxStaticBitmap is defined without a valid image,
    // so we wait until a tileset is loaded to define it.
    if (!m_tileset_bitmap)
    {
        m_tileset_bitmap = new wxStaticBitmap(
            m_bitmap_panel,
            wxID_ANY,
            m_tileset_image
        );
    }

    // Add the tileset name to the sizer label
    m_tileset_view_sizer->GetStaticBox()->SetLabelText("Tileset: " + tileset_path.filename().string());

    update_palette();

    return m_tileset;
}

/**
 * @brief Triggered when the "Apply Palette" checkbox is toggled.
 * @param event 
 */
void TilesetView::on_palette_check(wxCommandEvent& event)
{
    LOG_INFO("Palette application toggled");

    update_spinbox();
    update_palette();
}

/**
 * @brief Triggered when the "Palette" spin control is spun.
 * @param event 
 */
void TilesetView::on_palette_spin(wxSpinEvent& event)
{
    LOG_INFO("Palette number changed");

    update_palette();
}

/**
 * @brief Enables or disables the palette spinbox based on the state of the apply palette checkbox.
 */
void TilesetView::update_spinbox()
{
    m_palette_spinbox->Enable(m_palette_check->IsChecked());
}

/**
 * @brief Applies a color palette to the tileset image.
 */
void TilesetView::update_palette()
{
    if (!m_tileset_bitmap)
    {
        return;
    }

    if (!m_palette_check->IsChecked())
    {
        m_tileset_bitmap->SetBitmap(m_tileset_image);
        return;
    }

    // Define a new image to retain an instance of the original grayscale image.
    wxImage palette_image { m_tileset_image };
    data::Palette palette { m_tileset->palettes[m_palette_spinbox->GetValue()] };
    
    for (int i = 0; i < palette.colors.size(); i++)
    {
        int gray_value { data::grayscale_palette[i] };
        data::Color palette_color { palette.colors[i] };

        // Replace each grayscale value with the corresponding palette color.
        palette_image.Replace(
            gray_value, gray_value, gray_value,
            palette_color.red, palette_color.green, palette_color.blue
        );
    }

    m_tileset_bitmap->SetBitmap(palette_image);
}

/**
 * @brief Creates and arranges GUI widgets.
 */
void TilesetView::create_gui()
{
    // --- Top-level --- //

    // The max tileset size is 128x256. The display doubles that to 256x512.
    // The StaticBox defined below adds a 5px margin on each side,
    // except for the top which adds 17px due to the title label.
    // The sunken border of the bitmap panel adds a 2px margin on each side.
    // The options panel adds an additional 34px to the height.
    // 256 + (5 + 5) + (2 + 2) = 270
    // 512 + (17 + 5) + (2 + 2) + 34 = 572
    SetMinSize( wxSize(270, 572) );
    SetMaxSize( wxSize(270, 572) );

    m_tileset_view_sizer = new wxStaticBoxSizer(
        new wxStaticBox(this, wxID_ANY, "Tileset"),
        wxVERTICAL
    );
    SetSizer(m_tileset_view_sizer);

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
    m_bitmap_panel->SetMinSize( wxSize(260, 516) );
    m_bitmap_panel->SetMaxSize( wxSize(260, 516) );

    // Add to sizer
    m_tileset_view_sizer->Add(m_bitmap_panel, 1, wxEXPAND);

    // --- View Options --- //

    // Panel to contain option widgets
    wxPanel* tileset_options_panel {
        new wxPanel(
            this,
            wxID_ANY
        )
    };
    tileset_options_panel->SetMinSize( wxSize(-1, 34) );

    // Add to sizer
    m_tileset_view_sizer->Add(tileset_options_panel, 0, wxEXPAND);

    // Sizer for option widgets
    wxBoxSizer* tileset_options_sizer { new wxBoxSizer(wxHORIZONTAL) };
    tileset_options_panel->SetSizer(tileset_options_sizer);

    // Checkbox for showing palette colors
    m_palette_check = new wxCheckBox(
        tileset_options_panel,
        wxID_ANY,
        "Apply Palette?",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_RIGHT
    );

    // Add to sizer
    tileset_options_sizer->Add(m_palette_check, 2, wxEXPAND);

    // Add a separator to the sizer
	tileset_options_sizer->Add(0, 0, 1, wxEXPAND);

    // --- Palette Select --- //

    // Sizer for palette selector widgets
    wxBoxSizer* palette_num_sizer { new wxBoxSizer(wxHORIZONTAL) };

    // Add to sizer
    tileset_options_sizer->Add(palette_num_sizer, 2, wxALIGN_CENTER_VERTICAL);

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
    m_palette_spinbox = new wxSpinCtrl(
        tileset_options_panel,
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxSP_ARROW_KEYS | wxSP_WRAP,
        0,
        15,
        0
    );
    m_palette_spinbox->SetMinSize( wxSize(48, -1) );  

    // Add to sizer
	palette_num_sizer->Add(m_palette_spinbox, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
}

} // namespace gui
} // namespace viewer
