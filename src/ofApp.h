#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxMidi.h"
#include "ofxXmlSettings.h"

namespace fs = std::filesystem;

class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;

		ofTrueTypeFont font;

	const int WIDTH = 700;
	const int HEIGHT = 700;
	const string TITLE="Andyland ofxSampleKitCreator";
	const string VERSION="0.2";

	bool bShowGui = true;

	const string LBL_NONE = "[none]";
	const string LBL_CMD_MIDI_IN = "Midi In: ";
	const string LBLCMBMIDI = "Click hier to select MidiPort";
	const string LBLCMBPRESET = "Select Control-Preset";


	vector<string>logText;
	void addLog(string p);
    void showLog();

	ofxXmlSettings xmlSettings;
	ofxXmlSettings xmlPresets;

    void saveSettings();
	void restoreSettings();
	vector<string> vecPresets;
	string sActivePreset;
	int iActivePad = 0;


	ofxMidiIn midiIn;
	void newMidiMessage(ofxMidiMessage& eventArgs);
	void setMidiPort(string pPortName);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onButtonEvent(ofxDatGuiButtonEvent e);

	void setSampleFolder(int pSlot);
	void playSample(int pSlot, int pVelocity);
	void selectPreviousSample(int pSlot);
	void selectNextSample(int pSlot);
	
	void buildGui();
	void processMidi_ControlChange(ofxMidiMessage& message);
	void processMidi_NoteOn(ofxMidiMessage& message);

	void loadPresets();
	void setActivePreset(string pName);

	ofxDatGui *gui;
	ofxDatGuiButton* btnClear;
	ofxDatGuiDropdown* cmbMidiIn;
	ofxDatGuiDropdown* cmbPresets;

	ofxDatGui* panel[16];

	struct padData{
		int iPad;
		int iChannel;
		int iPlay;
		//int iPrevious;
		//int iNext;
	};
	
	vector<padData> vecPadmapping;
};

class myCustomTheme : public ofxDatGuiTheme{
public:
    myCustomTheme(){
        font.size = 12;
        //font.file = "path/to/font.ttf";
        init();
    }
};

class panelTheme : public ofxDatGuiTheme{
public:
    panelTheme(){
        font.size = 10;
		layout.height *=.75;
		color.background = hex(0xFF4081);
        init();
    }
};

class myButton : public ofxDatGuiButton{
public:
	myButton(string pLabel):ofxDatGuiButton(pLabel){}

	void setSlot(int p){
		iSlot=p;
	}

	int getSlot(){
		return iSlot;
	}
private:
	int iSlot=-1;
};


class myPanel : public ofxDatGui{

	public:
		inline static const string LBLBROWSE = "Browse";
		inline static const string LBLPLAY = "Play";
		inline static const string LBLPREVIOUS = "Previous";
		inline static const string LBLNEXT = "Next";
		
		myPanel(int pIndex, int pX, int pY, int pWidth):ofxDatGui(pX, pY){

			setTheme(new panelTheme());
			setWidth(pWidth);
			addHeader("Slot " + ofToString((pIndex)));
			myPanel::lblFolder = ((myButton * )addLabel(" --- "));
			myPanel::btnBrowse = ((myButton * )addButton(LBLBROWSE));
			myPanel::btnPlay = ((myButton * )addButton("Play"));
			myPanel::btnPrevious = ((myButton * )addButton("Previous"));
			myPanel::btnNext = ((myButton * )addButton("Next"));
			myPanel::lblFilename = ((myButton * )addLabel("Filename"));

			myPanel::btnBrowse->setSlot(pIndex);
			myPanel::btnPlay->setSlot(pIndex);
			myPanel::btnPrevious->setSlot(pIndex);
			myPanel::btnNext->setSlot(pIndex);
		}

		void setDirectory(string pDirectory){
			sDirectory = pDirectory;
			lblFolder->setLabel( fs::path( pDirectory ).filename() );
			for (const auto & entry : fs::directory_iterator(pDirectory)) {
				if((endsWith(ofToLower(entry.path()), ".wav"))||(endsWith(ofToLower(entry.path()), ".mp3"))) {
					addFile(entry.path());
				}
			}

			// Select the first file in the folder;
			if(vFiles.size()>0){
				iSelectedFile = 0;
				lblFilename->setLabel( fs::path( vFiles[iSelectedFile] ).filename() );
				mySample.load( vFiles[iSelectedFile] );
			}
		} 

		string getDirectory(){
			return sDirectory;
		}

		void addFile(string pPath){
			vFiles.push_back(pPath);
			sort(vFiles.begin(), vFiles.end());
		}

		void getFileList(){
			for(int i=0; i<vFiles.size(); i++){
				cout << vFiles[i] << endl;
			}
		}

		void selectNextSample(){
			if(vFiles.size()>0){
				if(iSelectedFile<vFiles.size()-1){
					iSelectedFile++;
				}else{
					iSelectedFile=0;
				}
				//cout << ofToString(vFiles[iSelectedFile]) << endl;
				//cout << fs::path( vFiles[iSelectedFile] ).filename() << endl;
				lblFilename->setLabel( fs::path( vFiles[iSelectedFile] ).filename() );
				mySample.load( vFiles[iSelectedFile] );
			}
		}

		void selectPreviousSample(){
			if(vFiles.size()>0){
				if(iSelectedFile>0){
					iSelectedFile--;
				}else{
					iSelectedFile=vFiles.size()-1;
				}
				lblFilename->setLabel( fs::path( vFiles[iSelectedFile] ).filename() );
				mySample.load( vFiles[iSelectedFile] );
			}
		}

		void play(int pVelocity){
			mySample.setVolume(pVelocity/127.0);
			mySample.play();
		}

		string getSelectedSample(){
			return vFiles[iSelectedFile];
		}

		int getSelectedSampleIndex(){
			return iSelectedFile;
		}

		void setSelectedSampleIndex(int pIndex){	
			if((vFiles.size()>0) && (pIndex>=0) && (pIndex<vFiles.size())){
				iSelectedFile=pIndex;
				lblFilename->setLabel( fs::path( vFiles[iSelectedFile] ).filename() );
				mySample.load( vFiles[iSelectedFile] );
			}
		}


	private:
		myButton* lblFolder;
		myButton* btnPlay;
		myButton* btnBrowse;
		myButton* btnPrevious;
		myButton* btnNext;
		myButton* lblFilename; 

		string sDirectory;
		vector<string> vFiles;
		int iSelectedFile = -1;

		bool endsWith(std::string const &str, std::string const &suffix) {
			if (str.length() < suffix.length()) {
				return false;
			}
			return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
		}

		ofSoundPlayer	mySample;
		
};
