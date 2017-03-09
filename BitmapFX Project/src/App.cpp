#include "App.h"

// ------------------------------
// ----- Author: Liam Griffiths, 1225116
// ------------------------------

int _W; // width
int _H; // height

// Position of the gui relative to window space.
int leftGUI_Width;
int viewportWidth;
int viewportX;
int viewportY;

ofImage originalImage; // the one that is loaded.
ofImage currentImage; // the image before a effect was applied.
ofImage outputImage; // the preview image, undegraded by zoom.
ofImage previewImage; // the displayed image. shows the live preview.

ofxPanel gui;

int currentEffect = 0;

int imageWidth = 0;
int imageHeight = 0;

int previewWidth = 0;
int previewHeight = 0;

int currentXpos;
int currentYpos;

string originalFileExtension;

//--------------------------------------------------------------
void App::setup(){

	// Set window width and height.
	_W = ofGetWidth();
	_H = ofGetHeight();
	leftGUI_Width = 100;
	viewportWidth = _W - leftGUI_Width;
	viewportX = (_W - viewportWidth)+30;
	viewportY = 10;

	// Settings for the touchGui system
	touchGUI.loadSettings("settings.xml", false, true); // savefile, default font, use mouse
	touchGUI.setBackgroundColor(ofColor(80,80,80),0,0,leftGUI_Width,_H);

	// Add GUI Components
	touchGUI.addText("MENU",25,30,80,15);
	touchGUI.addButton("OPEN",20,50,60,20);
	touchGUI.addButton("SAVE",20,80,60,20);
	touchGUI.addButton("RESET",20,110,60,20);
	touchGUI.addButton("EXIT",20,140,60,20);

	touchGUI.addButton("ZOOM IN",15,210,70,20);
	touchGUI.addButton("ZOOM OUT",15,240,70,20);

	touchGUI.addText("EFFECTS",10,315,80,15);
	touchGUI.addButton("GREYSCALE",10,330,80,20);
	touchGUI.addButton("RED",10,355,80,20);
	touchGUI.addButton("GREEN",10,385,80,20);
	touchGUI.addButton("BLUE",10,415,80,20);
	touchGUI.addButton("BINARY",10,445,80,20);
	touchGUI.addButton("TONE 1",10,475,80,20);
	touchGUI.addButton("TONE 2",10,505,80,20);
	touchGUI.addButton("NEGATIVE",10,535,80,20);
	touchGUI.addButton("CRAZY",10,565,80,20);
	touchGUI.addButton("MOTION",10,595,80,20);

	touchGUI.addButton("APPLY",15,660,70,20);
	touchGUI.addButton("CANCEL",15,690,70,20);

	// adds a listener to all gui items, pointing to onGuiChanged();
    touchGUI.addEventListenerAllItems(this);
}

//--------------------------------------------------------------
void App::update(){
	_W = ofGetWidth();
	_H = ofGetHeight();
	viewportWidth = _W - leftGUI_Width;
	viewportX = (_W - viewportWidth)+10;
}

//--------------------------------------------------------------
void App::draw(){
	// Give the app a nice gradient background. Lovely.
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	ofFill();

	previewImage.draw(viewportX,viewportY);

	touchGUI.draw(); // Draw gui last!
}

