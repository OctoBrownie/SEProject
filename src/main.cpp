#include <SDL.h>
#include<QApplication>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QWidget>
#include<QFrame>
#include<QMainWindow>
#include<QLayout>
#include<QLineEdit>
#include<QDockWidget>
#include<QScrollArea>
#include<QLabel>
#include <iostream>

#include "interface.h"

#define SDL_MAIN_HANDLED

int main(int argc, char **argv) {
    SDL_SetMainReady();
    SDL_setenv("SDL_AUDIODRIVER", "directsound", 1);
    	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "Couldn't initialize SDL..." << std::endl;
		return 1;
	}

    QApplication app (argc, argv);

    Interface* i = new Interface();
    i->show();

    int ret = app.exec();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
	SDL_Quit();

    return ret;
}



