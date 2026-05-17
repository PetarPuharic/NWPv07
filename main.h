#include <windows.h>
#include "nwpwin.h"

#include <gdiplus.h>

class gdiplus_application : public vsite::nwp::application
{
	Gdiplus::GdiplusStartupInput startup_input;
	ULONG_PTR           token;
public:
	gdiplus_application() {
		Gdiplus::GdiplusStartup(&token, &startup_input, nullptr);
	}
	~gdiplus_application() {
		Gdiplus::GdiplusShutdown(token);
	}
};

class main_window : public vsite::nwp::window {
public:
	main_window() = default;
	std::wstring slika;
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
private:
	std::wstring file_name;
	std::wstring current_filename;
	std::unique_ptr<Gdiplus::Image> loaded_image;
};
