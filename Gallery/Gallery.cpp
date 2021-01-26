#include "D3DApp.h"

#include <ctime>

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ const HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ const int nShowCmd) {

	D3DApp app;

	if (FAILED(app.initWindow(hInstance, nShowCmd)))
		return 0;

	if (FAILED(app.initDevice())) 
		return 0;

	if (FAILED(app.initScene()))
		return 0;

	const std::clock_t start = std::clock();
	std::clock_t last = start;

	MSG msg;
	msg.message = 0;
	while (WM_QUIT != msg.message) 
	{
		const clock_t now = clock();
		const double elapsed_secs = static_cast<double>(now - last) / static_cast<clock_t>(1000);
		last = now;

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			app.update(elapsed_secs);
			app.render();
		}
	}

	return static_cast<int>(msg.wParam);
}