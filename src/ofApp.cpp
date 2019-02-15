// Example 7: Background Subtraction
// This is ofApp.cpp
#include "ofApp.h"

//---------------------
void ofApp::setup(){
    vidGrabber.setup(0, 0);
    
    colorImg.allocate(0,0);
    grayImage.allocate(0,0);
    grayBg.allocate(0,0);
    grayDiff.allocate(0,0);
    
    bLearnBackground = true;
    thresholdValue = 30;
}

//---------------------
void ofApp::update(){
    
    // Ask the video player to update itself.
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()){ // If there is fresh data...
        
        // Copy the data from the video player into an ofxCvColorImage
        colorImg.setFromPixels(vidGrabber.getPixels());
        
        // Make a grayscale version of colorImg in grayImage
        grayImage = colorImg;
        
        // If it's time to learn the background;
        // copy the data from grayImage into grayBg
        if (bLearnBackground == true){
            grayBg = grayImage; // Note: this is 'operator overloading'
            bLearnBackground = false; // Latch: only learn it once.
        }
        
        // Take the absolute value of the difference
        // between the background and incoming images.
        grayDiff.absDiff(grayBg, grayImage);
        
        // Perform an in-place thresholding of the difference image.
        grayDiff.threshold(thresholdValue);
        
        // Find contours whose areas are betweeen 20 and 25000 pixels.
        // "Find holes" is true, so we'll also get interior contours.
        contourFinder.findContours(grayDiff, 2000, 25000, 10, true);
    }
}

//---------------------
void ofApp::draw(){
    //ofBackground(100,100,100);
    
    ofSetHexColor(0xffffff);
    //colorImg.draw(20,20);    // The incoming color image
    grayImage.draw(0,0);  // A gray version of the incoming video
    grayBg.draw(0,0);     // The stored background image
    grayDiff.draw(0,0);  // The thresholded difference image
    
    ofNoFill();
    
    // Draw each blob individually from the blobs vector
    int numBlobs = contourFinder.nBlobs;
    for (int i=0; i<numBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }
}

//---------------------
void ofApp::keyPressed(int key){
    bLearnBackground = true;
}
