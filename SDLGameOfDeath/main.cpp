#include <SDL.h>
#include <stdio.h>
#include <iterator>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int gridRows = 36;
const int gridCols = 64;

int grid[gridRows * gridCols] = {};
int nextGrid[gridRows * gridCols] = {};

void checkGreenCells(int cellNo)
{
    int numberOfFriendlyCells = 0;

    bool downCheck = false;
    bool upCheck = false;

    //up
    if (cellNo >= gridCols)
    {
        if(grid[cellNo - gridCols] == 1)
            numberOfFriendlyCells++;
        upCheck = true;
    }

    //down
    if (cellNo < (size(grid) - gridCols))
    {
        if(grid[cellNo + gridCols] == 1)
            numberOfFriendlyCells++;
        downCheck = true;
    }
    
    //right
    if (cellNo % (gridCols - 1) != 0)
    {
        if(grid[cellNo + 1] == 1)
            numberOfFriendlyCells++;

        if (downCheck)
        {
            if (grid[cellNo + gridCols + 1] == 1)
                numberOfFriendlyCells++;
        }
        if (upCheck)
        {
            if (grid[cellNo - gridCols + 1] == 1)
                numberOfFriendlyCells++;
        }
    }

    //left
    if (cellNo % gridCols != 0)
    {
        if(grid[cellNo - 1] == 1)
            numberOfFriendlyCells++;

        if (downCheck)
        {
            if (grid[cellNo + gridCols - 1] == 1)
                numberOfFriendlyCells++;
        }
        if (upCheck)
        {
            if (grid[cellNo - gridCols - 1] == 1)
                numberOfFriendlyCells++;
        }
    }

    if (grid[cellNo] == 1)
    {
        if (numberOfFriendlyCells < 2)
        {
            nextGrid[cellNo] = 0;
        }
        else if (numberOfFriendlyCells > 3)
        {
            nextGrid[cellNo] = 0;
        }
        else
        {
            nextGrid[cellNo] = 1;
        }
    }
    else if (numberOfFriendlyCells == 3)
    {
        nextGrid[cellNo] = 1;
    }
}

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

SDL_Renderer* gRenderer = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        //Create window
        gWindow = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            //SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            //Get window surface
            //gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP("blackbuck.bmp");
    if (gHelloWorld == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

void close()
{
    //Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[])
{
    grid[66] = 1;
    grid[67] = 1;
    grid[68] = 1;

    //grid[1] = 2;
    
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        loadMedia();

        bool quit = false;

        //Event handler
        SDL_Event e;

        //While application is running
        while (!quit)
        {
            //Handle events on queue
            while (SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
            }

            //Apply the image
            //SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

            //Update the surface
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(gRenderer);

            for (int i = 0; i < size(grid); i++)
            {
                checkGreenCells(i);
            }

            for (int i = 0; i < size(nextGrid); i++)
            {
                grid[i] = nextGrid[i];

                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                SDL_Rect fillRect = { (i % gridCols) * SCREEN_WIDTH / gridCols, (i / gridCols) * SCREEN_HEIGHT / gridRows, SCREEN_WIDTH / gridCols, SCREEN_HEIGHT / gridRows};

                if (nextGrid[i] == 1)
                {
                    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                }
                    
                SDL_RenderFillRect(gRenderer, &fillRect);
            }

            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            for (int i = 0; i <= gridCols; i++)
            {
                SDL_RenderDrawLine(gRenderer, i * SCREEN_WIDTH / gridCols, 0, i * SCREEN_WIDTH / gridCols, SCREEN_HEIGHT);
            }

            for (int i = 0; i <= gridRows; i++)
            {
                SDL_RenderDrawLine(gRenderer, 0, i * SCREEN_HEIGHT / gridRows, SCREEN_WIDTH, i * SCREEN_HEIGHT / gridRows);
            }

            SDL_RenderPresent(gRenderer);

            //SDL_UpdateWindowSurface(gWindow);
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
