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

	const int WIDTH = 1400;
	const int HEIGHT = 1600;
	const string TITLE="ofxSampleKitCreator";
	const string VERSION="0.8";
	const string WEBSITE = "                                  www.Andyland.info";

	bool bShowGui = true;

	const string LBL_NONE = "[none]";
	const string LBL_CMD_MIDI_IN = "Midi In: ";
	const string LBLCMBMIDI = "Click here to select MidiPort";
	const string LBLCMBPRESET = "Select Control-Preset";
	const string LBLCMBVELOCITY = "Select how to treat incomfing midi";
	const string LBL_EXPORTTOFOLDER = "Export to Folder";
	const string LBL_EXPORTASSINGLEWAV = "Export as single WAV File";
	const string LBL_VELOCITYFIXED = "Play samples with fixed velocity";
	const string LBL_VELOCITYVARI = "Play samples with Controller's velocity";

	// For them Pads
	const int PAD_CONTROL_NONE = -1;
	const int PAD_CONTROL_PREV = 1;
	const int PAD_CONTROL_NEXT = 2;
	const int PAD_PLAYSAMPLE = 4;

	vector<string> vVelo = {LBL_VELOCITYVARI, LBL_VELOCITYFIXED};
	bool bIncomingVelocityFixed=false;

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
	void stopAllSamples();
	void selectPreviousSample(int pSlot);
	void selectNextSample(int pSlot);
	
	void buildGui();
	void processMidi_ControlChange(ofxMidiMessage& message);
	void processMidi_NoteOn(ofxMidiMessage& message);

	void loadPresets();
	void setActivePreset(string pName);
	void exportToFolder();
	void exportAsSingleWav();

	ofxDatGui *gui;
	ofxDatGuiDropdown* cmbMidiIn;
	ofxDatGuiDropdown* cmbPresets;
	ofxDatGuiDropdown* cmbVelocity;


	ofxDatGui* panel[16];

	ofxDatGui *bottomGui;
	ofxDatGuiButton* btnExportToFolder;
	ofxDatGuiButton* btnExportAsSingleWav;
	struct padData{
		int iPad;
		int iChannel;
		int iPlay;
		int iAction = -1;
	};
	
	vector<padData> vecPadmapping;
};

class myCustomTheme : public ofxDatGuiTheme{
public:
    myCustomTheme(){
        font.size = 12;
		layout.upperCaseLabels = false;
        //font.file = "path/to/font.ttf";
        init();
    }
};

class panelTheme : public ofxDatGuiTheme{
public:
    panelTheme(){
        font.size = 10;
		layout.height *=.75;
		color.background = hex(0x0b2a70 /*0xFF4081*/);
		layout.upperCaseLabels = false;
		layout.vMargin = 2.0f;
		//stripe.visible=false;
		stripe.label = ofColor::fromHex(0x294c9e);
		stripe.button = ofColor::fromHex(0xFF0000);
        init();
    }
};

class myButton : public ofxDatGuiButton{
public:
	myButton(string pLabel):ofxDatGuiButton(pLabel){

	}
	
	void setSlot(int p){
		iSlot=p;
		
	}

	int getSlot(){
		return iSlot;
	}

	// BETA
	void setColor(int pColor){
		mStyle.stripe.color = pColor;
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
			iSlot = pIndex;
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
			vFiles.clear();
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

		vector<string> getFileVector(){
			return vFiles;
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

		void stop(){
			mySample.stop();
		}

		string getSelectedSample(){
			if(iSelectedFile!=-1){
				return vFiles[iSelectedFile];
			}else{
				return "";
			}
			
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

		int getSlot(){
			return iSlot;
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
		int iSlot;

		bool endsWith(std::string const &str, std::string const &suffix) {
			if (str.length() < suffix.length()) {
				return false;
			}
			return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
		}

		ofSoundPlayer	mySample;
		
};
