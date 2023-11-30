//
//  main.c
// CSC412 - Fall 2023 - Prog 06
//
//  Created by Jean-Yves Hervé on 2017-05-01, modified 2023-11-12
//

#include <iostream>
#include <string>
#include <random>
#include <thread>
//
#include <cstdio>
#include <cstdlib>
#include <time.h>
//
#include "gl_frontEnd.h"
#include "ImageIO_TGA.h"

using namespace std;

//==================================================================================
//	Function prototypes
//==================================================================================
void myKeyboard(unsigned char c, int x, int y);
void initializeApplication(void);


//==================================================================================
//	Application-level global variables
//==================================================================================

//	Don't touch. These are defined in the front-end source code
extern int	gMainWindow;


//	Don't rename any of these variables/constants
//--------------------------------------------------
unsigned int numLiveFocusingThreads = 4;		//	the number of live focusing threads

//	An array of C-string where you can store things you want displayed in the spate pane
//	that you want the state pane to display (for debugging purposes?)
//	Dont change the dimensions as this may break the front end
//	I preallocate the max number of messages at the max message
//	length.  This goes against some of my own principles about
//	good programming practice, but I do that so that you can
//	change the number of messages and their content "on the fly,"
//	at any point during the execution of your program, whithout
//	having to worry about allocation and resizing.
const int MAX_NUM_MESSAGES = 8;
const int MAX_LENGTH_MESSAGE = 32;
char** message;
int numMessages;
time_t launchTime;

//	This is the image that you should be writing into.  In this
//	handout, I simply read one of the input images into it.
//	You should not rename this variable unless you plan to mess
//	with the display code.
RasterImage* imageOut;

//	Random Generation stuff
random_device myRandDev;
//	If you get fancy and specialized, you may decide to go for a different engine,
//	for exemple
//		mt19937_64  Mersenne Twister 19937 generator (64 bit)
default_random_engine myEngine(myRandDev());
//	a distribution for generating random r/g/b values
uniform_int_distribution<unsigned char> colorChannelDist;
//	Two random distributions for row and column indices.  I will
//	only be able to initialize them after I have read the image
uniform_int_distribution<unsigned int> rowDist;
uniform_int_distribution<unsigned int> colDist;


//------------------------------------------------------------------
//	The variables defined here are for you to modify and add to
//------------------------------------------------------------------
#define IN_PATH		"./DataSets/Series02/"
#define OUT_PATH	"./Output/"

const string hardCodedOutPath = "./outputImage.tga";

//==================================================================================
//	These are the functions that tie the computation with the rendering.
//	Some parts are "don't touch."  Other parts need your intervention
//	to make sure that access to critical section is properly synchronized
//==================================================================================

//	I can't see any reason why you may need/want to change this
//	function
void displayImage(GLfloat scaleX, GLfloat scaleY)
{
	//==============================================
	//	This is OpenGL/glut magic.  Don't touch
	//==============================================
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPixelZoom(scaleX, scaleY);

	//--------------------------------------------------------
	//	stuff to replace or remove.
	//	Here, each time we render the image I assign a random
	//	color to a few random pixels
	//--------------------------------------------------------
	unsigned char** imgRaster2D = (unsigned char**)(imageOut->raster2D);
	
	for (int k=0; k<100; k++) {
		unsigned int i = rowDist(myEngine);
		unsigned int j = colDist(myEngine);

		//	get pointer to the pixel at row i, column j
		unsigned char* rgba = imgRaster2D[i] + 4*j;
		// random r, g, b
		rgba[0] = colorChannelDist(myEngine);
		rgba[1] = colorChannelDist(myEngine);
		rgba[2] = colorChannelDist(myEngine);
		//	keep alpha unchanged at 255
	}

	//==============================================
	//	This is OpenGL/glut magic.  Don't touch
	//==============================================
	glDrawPixels(imageOut->width, imageOut->height,
				  GL_RGBA,
				  GL_UNSIGNED_BYTE,
				  imageOut->raster);

}


