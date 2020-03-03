#include "ising.h"
#include "cpp_cli/cpp_cli.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <ctime>
using namespace ising;
typedef unsigned int uint;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Starts up SDL, creates window, and initializes OpenGL
bool init(const uint size, const char * title);

//Initializes matrices and clear color
bool initGL();

void close();

int main(int argc, char** argv) {
	bool help = false;
	uint size = 512;
	double temperature = 0.5;
	uint seed = time(NULL);
	cli::Parser p;
	char header[] = "A program to run the metropolis algorithm on an ising model.";
	char usage[] = "[options]";
	p.set_header(header);
	p.set_usage(usage);
	p.set_help_file_path("");
	p.arg(temperature, { "T", "temperature" }, "Set the temperature for the simulation.");
	p.arg(size, { "s", "size" }, "Set the size of the ising model square.");
	p.arg(seed, { "seed" }, "Set an initial seed for the random number generation.");
	p.value(help, { "h", "help" }, true, "Prints the help message and exits.");
	p.generate_help(argv[0]);
	p.parse(argc, argv);
	Ising_2D ising = Ising_2D(size, temperature, seed);
	if (help) {
		p.print_help();
		return 0;
	}

	const int buff_sz = 1024;
	char title[buff_sz];
	snprintf(title, buff_sz, "Temp: %f K | Size: %u | Seed: %u", temperature, size, seed);
	//Start up SDL and create window
	if(!init(size, title)) {
		printf("Failed to initialize!\n");
	} else {
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//Enable text input
		SDL_StartTextInput();
		#ifdef CIRCLE_TESTS
		uint32_t texture_80, texture_81;
		Coordinates center(size / 2, size / 2);
		Array_2D<uint32_t> circle_80 = gen_circle_bitmap(80, size, center);
		glGenTextures(1, &texture_80);
		glGenTextures(1, &texture_81);
		Array_2D<uint32_t> circle_81 = gen_circle_bitmap(81, size, center);
		#endif

		ising.draw();

		//While application is running
		while(!quit) {
			//Handle events on queue
			while(SDL_PollEvent(&e) != 0) {
				//User requests quit
				if(e.type == SDL_QUIT) {
					quit = true;
				} else if(e.type == SDL_TEXTINPUT) {
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					ising.handle_keys(e.text.text[0], x, y);
				}
			}
			ising.draw();
			#ifdef CIRCLE_TESTS
			draw_circle_test(circle_80, size, texture_80);
			draw_circle_test(circle_81, size, texture_81);
			#endif
			//Update screen
			SDL_GL_SwapWindow(gWindow);
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init(const uint size, const char * title) {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		//Create window
		gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, size, size, SDL_WINDOW_OPENGL |
		SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		} else {
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if(gContext == NULL) {
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				//Use Vsync
				if(SDL_GL_SetSwapInterval(1) < 0) {
					printf("Warning: Unable to set VSync! SDL Error: %s\n",
					SDL_GetError());
				}

				//Initialize OpenGL
				if(!initGL()) {
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL() {
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Check for error
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	return success;
}

void close() {
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
