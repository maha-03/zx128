#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>
#include <memory>

#include "Memory.h"
#include "IO.h"
#include "Z80CPU.h"
#include "AudioDriver.h"

class Window
{
public:
	static constexpr int DEFAULT_WIDTH = 800;
	static constexpr int DEFAULT_HEIGHT = 600;
	static constexpr int PIXEL_SCALE = 2;
protected:
	using WindowPtr = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;
	using RendererPtr = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>;
	using IOPtr = std::unique_ptr<IO>;

	int _width, _height;
	WindowPtr _window;
	RendererPtr _renderer;
	uint8_t _tmp_vram[6912];
	uint8_t _tmp_border {1};
	bool _want_quit { false };

	AudioDriver _adrv;
	IO io;
	Z80CPU z80;

	void handle_event(const SDL_Event & event);
	void handle_keys(const Uint8 * keys);
	void do_logic();
	void render();
public:
	Window(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	Window(const Window &) = delete;
	Window(Window &&) = delete;

	void main();

protected:
	ROM rom { "zx.rom" };
	RAM ram { 16 };
	IO io { &_adrv };
	AddressSpace system_bus { ram, rom, io };
	Z80CPU z80 { system_bus };
};





#endif /* WINDOW_H_ */
