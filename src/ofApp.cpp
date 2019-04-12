#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
	ImGui::GetIO().MouseDrawCursor = false;

	outputDir.open(ofToDataPath(""));
}

//--------------------------------------------------------------
void ofApp::update(){
	if (processing) {
		vector<ofImage> images;
        

		ofImage loadedImage(itrImages->path());
        loadedImage.setImageType(ofImageType::OF_IMAGE_COLOR);
        
        int w = loadedImage.getWidth();
        int h = loadedImage.getHeight();

        if (w > h) {
            int newWidth = 1024 * (float)w / h ;
            ofLogNotice() << newWidth;
            loadedImage.resize( newWidth , 1024.0 );
            loadedImage.crop((newWidth - 1024) * 0.5, 0, 1024, 1024);
        }
        else {
            int newHeight = 1024 * (float)h / w ;
            ofLogNotice() << newHeight;
            loadedImage.resize( 1024.0, newHeight);
            loadedImage.crop(0,  (newHeight - 1024) * 0.5, 1024, 1024);
        }

        const ofImage & image = loadedImage;
        
		ofImage newImage;

        
        images.emplace_back(image);

		if (flipVertical) {
			newImage.setFromPixels(image.getPixels());
			newImage.mirror(true, false);
			images.emplace_back(newImage);
		}
		if(flipHorizontal) {
			newImage.setFromPixels(image.getPixels());
			newImage.mirror(false, true);
			images.push_back(newImage);
		}
		if(flipBoth) {
			newImage.setFromPixels(image.getPixels());
			newImage.mirror(true, true);
			images.push_back(newImage);
		}


		vector<ofImage> images2;
		for (const ofImage & prevImage : images) {
			if(rotate90) {
				newImage.setFromPixels(prevImage.getPixels());
				newImage.rotate90(1);
				images2.push_back(newImage);

			}
			if(rotate180) {
				newImage.rotate90(1);
				images2.push_back(newImage);

			}
			if(rotate270) {
				newImage.rotate90(1);
				images2.push_back(newImage);
			}
		}
		images.insert(images.end(), images2.begin(), images2.end());

		currentImage.setFromPixels(image.getPixels());

		for (ofImage & save_image : images) {
			string name = "image_" + ofToString(imageNum++, 4, '0') + ".png";
			ofLogNotice() << name;
			save_image.save(ofFilePath::join(  outputDir.path(), name));
		}

		progress = (float)distance<vector<ofFile>::const_iterator>(inputFiles.begin(), itrImages) / distance(inputFiles.begin(), inputFiles.end());
		ofSetWindowTitle(ofToString(progress * 100.0, 2) + "%");

		if (++itrImages == inputFiles.end()) {
			toggleProcessing();
		};
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (currentImage.isAllocated()) {
		currentImage.draw(0, 0);
	}

    auto settings = ofxImGui::Settings();
	gui.begin();
	if (!processing && ofxImGui::BeginWindow("Menu", settings, false)) {
		if(ImGui::Button("Load"))
		{
			auto dialogResult = ofSystemLoadDialog("Load ", true, ofToDataPath(""));
			if (dialogResult.bSuccess)
			{
                vector<string> ext = { "jpg", "png", "tif" };

                inputFiles = getFilesRecursively(dialogResult.getPath(), ext );
                itrImages = inputFiles.begin();

                ofLogNotice() << inputFiles.size() << " images loaded";
			}
		}
		if(ImGui::Button("Output"))
		{
			auto dialogResult = ofSystemLoadDialog("Load ", true, ofToDataPath(""));
			if (dialogResult.bSuccess)
			{
				outputDir.open(dialogResult.filePath);
			}
		}

		if(ImGui::Button(processing ? "Stop" : "Start"))
		{
            if (inputFiles.size() > 0) {
                processing = !processing;
                imageNum = 0;
            }
			
		}

		if (!processing && ofxImGui::BeginTree(options, settings)) {
			ofxImGui::AddParameter(flipVertical);
			ofxImGui::AddParameter(flipHorizontal);
			ofxImGui::AddParameter(flipBoth);
			ofxImGui::AddParameter(rotate90);
			ofxImGui::AddParameter(rotate180);
			ofxImGui::AddParameter(rotate270);
            ofxImGui::EndTree(settings);
		} 
        ofxImGui::EndWindow(settings);
	} 
    

	gui.end();
}
//--------------------------------------------------------------
vector<ofFile> ofApp::getFilesRecursively(const string path, const vector<string> exts) {
	ofDirectory inputDir(path);
    vector<ofFile> files;

    if (!inputDir.isDirectory()) {
        return files;
    }

    for (auto file : inputDir)
    {
        ofLogNotice() << file.getBaseName();
        if (file.isDirectory())
        {
            ofLogNotice() << file.getAbsolutePath();
            vector<ofFile> newFiles = getFilesRecursively(file.getAbsolutePath(), exts);
            files.insert(files.end(), newFiles.begin(), newFiles.end());
        }
        else
        {

            for (auto ext : exts) {
                if (file.getExtension() == ext) {
                    files.push_back(file);
                }
            }
        }
    }
    return files;
}

//--------------------------------------------------------------
void ofApp::toggleProcessing() {
	processing = !processing;
	imageNum = 0;
	itrImages = inputFiles.begin();
	progress = 0.0;
}