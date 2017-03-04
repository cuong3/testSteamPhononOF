#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	 
	
	//ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
	
	soundStream.printDeviceList();
	
	setupPhonon();

	int bufferSize = 512;
	soundStream.setup(this, 2, 1, 44100, bufferSize, 4);
	soundStream.start();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
		
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){	

	for (int i = 0; i < bufferSize; i++){
		bufferIn[i] = input[i];
	}
	
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels) {


	spatilizeIPL(ofVec3f(200, 0, 0), bufferIn, 512, output, 1024); //test to see if sound comes from the right

	//for (int i = 0; i < bufferSize; i++) {
	//	output[i*nChannels] = bufferIn[i];
	//	output[i*nChannels+1] = bufferIn[i];
	//}

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 

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

