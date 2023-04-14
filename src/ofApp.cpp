#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	vidPlayer.load("finger.mov");
	vidPlayer.play();
	vidPlayer.setLoopState(OF_LOOP_NORMAL);

	colorImg.allocate(320, 240);
	grayImg.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);

	gui.setup();
	gui.add(overallSpeed.setup("speed", 1, 0, 3));
	gui.add(noiseAmount.setup("noise", 1, 0, 3));
	gui.add(trail.setup("trail", 6.0, 0, 20));
	gui.add(threshold.setup("threshold", 80, 0, 256));
	gui.add(bLearnBackground.setup("capture bg", false));

	p.assign(100, particle());

	for (int i = 0;i < p.size();i++) {
		p[i].setup();
	}

	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetFrameRate(60);

	bool bNewFrame = false;

	vidPlayer.update();
	bNewFrame = vidPlayer.isFrameNew();

	if (bNewFrame == true) {
		colorImg.setFromPixels(vidPlayer.getPixels());
		grayImg = colorImg;

		if (bLearnBackground == true) {
			grayBg = grayImg;
			bLearnBackground = false;
		}

		grayDiff.absDiff(grayBg, grayImg);
		grayDiff.threshold(threshold);

		contourFinder.findContours(grayDiff, 20, (340 * 20) / 2.0, 10, true);
	}

	vector<ofVec2f> blobPts;
	for (int i = 0;i < contourFinder.nBlobs;i++) {
		for (int j = 0;j < contourFinder.blobs[i].pts.size();j++) {
			blobPts.push_back(contourFinder.blobs[i].pts[j] * ofGetWidth() / 320.0);
		}
	}

	for (int i = 0;i < p.size();i++) {
		p[i].repel(blobPts);
		p[i].update(overallSpeed, noiseAmount);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(0, 0, 0, trail);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofSetColor(255, 255, 255, 255);
	for (int i = 0;i < p.size();i++) {
		p[i].draw(1);
	}

	// draw the incoming, the grayscale, the bg, and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20, 20);
	grayImg.draw(360, 20);
	grayBg.draw(20, 280);
	grayDiff.draw(360, 280);

	// then draw the contours
	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(360, 540, 320, 240);
	ofSetHexColor(0xffffff);

	contourFinder.draw(360, 540);

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
