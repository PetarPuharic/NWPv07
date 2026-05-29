#include "main.h"
#include "rc.h"
#include <commdlg.h>
#include <filesystem>

using namespace Gdiplus;

void main_window::on_paint(HDC hdc)
{
	RECT client_rect;
	GetClientRect(*this, &client_rect);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush background_brush(Gdiplus::Color::White);
	graphics.FillRectangle(&background_brush, 0, 0, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);

	if (loaded_image && !file_name.empty())
	{
		Gdiplus::RectF display_area(
			static_cast<Gdiplus::REAL>(client_rect.left),
			static_cast<Gdiplus::REAL>(client_rect.top),
			static_cast<Gdiplus::REAL>(client_rect.right - client_rect.left),
			static_cast<Gdiplus::REAL>(client_rect.bottom - client_rect.top)
		);

		graphics.DrawImage(loaded_image.get(), display_area);

		int width = client_rect.right - client_rect.left;
		int height = client_rect.bottom - client_rect.top;
		int font_size = static_cast<int>(width * 0.035);

		Gdiplus::Font font(L"Calibri", static_cast<Gdiplus::REAL>(font_size), Gdiplus::FontStyleBold);
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentFar);

		Gdiplus::SolidBrush shadow_brush(Gdiplus::Color(255, 0, 0, 0)); // poluprozirna sjena
		Gdiplus::SolidBrush text_brush(Gdiplus::Color::White);

		Gdiplus::RectF shadow_area = display_area;
		shadow_area.X += 3;
		shadow_area.Y += 3;

		graphics.DrawString(file_name.c_str(), -1, &font, shadow_area, &format, &shadow_brush);
		graphics.DrawString(file_name.c_str(), -1, &font, display_area, &format, &text_brush);
	}
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
	case ID_OPEN:
	{
		TCHAR file_path[MAX_PATH];
		file_path[0] = _T('\0');

		TCHAR file_filter[] =	_T("Image Files\0*.jpeg;*.jpg;*.png;*.bmp;*.gif;*.tiff;*.tif;*.emf\0")
								_T("JPEG Files\0*.jpeg;*.jpg\0")
								_T("PNG Files\0*.png\0")
								_T("BMP Files\0*.bmp\0")
								_T("GIF Files\0*.gif\0")
								_T("TIFF Files\0*.tiff;*.tif\0")
								_T("EMF Files\0*.emf\0")
								_T("All Files\0*.*\0\0");

		OPENFILENAME open_dialog;
		ZeroMemory(&open_dialog, sizeof(open_dialog));

		open_dialog.lStructSize = sizeof(open_dialog);
		open_dialog.hwndOwner = *this;
		open_dialog.lpstrFile = file_path;
		open_dialog.nMaxFile = MAX_PATH;
		open_dialog.lpstrFilter = file_filter;
		open_dialog.nFilterIndex = 1;
		open_dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&open_dialog))
		{
			auto temp_image = std::make_unique<Gdiplus::Image>(file_path);

			if (temp_image->GetLastStatus() == Gdiplus::Ok) {
				loaded_image = std::move(temp_image);
				file_name = std::filesystem::path(file_path).filename();
				InvalidateRect(*this, nullptr, FALSE);
			}
			else {
				loaded_image.reset();
				file_name.clear();
				InvalidateRect(*this, nullptr, FALSE);
				MessageBox(*this, _T("Failed to load the selected image file."), _T("Error"), MB_OK | MB_ICONERROR);
			}
		}
	}
	break;

		case ID_EXIT:
			DestroyWindow(*this);
            break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