void App::onGuiChanged(const void* sender, string &buttonLabel) {
    
    // Check for label identifier of pressed GUI component.
    if(buttonLabel == "OPEN")
	{
		printf_s("Open pressed");
		//Open the Open File Dialog
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select an image."); 
		
		//Check if the user opened a file
		if (openFileResult.bSuccess){
			//We have a file, check it and process it
			processOpenFileSelection(openFileResult);	
		}
    }

	if(buttonLabel == "SAVE")
	{

		ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower(originalFileExtension), "Save your file");
		if (saveFileResult.bSuccess){
			currentImage.saveImage(saveFileResult.filePath);
		}

    }

	if(buttonLabel == "RESET")
	{
		previewHeight = imageHeight;
		previewWidth = imageWidth;
		previewImage.resize(imageWidth, imageHeight);
		previewImage.setAnchorPoint(viewportX - 120, viewportY - 20);
		currentImage = originalImage;
		previewImage = originalImage;
    }

	if(buttonLabel == "APPLY")
	{
		previewHeight = imageHeight;
		previewWidth = imageWidth;
		previewImage.resize(imageWidth, imageHeight);
		previewImage.setAnchorPoint(viewportX - 120, viewportY - 20);
		currentImage = outputImage;
		previewImage = outputImage;
    }
	if(buttonLabel == "CANCEL")
	{
		previewHeight = imageHeight;
		previewWidth = imageWidth;
		previewImage.resize(imageWidth, imageHeight);
		previewImage.setAnchorPoint(viewportX - 120, viewportY - 20);
		outputImage = currentImage;
		previewImage = currentImage;
    } 

	if(buttonLabel == "EXIT")
	{
		exit();
		exitApp(); // kill program
    }

	if(buttonLabel == "ZOOM IN")
	{
		previewImage.resize(int(previewWidth * 1.2), int(previewHeight * 1.2));
		previewHeight = previewImage.getHeight();
		previewWidth = previewImage.getWidth();
    } 

	if(buttonLabel == "ZOOM OUT")
	{
		previewImage.resize(int(previewWidth * 0.8), int(previewHeight * 0.8));
		previewHeight = previewImage.getHeight();
		previewWidth = previewImage.getWidth();
    } 

	if(buttonLabel == "GREYSCALE")
	{
		grayscaleEffect(currentImage);
    }
	if(buttonLabel == "BLUE")
	{
		blueEffect(currentImage);
    }
	if(buttonLabel == "RED")
	{
		redEffect(currentImage);
    } 
	if(buttonLabel == "GREEN")
	{
		greenEffect(currentImage);
    }
	if(buttonLabel == "BINARY")
	{
		binaryEffect(currentImage);
    }
	if(buttonLabel == "TONE 1")
	{
		toneEffectOne(currentImage);
    }
	if(buttonLabel == "TONE 2")
	{
		toneEffectTwo(currentImage);
    }
	if(buttonLabel == "NEGATIVE")
	{
		negativeEffect(currentImage);
    }
	if(buttonLabel == "CRAZY")
	{
		crazyEffect(currentImage);
    }
	if(buttonLabel == "MOTION")
	{
		motionEffect(currentImage);
    }

}

void App::processOpenFileSelection(ofFileDialogResult openFileResult)
{

	
	ofFile file (openFileResult.getPath()); 
	
	if (file.exists()){
		
		// Clear any previously loaded images.
		originalImage.clear();
		currentImage.clear();
		previewImage.clear();

		string fileExtension = ofToUpper(file.getExtension());
		
		//We only want images
		if (fileExtension == "JPG" || fileExtension == "PNG" || fileExtension == "BMP") 
		{
			
			//Save the file extension to use when we save out
			originalFileExtension = fileExtension;
			
			// Load the selected image
			// Each image store needs to keep it's own version.
			originalImage.loadImage(openFileResult.getPath());
			currentImage.loadImage(openFileResult.getPath());
			previewImage.loadImage(openFileResult.getPath());

			imageWidth = originalImage.getWidth();
			previewWidth = originalImage.getWidth();
			imageHeight = originalImage.getHeight();
			previewHeight = originalImage.getHeight();

			currentXpos = viewportX - 120;
			currentYpos = viewportY - 20;
			previewImage.setAnchorPoint(currentXpos, currentYpos);

		}
		else
		{
			ofSystemAlertDialog("Unsupported Format.");
		}
	}
}

//--------------------------------------------------------------
void App::keyPressed(int key){

	if (key == OF_KEY_UP)
	{
		currentYpos -= 5; 
		previewImage.setAnchorPoint(currentXpos, currentYpos);
    }
	else if (key == OF_KEY_DOWN)
	{
        currentYpos += 5; 
		previewImage.setAnchorPoint(currentXpos, currentYpos);
    }

	if (key == OF_KEY_LEFT)
	{
        currentXpos -= 5; 
		previewImage.setAnchorPoint(currentXpos, currentYpos);
    }
	else if (key == OF_KEY_RIGHT)
	{
        currentXpos += 5; 
		previewImage.setAnchorPoint(currentXpos, currentYpos);
    }


}

