#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <string>
using namespace std;

// ---GLOBAL VARS---

// Screen Dimensions
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 720;

// Bars representing array values
const int bars = 100;
const int BarSize = 12;

int arr[bars];
int height[bars];

// Rendering Window
SDL_Window* window = NULL;

// Window Surface
SDL_Surface* ScreenSurface = NULL;

// Window Renderer
SDL_Renderer* gRenderer = NULL;


// To check sorted state
bool sorted = false;


// Initialise function
bool Initialise()
{
    bool success = true;

    // Initialising SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // Creating the window
        window = SDL_CreateWindow("Algorithm Visualiser", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        // Initalising renderer
        else {
            gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }

            // Get window surface
            ScreenSurface = SDL_GetWindowSurface(window);
            // Filling surface black
            SDL_FillRect(ScreenSurface, NULL, SDL_MapRGB(ScreenSurface->format, 0, 0, 0));
            // Update window surface
            SDL_UpdateWindowSurface(window);

        }
    }

    return success;
}

// Function to close program and destroy window and renderer once X is pressed at the top right of window
void Close()
{
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void LoadArray()
{
    // Copies height data to arr
    memcpy(arr, height, sizeof(int) * bars);
}

// Randomise array function
void RandomiseArray()
{
    srand(time(0));

    // Goes through 0-100 bars, assigning them a random height value within the screen height
    for (int i = 0; i < bars; i++) {
        int RandValue = (rand() % SCREEN_HEIGHT);
        height[i] = RandValue;
    }
}

// Drawing bars to screen
void Draw(int x = -1, int y = -1, int z = -1)
{
    // Setting background to black and clearing the renderer
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    // Counter
    int j = 0;

    for (int i = 0; i <= SCREEN_WIDTH - BarSize; i += BarSize) {
        SDL_Rect rect = { i, 0, BarSize, arr[j] };

        // Sets to green if in sorted pos
        if (sorted) {
            SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
            SDL_RenderDrawRect(gRenderer, &rect);
        }
        // Sets color of right bar in comparison 
        else if (j == x || j == z) {
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0);
            SDL_RenderFillRect(gRenderer, &rect);
        }
        // Sets color of left bar in comparison
        else if (j == y) {
            SDL_SetRenderDrawColor(gRenderer, 212, 19, 19, 0);
            SDL_RenderFillRect(gRenderer, &rect);
        }
        // Color of bars before sorting begins
        else {
            SDL_SetRenderDrawColor(gRenderer, 113, 117, 116, 0);
            SDL_RenderFillRect(gRenderer, &rect);
        }
        j++;
    }
    // Displays whats on the renderer
    SDL_RenderPresent(gRenderer);
}



// Bubble Sort function
void BubbleSort()
{
    for (int i = 0; i < bars - 1; i++) {
        for (int j = 0; j < bars - 1 - i; j++) {
            if (arr[j + 1] < arr[j]) {
                // Swapping values
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                Draw(j + 1, j, bars - i);
            }
            SDL_Delay(1);
        }
    }
}

//
int Partition(int arr[], int start, int end)
{
    // Declaring pivot at start of array
    int pivot = arr[start];
    int count = 0;

    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot) {
            count++;
        }
    }
    // Assigning pivotindex to pivot pos
    int PivotIndex = start + count;
    swap(arr[PivotIndex], arr[start]);

    int i = start, j = end;

    // Sorting left and right from pivotindex
    while (i < PivotIndex && j > PivotIndex) {
        if (arr[i] <= arr[PivotIndex]) {
            i++;
        }
        else if (arr[j] > arr[PivotIndex]) {
            j--;
        }
        else {
            // Swapping i and j, swap function didnt work
            int temp2 = arr[j];
            arr[j] = arr[i];
            arr[i] = temp2;
            Draw(i, j);
            SDL_Delay(50);
            i++;
            j--;
        }
    }
    return PivotIndex;
}

// Quick Sort function
void QuickSort(int arr[], int start, int end)
{
    if (start < end) {
        int PivotIndex = Partition(arr, start, end);
        QuickSort(arr, start, PivotIndex - 1);
        QuickSort(arr, PivotIndex + 1, end);
    }
}


void Start()
{
    cout << "============================================================= Sorting Visualiser ===========================================================\n\n"
        << "I created this program teach myself and remember how the various sorting algorithms work, both visually and in code format.\n"
        << "This program randomly generates 100 integers and their value is represented by the height of their bar.\n"
        << "You can then choose which sorting method you would like to use.\n\n"
        << "Press the Enter key to advance to controls:";

    string s;
    getline(cin, s);

    if (s == "\n") {
        return;
    }
}

bool Controls()
{
    cout << "================================================================== Controls ================================================================\n\n"
        << "Press 0 to generate a new array.\n"
        << "Press 1 to start Bubble Sort.\n"
        << "Press 2 to start Quick Sort.\n\n"
        << "Press Enter to start:\n";

    string s;
    getline(cin, s);

    if (s == "\n") {
        return true;
    }
    return true;
}

// Main Function
void EventCommands()
{
    if (!Initialise()) {
        printf("Failed to initialise!\n");
    }
    else {

        bool quit = false;

        // Event handler (stores inputs/events from user and puts them in a queue)
        SDL_Event e;

        RandomiseArray();
        LoadArray();
        cout << "You can now utilise the control keys (0,1 and 2):\n\n";

        // Loop checking user inputs while quit is == false
        while (!quit) {
            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                // If user presses X at corner of screen, quit == true is returned and the program is exitted
                if (e.type == SDL_QUIT) {
                    quit = true;
                    sorted = false;
                }
                else if (e.type == SDL_KEYDOWN) {
                    // Switch to carry out different commands on user input
                    switch (e.key.keysym.sym) {
                    case(SDLK_0):
                        RandomiseArray();
                        sorted = false;
                        LoadArray();
                        cout << "New array generated!\n";
                        break;
                    case(SDLK_1):
                        LoadArray();
                        sorted = false;
                        cout << "Begginning Bubble Sort...\n";
                        BubbleSort();
                        sorted = true;
                        cout << "Bubble Sort complete!\n";
                        break;
                    case(SDLK_2):
                        LoadArray();
                        sorted = false;
                        cout << "Begginning Quick Sort...\n";
                        QuickSort(arr, 0, bars - 1);
                        sorted = true;
                        cout << "Quick Sort complete!\n";
                        break;
                    }
                }
            }
            Draw();
        }
        Close();
    }
}

int main(int argc, char* args[])
{
    Start();

    while (true) {
        if (Controls())
            EventCommands();
        else {
            cout << "QUTTING...\n";
            break;
        }
    }
    return 0;
}
