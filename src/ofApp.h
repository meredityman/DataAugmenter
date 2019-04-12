#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void toggleProcessing();

        vector<ofFile> getFilesRecursively(const string path, const vector<string> exts);
	

		ofParameter<bool> flipVertical  { "flipVertical",   true };
		ofParameter<bool> flipHorizontal{ "flipHorizontal", true };
		ofParameter<bool> flipBoth      { "flipBoth",       true };
		ofParameter<bool> rotate90      { "rotate90",       true };
		ofParameter<bool> rotate180     { "rotate180",      true };
		ofParameter<bool> rotate270     { "rotate270",      true };
		ofParameterGroup options{ "Options",
			                      flipVertical,
								  flipHorizontal,
								  flipBoth,
								  rotate90,
								  rotate180,
								  rotate270 };


		vector<ofFile>::const_iterator itrImages;
		int imageNum = 0;

		float progress;

		bool processing = false;

        vector<ofFile> inputFiles;
		ofDirectory outputDir;

		ofImage currentImage;

		ofxImGui::Gui gui;
};