//--------------------------- Effects --------------------------
void App::grayscaleEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			char greyscaleVal = (color.r + color.g + color.b) / 3;
			ofColor newColor(greyscaleVal, greyscaleVal, greyscaleVal);
			image.setColor(x, y, newColor);

		}
	}

	outputImage = image;
	previewImage = image;
}

void App::blueEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			ofColor newColor(0, 0, color.b);
			image.setColor(x, y, newColor);
		}
	}

	outputImage = image;
	previewImage = image;
}

void App::redEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			ofColor newColor(color.r, 0, 0);
			image.setColor(x, y, newColor);
		}
	}

	outputImage = image;
	previewImage = image;
}

void App::greenEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			ofColor newColor(0, color.g, 0);
			image.setColor(x, y, newColor);
		}
	}

	outputImage = image;
	previewImage = image;
}

void App::binaryEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			char greyscaleVal = (color.r + color.g + color.b) / 3;

			if(greyscaleVal > 90)
			{
				ofColor newColor(255,255,255);
				image.setColor(x, y, newColor);
			}else{
				ofColor newColor(0,0,0);
				image.setColor(x, y, newColor);
			}
		}
	}

	outputImage = image;
	previewImage = image;
}

void App::toneEffectOne(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			char greyscaleVal = (color.r + color.g + color.b) / 3;

			ofColor newColor((greyscaleVal * 240)/255,(greyscaleVal * 220)/255,(greyscaleVal * 130)/255);

			image.setColor(x, y, newColor);
		}
	}
	

	outputImage = image;
	previewImage = image;
}

void App::toneEffectTwo(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);
			char greyscaleVal = (color.r + color.g + color.b) / 3;

			ofColor newColor((greyscaleVal * 112)/255,(greyscaleVal * 66)/255,(greyscaleVal * 60)/255);

			image.setColor(x, y, newColor);
		}
	}
	

	outputImage = image;
	previewImage = image;
}

void App::negativeEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++){
			printf(".");
			ofColor color = image.getColor(x, y);

			ofColor newColor((255 - color.r),(255 - color.g),(255 - color.b));

			image.setColor(x, y, newColor);
		}
	}
	

	outputImage = image;
	previewImage = image;
}

void App::crazyEffect(ofImage image)
{

	ofImage newImage;

	newImage = image;

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 0; y < (h-5); y++)
	{
		for (int x = 0; x < (w-5); x++){
			printf(".");

			ofColor pixelArray[5][5];
			
			ofColor blankColor(128,128,128);

			for (int y2 = 0; y2 < 5; y2++)
			{
				for (int x2 = 0; x2 < 5; x2++)
				{
					pixelArray[y2][x2] = image.getColor(x + x2, y +2);
				}
			}

			int redTotal; int greenTotal; int blueTotal;
			for (int y3 = 0; y3 < 5; y3++)
			{
				for (int x3 = 0; x3 < 5; x3++)
				{
					redTotal += pixelArray[y3][x3].r;
					greenTotal += pixelArray[y3][x3].g;
					blueTotal += pixelArray[y3][x3].b;
				}
			}

			ofColor newColor((redTotal / 24), (greenTotal / 24), (blueTotal / 24));

			image.setColor(x, y, newColor);
		}
	}
	

	outputImage = image;
	previewImage = image;
}

void App::motionEffect(ofImage image)
{

	int w = image.getWidth();
	int h = image.getHeight();

	//Walk through the pixels
	for (int y = 2; y < (h-2); y++)
	{
		for (int x = 2; x < (w-2); x++){

			ofColor color3 = image.getColor(x-1, y-1);
			ofColor color1 = image.getColor(x-1, y-1);
			ofColor color = image.getColor(x, y);
			ofColor color2 = image.getColor(x+2, y+2);
			ofColor color4 = image.getColor(x-2, y-2);

			ofColor newColor((color1.r + color.r + color2.r + color3.r + color4.r) / 5,(color1.g + color.g + color2.g + color3.g + color4.g) / 5,(color1.b + color.b + color2.b + color3.b + color4.b) / 5);

			image.setColor(x, y, newColor);
		}
	}
	

	outputImage = image;
	previewImage = image;
}


//--------------------------------------------------------------
void App::keyReleased(int key){

}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void App::windowResized(int w, int h){

}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo){ 

}
