#include "gui/app.h"

#include <logger/logger.h>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "gui/tileset_view.h"

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

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Pokemon Emerald Tileset Viewer")
{
    LOG_DEBUG("Creating main frame...");

    create_gui();

    // Bind events
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
    LOG_INFO("Open menu selected");

    m_tileset_view->load_tileset();
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

/**
 * @brief Creates and arranges GUI widgets.
 */
void MainFrame::create_gui()
{
    // GUI coding go brrrr

    // --- Top-Level --- //

    // Position the window in the center of the main display
	Center(wxBOTH);

    // The TilesetView's size is 270x571 (see the comment in TilesetView::create_gui() for why that is).
    // The window title adds 30px of height.
    // The menu bar adds 20px of height.
    // The TilesetView is given a 5px border on the left, which is superficially added to the bottom and right sides.
    // The entire window has a 1px border.
    // The defined window area is larger than the displayed window by 14px horizontally and 7px vertically.
    // 270 + (5 + 5) + (1 + 1) + 14 = 296
    // 571 + 30 + 20 + 5 + (1 + 1) + 7 = 635
    SetMinSize( wxSize(296, 635) );
    SetMaxSize( wxSize(296, 635) );

    create_menu_bar();

    // Top-level panel
    wxPanel* main_panel {
        new wxPanel(
            this,
            wxID_ANY
        )
    };

    // Top-level sizer
    wxFlexGridSizer* main_sizer { new wxFlexGridSizer(2, 2, 5, 5) };
    main_panel->SetSizer(main_sizer);

    // --- Tileset View --- //

    // Custom widget for the tileset view panel
    m_tileset_view = new TilesetView(main_panel);

    // Add to sizer
    main_sizer->Add(m_tileset_view, 1, wxLEFT, 5);
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
        "&Open...\tCtrl+O",
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
}

} // namespace gui
} // namespace viewer
