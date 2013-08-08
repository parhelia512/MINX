/*
    MINX - A C++ Graphics and Input Wrapper Library
    Copyright (C) 2013  MINX Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/
#include "Game.h"
#include <iostream>
#include <thread>
#if defined(LINUX) || defined(DARWIN)
#include "X11/Xlib.h"
#endif
using namespace MINX;
using namespace MINX::Graphics;
using namespace std;

Game::Game()
{	
	desiredFPS = 60;
	windowWidth = 640;
	windowHeight = 480;
	windowBPP = 32;
	windowFlags = SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_SRCALPHA|SDL_HWACCEL;
	content = new MINX::Content(this);
	Components = new vector<GameComponent*>();
	keyboardEvents = new queue<SDL_Event*>();
	mouseEvents = new queue<SDL_Event*>();
	gamepadEvents = new queue<SDL_Event*>();
	gameTime = new GameTime();
	#if defined(LINUX) || defined(DARWIN)
	XInitThreads();
	#endif
}

int doDraw(Game * game){
	while(game->isRunning)
	{
		game->Draw(game->getGameTime());
	}
	return 0;
}

void Game::Run()
{	
	this->Initialize();
	this->LoadContent();
	std::thread drawingThread(&doDraw, this);
	while(isRunning)
	{
		if(SDL_PollEvent(&evt))
		{
			switch(evt.type)
			{
				case SDL_ACTIVEEVENT:
					break;
				case SDL_KEYDOWN:
					keyboardEvents->push(&evt);
					break;
				case SDL_KEYUP:
					keyboardEvents->push(&evt);
					break;
				case SDL_MOUSEMOTION:
					mouseEvents->push(&evt);
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseEvents->push(&evt);
					break;
				case SDL_MOUSEBUTTONUP:
					mouseEvents->push(&evt);
					break;
				case SDL_JOYAXISMOTION:
					gamepadEvents->push(&evt);
					break;
				case SDL_JOYBALLMOTION:
					gamepadEvents->push(&evt);
					break;
				case SDL_JOYHATMOTION:
					gamepadEvents->push(&evt);
					break;
				case SDL_JOYBUTTONDOWN:
					gamepadEvents->push(&evt);
					break;
				case SDL_JOYBUTTONUP:
					gamepadEvents->push(&evt);
					break;
				case SDL_VIDEORESIZE:
					break;
				case SDL_VIDEOEXPOSE:
					break;
				case SDL_QUIT:
					isRunning=false;
					break;
				case SDL_USEREVENT:
					break;
				case SDL_SYSWMEVENT:
					break;
			}
				
		}
		gameTime->update();
		this->Update(gameTime);
	}
	drawingThread.join();
	this->UnloadContent();
}

GameTime * Game::getGameTime()
{
	return gameTime;
}

void Game::Initialize()
{
	if( SDL_Init(SDL_INIT_EVERYTHING) == -1 )
	{
		std::cout << "SDL NOT INITED!\n";
	}

	if(TTF_Init() == -1 )
	{
		std::cout << "TTF NOT INITED!\n";
	}


					//uses video memory,prevents tearing,alpha works,hardware accelerated 
	 gameWindow = new GameWindow(windowWidth, windowHeight, windowBPP, windowFlags);

	for (vector<GameComponent*>::size_type i=0; i < Components->size(); i++)
	{
		(*Components)[i]->Initialize();
	}
}
void Game::LoadContent()
{

}
void Game::Update(GameTime * gameTime)
{
	for (vector<GameComponent*>::size_type i=0; i < Components->size(); i++)
	{
		if((*Components)[i]->enabled)
		{
			(*Components)[i]->Update(gameTime);
		}
	}
	gameTime->limitFPS(desiredFPS);
}
void Game::Draw(GameTime * gameTime)
{
	SDL_Flip(gameWindow->screen);
}

void Game::UnloadContent()
{
	while (!content->TTFFonts->empty())
	{
		TTF_CloseFont((content->TTFFonts->begin()->second));
		content->TTFFonts->erase(content->TTFFonts->begin());
	}
	while (!content->textures->empty())
	{
		SDL_FreeSurface((content->textures->begin()->second->tex));
		content->textures->erase(content->textures->begin());
	}
	SDL_Quit();
}
