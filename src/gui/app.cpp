#include "gui/app.h"

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

/**
 * @brief Helper enum to store menu item IDs.
 */
enum MenuItem
{
    Open = 1
};

/**
 * @brief Responsible for initializing the application.
 * @return True on successful initialization, false if otherwise.
 */
bool App::OnInit()
{
    LOG_INFO("Initializing GUI...");

    MainFrame* frame { new MainFrame() };
    frame->Show();
    return true;
}

/**
 * @brief Initializes the main GUI window.
 */
MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Pokemon Emerald Tileset Viewer")
{
    LOG_DEBUG("Creating main frame...");

    // GUI coding go brrrr

    // --- Top-Level --- //

    // Position the window in the center of the main display
	Center(wxBOTH);

    create_menu_bar();

    // Top-level sizer
    wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(main_sizer);

    // --- Tileset View --- //

    // Sizer for tileset view panel widgets
    wxBoxSizer* tileset_view_sizer = new wxBoxSizer(wxVERTICAL);

    // Panel responsible for displaying the loaded tileset and display options
    wxPanel* tileset_view_panel = new wxPanel(
        this,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxTAB_TRAVERSAL
    );
    tileset_view_panel->SetSizer(tileset_view_sizer);

    // Add to sizer
    main_sizer->Add(tileset_view_panel, 1, wxEXPAND | wxALL, 5);

    // --- Image Display --- //

    // Sizer for the tileset image display
    wxBoxSizer* image_sizer = new wxBoxSizer(wxVERTICAL);

    // Window to allow image scrolling
	wxScrolledWindow* tilset_scroll = new wxScrolledWindow(
        tileset_view_panel,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxBORDER_SUNKEN | wxHSCROLL | wxVSCROLL
    );
    tilset_scroll->SetScrollRate(5, 5);
    tilset_scroll->SetSizer(image_sizer);

    // Add to sizer
    tileset_view_sizer->Add(tilset_scroll, 1, wxALL | wxEXPAND, 5);
    
    // TODO: Create image display

    // --- View Options --- //

    // Sizer for option widgets
    wxStaticBoxSizer* tileset_options_sizer = new wxStaticBoxSizer(
        new wxStaticBox(tileset_view_panel, wxID_ANY, "Options"),
        wxHORIZONTAL
    );

    // Add to sizer
    tileset_view_sizer->Add(tileset_options_sizer, 0, wxEXPAND, 5);

    // Checkbox for showing palette colors
    wxCheckBox* apply_palette_check = new wxCheckBox(
        tileset_options_sizer->GetStaticBox(),
        wxID_ANY,
        "Apply Palette",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_RIGHT
    );

    // Add to sizer
    tileset_options_sizer->Add(apply_palette_check, 2, wxALL | wxEXPAND, 5);

    // Add a separator to the sizer
	tileset_options_sizer->Add(0, 0, 1, wxEXPAND, 5);

    // --- Palette Select --- //

    // Sizer for palette selector widgets
    wxBoxSizer* palette_num_sizer = new wxBoxSizer(wxHORIZONTAL);

    // Add to sizer
    tileset_options_sizer->Add(palette_num_sizer, 2, wxEXPAND, 5);

    // Label for the spinbox
    wxStaticText* palette_num_label = new wxStaticText(
        tileset_options_sizer->GetStaticBox(),
        wxID_ANY,
        "Palette",
        wxDefaultPosition,
        wxDefaultSize,
        wxALIGN_RIGHT
    );
    palette_num_label->Wrap(-1);

    // Add to sizer
    palette_num_sizer->Add(palette_num_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    // Spinbox to display palette number
    wxSpinCtrl* palette_num_spinbox = new wxSpinCtrl(
        tileset_options_sizer->GetStaticBox(),
        wxID_ANY,
        wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,
        wxSP_ARROW_KEYS,
        0,
        10,
        0
    );
    palette_num_spinbox->SetMinSize( wxSize(48,-1) );

    // Add to sizer
	palette_num_sizer->Add(palette_num_spinbox, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
}

/**
 * @brief Creates and populates the menu bar.
 */
void MainFrame::create_menu_bar()
{
    LOG_DEBUG("Creating menu bar...");
    
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(
        MenuItem::Open,
        "&Open...\tCtrl-O",
        "Open a tileset folder"
    );
    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT);

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &MainFrame::on_open, this, MenuItem::Open);
    Bind(wxEVT_MENU, &MainFrame::on_exit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::on_about, this, wxID_ABOUT);
}

/**
 * @brief Functionality for the "File > Hello" menu item.
 * @param event 
 */
void MainFrame::on_open(wxCommandEvent& event)
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
}

/**
 * @brief Functionality for the "File > Exit" menu item.
 * @param event 
 */
void MainFrame::on_exit(wxCommandEvent& event)
{
    Close(true);
}

/**
 * @brief Functionality for the "Help > About" menu item.
 * @param event 
 */
void MainFrame::on_about(wxCommandEvent& event)
{
    wxMessageBox(
        "This is a wxWidgets Hello World example",
        "About Hello World",
        wxOK | wxICON_INFORMATION
    );
}

} // namespace gui
} // namespace viewer
