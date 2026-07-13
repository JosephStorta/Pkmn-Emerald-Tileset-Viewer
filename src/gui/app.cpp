#include "gui/app.h"

#include <logger/logger.h>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

namespace viewer {
namespace gui {

SET_LOG_MODULE("GUI");

/**
 * @brief Helper enum to store menu item IDs.
 */
enum MenuItem
{
    Hello = 1
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

    SetSizeHints(wxDefaultSize, wxDefaultSize);
    create_menu_bar();

	wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel* tileset_view_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* tileset_view_sizer = new wxBoxSizer(wxVERTICAL);
	wxScrolledWindow* tilset_scroll = new wxScrolledWindow(tileset_view_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxHSCROLL|wxVSCROLL);
	tilset_scroll->SetScrollRate(5, 5);
	wxBoxSizer* image_sizer = new wxBoxSizer(wxVERTICAL);

	//wxStaticBitmap* tileset_image = new wxStaticBitmap(tilset_scroll, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0);
	//image_sizer->Add(tileset_image, 1, wxALL|wxEXPAND, 5);

	tilset_scroll->SetSizer(image_sizer);
	tilset_scroll->Layout();
	image_sizer->Fit(tilset_scroll);
	tileset_view_sizer->Add(tilset_scroll, 1, wxALL|wxEXPAND, 5);

	wxStaticBoxSizer* tileset_options_sizer = new wxStaticBoxSizer(new wxStaticBox(tileset_view_panel, wxID_ANY, _("Options")), wxHORIZONTAL);

	wxCheckBox* apply_palette_check = new wxCheckBox(tileset_options_sizer->GetStaticBox(), wxID_ANY, _("Apply Palette"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	tileset_options_sizer->Add(apply_palette_check, 2, wxALL|wxEXPAND, 5);
	tileset_options_sizer->Add(0, 0, 1, wxEXPAND, 5);

	wxBoxSizer* palette_num_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* palette_num_label = new wxStaticText(tileset_options_sizer->GetStaticBox(), wxID_ANY, _("Palette"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	palette_num_label->Wrap(-1);
	palette_num_sizer->Add(palette_num_label, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxSpinCtrl* palette_num_spinbox = new wxSpinCtrl(tileset_options_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0);
	palette_num_spinbox->SetMinSize( wxSize(48,-1) );

	palette_num_sizer->Add(palette_num_spinbox, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	tileset_options_sizer->Add(palette_num_sizer, 2, wxEXPAND, 5);
	tileset_view_sizer->Add(tileset_options_sizer, 0, wxEXPAND, 5);

	tileset_view_panel->SetSizer(tileset_view_sizer);
	tileset_view_panel->Layout();
	tileset_view_sizer->Fit(tileset_view_panel);
	main_sizer->Add(tileset_view_panel, 1, wxEXPAND|wxALL, 5);

	SetSizer(main_sizer);
	Layout();
	Centre(wxBOTH);
}

/**
 * @brief Creates and populates the menu bar.
 */
void MainFrame::create_menu_bar()
{
    LOG_DEBUG("Creating menu bar...");
    
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(
        MenuItem::Hello,
        "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item"
    );
    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT);

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &MainFrame::on_hello, this, MenuItem::Hello);
    Bind(wxEVT_MENU, &MainFrame::on_exit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::on_about, this, wxID_ABOUT);
}

/**
 * @brief Functionality for the "File > Hello" menu item.
 * @param event 
 */
void MainFrame::on_hello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
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
