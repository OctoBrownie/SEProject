#include<SDL.h>
#include<QApplication>
#include<iostream>

#include "app.h"

// so that SDL doesn't make a main function too
#define SDL_MAIN_HANDLED

int main(int argc, char** argv) {
	SDL_SetMainReady();
	SDL_setenv("SDL_AUDIODRIVER", "directsound", 1);
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "Couldn't initialize SDL..." << std::endl;
		return 1;
	}

    QApplication app(argc, argv);

	App a;
	a.show();

	int res = app.exec();	// app event loop

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	SDL_Quit();
	return res;
}