void displayState(void)
{
	//==============================================
	//	This is OpenGL/glut magic.  Don't touch
	//==============================================
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//--------------------------------------------------------
	//	stuff to replace or remove.
	//--------------------------------------------------------
	//	Here I hard-code a few messages that I want to see displayed in my state
	//	pane.  The number of live focusing threads will always get displayed
	//	(as long as you update the value stored in the.  No need to pass a message about it.
	time_t currentTime = time(NULL);
	numMessages = 3;
	sprintf(message[0], "System time: %ld", currentTime);
	sprintf(message[1], "Time since launch: %ld", currentTime-launchTime);
	sprintf(message[2], "I like cheese");
	
	
	//---------------------------------------------------------
	//	This is the call that makes OpenGL render information
	//	about the state of the simulation.
	//	You may have to synchronize this call if you run into
	//	problems, but really the OpenGL display is a hack for
	//	you to get a peek into what's happening.
	//---------------------------------------------------------
	drawState(numMessages, message);
}

void cleanupAndQuit(void)
{
	writeTGA(hardCodedOutPath.c_str(), imageOut);

	//	Free allocated resource before leaving (not absolutely needed, but
	//	just nicer.  Also, if you crash there, you know something is wrong
	//	in your code.
	for (int k=0; k<MAX_NUM_MESSAGES; k++)
		free(message[k]);
	free(message);

	// delete images [optional]
	
	exit(0);
}

//	This callback function is called when a keyboard event occurs
//	You can change things here if you want to have keyboard input
//
void handleKeyboardEvent(unsigned char c, int x, int y)
{
	int ok = 0;
	
	switch (c)
	{
		//	'esc' to quit
		case 27:
			//	If you want to do some cleanup, here would be the time to do it.
			cleanupAndQuit();
			break;

		//	Feel free to add more keyboard input, but then please document that
		//	in the report.
		
		
		default:
			ok = 1;
			break;
	}
	if (!ok)
	{
		//	do something?
	}
}

//------------------------------------------------------------------------
//	You shouldn't have to change anything in the main function.
//------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//	Now we can do application-level initialization
	initializeApplication();

	//	Even though we extracted the relevant information from the argument
	//	list, I still need to pass argc and argv to the front-end init
	//	function because that function passes them to glutInit, the required call
	//	to the initialization of the glut library.
	initializeFrontEnd(argc, argv, imageOut);
	

	//==============================================
	//	This is OpenGL/glut magic.  Don't touch
	//==============================================
	//	Now we enter the main loop of the program and to a large extend
	//	"lose control" over its execution.  The callback functions that
	//	we set up earlier will be called when the corresponding event
	//	occurs
	glutMainLoop();
		
	//	This will probably never be executed (the exit point will be in one of the
	//	call back functions).
	return 0;
}


//==================================================================================
//	This is a part that you have to edit and add to, for example to
//	load a complete stack of images and initialize the output image
//	(right now it is initialized simply by reading an image into it.
//==================================================================================

RasterImage* loadSingleImage(const std::string& filePath) {
   	return readTGA(filePath.c_str());
}

std::vector<RasterImage*> loadImageStack(const std::vector<std::string>& filePaths) {
	std::vector<RasterImage*> imageStack;
	for (const std::string& path : filePaths) {
		imageStack.push_back(loadSingleImage(path));
	}
	return imageStack;
}

RasterImage* createOutputImage(const RasterImage* referenceImage) {
   	return new RasterImage(referenceImage->width, referenceImage->height, referenceImage->type);
}

float convertToGrayscale(unsigned char r, unsigned char g, unsigned char b) {
    return 0.21f * r + 0.72f * g + 0.07f * b;
}


