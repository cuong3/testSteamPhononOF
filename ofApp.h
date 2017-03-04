#pragma once

#include "ofMain.h"
#include "phonon.h"

class ofApp : public ofBaseApp{
	
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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
				
		float bufferIn[512];
		void audioIn(float * input, int bufferSize, int nChannels); 
		void audioOut(float * output, int bufferSize, int nChannels);
		ofSoundStream soundStream;


		//==== PHONON 3D or Steam Audio
		//store data for the IPLAudioBuffer
		IPLfloat32 iplDataDry[512];
		IPLfloat32 iplDataSpatilized[1024];

		//buffer object
		IPLAudioBuffer bufFromOf; //from OF to Phonon
		IPLAudioBuffer bufFromPhoton; //from Phonon back to OF

		//setup stuff
		IPLContext iplContext;
		IPLRenderingSettings iplRenderSettings;

		//renderer & effects
		IPLhandle *binRen; //binarual renderer
		IPLhandle *binEff; //object based binaural effect

		void setupPhonon()
		{
			
			iplRenderSettings.frameSize = 512;
			iplRenderSettings.samplingRate = 44100;
			iplRenderSettings.convolutionType = IPL_CONVOLUTIONTYPE_PHONON;

			//-- buffer dry (for OF)
			IPLAudioFormat formatIn;
			formatIn.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS; // intended to be played back by a single speaker
			formatIn.channelLayout = IPL_CHANNELLAYOUT_STEREO; //play over headphones
			formatIn.numSpeakers = 1; //number of channels in the audio data, input is mono
			formatIn.channelOrder = IPL_CHANNELORDER_INTERLEAVED; //LRLRLR...
			//setup two speakers, one on left, one on right, assume radius is 400
			IPLVector3 speakers[2];
			speakers[0].x = -400;
			speakers[0].y = 0;
			speakers[0].z = 0;
			speakers[1].x = 400;
			speakers[1].y = 0;
			speakers[1].z = 0;
			formatIn.speakerDirections = speakers;
			
			bufFromOf.format = formatIn;
			bufFromOf.numSamples = 512;
			bufFromOf.interleavedBuffer = new IPLfloat32[bufFromOf.numSamples];

			//-- buffer spatialize (for Phonon)
			IPLAudioFormat formatOut;
			formatOut.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS; // intended to be played back by a single speaker
			formatOut.channelLayout = IPL_CHANNELLAYOUT_STEREO; //play over headphones
			formatOut.numSpeakers = 2; //number of channels in the audio data
			formatOut.channelOrder = IPL_CHANNELORDER_INTERLEAVED; //LRLRLR...
			formatOut.speakerDirections = speakers;

			bufFromPhoton.format = formatOut;
			bufFromPhoton.numSamples = 512;
			bufFromPhoton.interleavedBuffer = new IPLfloat32[bufFromPhoton.numSamples * 2]; //output is stereo, so multiple by 2 channels

			//--- create renderer & effects
			binRen = new IPLhandle; //IMPORTANT, need to allocate it first, if not, will raise error
			IPLerror check = iplCreateBinauralRenderer(iplContext, iplRenderSettings, NULL, binRen);
			if (check != IPL_STATUS_SUCCESS)
				cout << "ERROR iplCreateBinauralRenderer: " << check << endl;

			binEff = new IPLhandle;
			check = iplCreateBinauralEffect(*binRen, formatIn, formatOut, binEff); //IMPORTANT: use *binRen, if not, will error
			if (check != IPL_STATUS_SUCCESS)
				cout << "ERROR iplCreateBinauralEffect: " << check << endl;


		}

		void spatilizeIPL(ofVec3f dirFromListener, float *in, int inSize, float *out, int outSize)
		{
			//-- get the direction from listener to the sound source
			IPLVector3 dir;
			dir.x = dirFromListener.x;
			dir.y = dirFromListener.y;
			dir.z = dirFromListener.z;

			//-- get the input samples to IPL buffer
			for (int i = 0; i < inSize; ++i)
				iplDataDry[i] = in[i];
			this->bufFromOf.interleavedBuffer = iplDataDry;


			//-- spatialize
			iplApplyBinauralEffect(*binEff, bufFromOf, dir, IPL_HRTFINTERPOLATION_NEAREST, bufFromPhoton);

			//-- output *** HEARING DISTORTED STUFF ***
			for (int i = 0; i < outSize; ++i)
				out[i] = this->bufFromPhoton.interleavedBuffer[i];

			////-- test: simply output from bufFromOf, works. I think the problem is with the bufFromPhoton data
			//for (int i = 0; i < inSize; ++i)
			//{
			//	out[i * 2] = bufFromOf.interleavedBuffer[i];
			//	out[i * 2 + 1] = bufFromOf.interleavedBuffer[i];
			//}

		}

		void exit()
		{
			soundStream.stop();
			iplDestroyBinauralEffect(binEff);
			iplDestroyBinauralRenderer(binRen);
		}
};
