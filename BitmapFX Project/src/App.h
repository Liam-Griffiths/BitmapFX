#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTouchGUI.h" // Nice ofx addon for buttons and the like. Fuck glut. 90's piece of junk.

class App : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//ofxTouchGui library setup.
		ofxTouchGUI touchGUI;
		void onGuiChanged(const void* sender, string &buttonLabel); // This is the GUI Event function.
		void processOpenFileSelection(ofFileDialogResult openFileResult);

		//Effect methods
		void grayscaleEffect(ofImage image);
		void blueEffect(ofImage image);
		void redEffect(ofImage image);
		void greenEffect(ofImage image);
		void binaryEffect(ofImage image);
		void toneEffectOne(ofImage image);
		void toneEffectTwo(ofImage image);
		void negativeEffect(ofImage image);
		void crazyEffect(ofImage image);
		void motionEffect(ofImage image);
		
};