void processImageRegion(unsigned int startRow, unsigned int endRow, unsigned int startCol, unsigned int endCol, 
                        const std::vector<RasterImage*>& imageStack, RasterImage* outputImage) {
    const int windowSize = 3;  // Example window size
    const int halfWindow = windowSize / 2;

    for (unsigned int row = startRow; row < endRow; ++row) {
        for (unsigned int col = startCol; col < endCol; ++col) {
            int bestFocusIndex = 0;
            float bestFocusMeasure = -1.0f;

            for (size_t imgIndex = 0; imgIndex < imageStack.size(); ++imgIndex) {
                float minPixelValue = 255.0f;
                float maxPixelValue = 0.0f;

                for (int wy = -halfWindow; wy <= halfWindow; ++wy) {
                    for (int wx = -halfWindow; wx <= halfWindow; ++wx) {
                        int currentRow = static_cast<int>(row) + wy;
                        int currentCol = static_cast<int>(col) + wx;

                        if (currentRow >= 0 && static_cast<unsigned int>(currentRow) < outputImage->height &&
                            currentCol >= 0 && static_cast<unsigned int>(currentCol) < outputImage->width) {
                            unsigned char* pixel = static_cast<unsigned char*>(imageStack[imgIndex]->raster) +
                                                   currentRow * outputImage->bytesPerRow +
                                                   currentCol * outputImage->bytesPerPixel;

                            float grayValue = convertToGrayscale(pixel[0], pixel[1], pixel[2]); // Assuming RGB format
                            minPixelValue = std::min(minPixelValue, grayValue);
                            maxPixelValue = std::max(maxPixelValue, grayValue);
                        }
                    }
                }

                float focusMeasure = maxPixelValue - minPixelValue;
                if (focusMeasure > bestFocusMeasure) {
                    bestFocusMeasure = focusMeasure;
                    bestFocusIndex = imgIndex;
                }
            }

            // Copy pixel data from the most in-focus image to the output image
            unsigned char* sourcePixel = static_cast<unsigned char*>(imageStack[bestFocusIndex]->raster) +
                                         row * outputImage->bytesPerRow +
                                         col * outputImage->bytesPerPixel;
            unsigned char* destPixel = static_cast<unsigned char*>(outputImage->raster) +
                                       row * outputImage->bytesPerRow +
                                       col * outputImage->bytesPerPixel;

            for (unsigned int i = 0; i < outputImage->bytesPerPixel; ++i) {
                destPixel[i] = sourcePixel[i];
            }
        }
    }
}

void initializeApplication(void) {

    // Allocate memory for messages
    message = (char**) malloc(MAX_NUM_MESSAGES * sizeof(char*));
    for (int k = 0; k < MAX_NUM_MESSAGES; k++) {
        message[k] = (char*) malloc((MAX_LENGTH_MESSAGE + 1) * sizeof(char));
    }

    // Define the file paths for the images
    const std::string File_Path = "../Handout-Data/Series01/";
    std::vector<std::string> filePaths = {
        File_Path+"_MG_6291.tga", File_Path+"_MG_6293.tga", File_Path+"_MG_6294.tga",
        File_Path+"_MG_6295.tga", File_Path+"_MG_6296.tga", File_Path+"_MG_6297.tga",
        File_Path+"_MG_6298.tga", File_Path+"_MG_6299.tga", File_Path+"_MG_6300.tga",
        File_Path+"_MG_6301.tga", File_Path+"_MG_6302.tga"
    };

    // Load the image stack
    std::vector<RasterImage*> imageStack = loadImageStack(filePaths);

    // Prepare the output image based on the first image in the stack
    RasterImage* outputImage = createOutputImage(imageStack[0]);

    // Initialize threads and process image regions
    const int numThreads = numLiveFocusingThreads;
    int rowsPerThread = outputImage->height / numThreads;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        unsigned int startRow = i * rowsPerThread;
        unsigned int endRow = (i == numThreads - 1) ? outputImage->height : (i + 1) * rowsPerThread;
        threads.emplace_back(processImageRegion, startRow, endRow, 0, outputImage->width, imageStack, outputImage);
    }

    // Wait for threads to complete
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    launchTime = time(NULL);

    // Clean up: Free the image stack (if not needed anymore)
    for (RasterImage* img : imageStack) {
        delete img;
    }

    // Note: Be sure to free 'outputImage' when it is no longer needed
}
