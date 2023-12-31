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
	const string VERSION="0.18";
	const string WEBSITE = "                                  www.Andyland.info";

	bool bShowGui = true;

	const string LBL_NONE = "[none]";
	const string LBL_CMD_MIDI_IN = "Midi In: ";
	const string LBLCMBMIDI = "Click here to select MidiPort";
	const string LBLCMBPRESET = "Select Control-Preset";
	const string LBLCMBVELOCITY = "Select how to treat incoming midi";
	const string LBL_RANDOMIZE = "Randomize samples...";
	const string LBL_PLAYSAMPLESINORDER = "Play samples in order";
	const string LBL_EXPORTTOFOLDER = "Export Samples to Folder";
	const string LBL_EXPORTASSINGLEWAV = "Export as single WAV File";
	const string LBL_VELOCITYFIXED_75 = "Play samples with fixed velocity (75)";
	const string LBL_VELOCITYFIXED_100 = "Play samples with fixed velocity (100)";
	const string LBL_VELOCITYFIXED_127 = "Play samples with fixed velocity (127)";
	const string LBL_VELOCITYVARI = "Play samples with Controller's velocity";

	// For them Pads
	const int PAD_CONTROL_NONE = -1;
	const int PAD_CONTROL_PREV = 1;
	const int PAD_CONTROL_NEXT = 2;
	const int PAD_PLAYSAMPLE = 4;

	vector<string> vVelo = {LBL_VELOCITYVARI, LBL_VELOCITYFIXED_75, LBL_VELOCITYFIXED_100, LBL_VELOCITYFIXED_127};
	int iIncomingVelocityFixed=-1;

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

	//void setSampleFolder(int pSlot);
	void stopAllSamples();
	
	void buildGui();
	void processMidi_ControlChange(ofxMidiMessage& message);
	void processMidi_NoteOn(ofxMidiMessage& message);

	void loadPresets();
	void setActivePreset(string pName);
	void playSample(int pSlot, int pVelocity);
	void selectPreviousSample(int pSlot);
	void selectNextSample(int pSlot);
	void exportToFolder();
	void randomizeSamples();
	void exportAsSingleWav();
	void playSamplesInOrder();

	ofxDatGui *gui;
	ofxDatGuiDropdown* cmbMidiIn;
	ofxDatGuiDropdown* cmbPresets;
	ofxDatGuiDropdown* cmbVelocity;


	ofxDatGui* panel[16];

	ofxDatGui *bottomGui;
	ofxDatGuiButton* btnRandomize;
	ofxDatGuiButton* btnPlaySamplesInOrder;
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

class buttonTheme : public ofxDatGuiTheme{
public:
    buttonTheme(){
        font.size = 10;
		layout.height *=.75;
		color.background = hex(0x0b2a70 /*0xFF4081*/);
		layout.upperCaseLabels = false;
		layout.vMargin = 2.0f;
		stripe.visible=false;
		//label.stripe.visible = false;
		//stripe.label = ofColor::fromHex(0x294c9e);
		stripe.button = ofColor::fromHex(0xFF0000);
        init();
    }
};

class slotTheme : public ofxDatGuiTheme{
public:
    slotTheme(){
        font.size = 10;
		layout.height *=.75;
		color.background = hex( 0xc93a3a /*0xFF4081*/);
		layout.upperCaseLabels = false;
		layout.vMargin = 2.0f;
		stripe.visible=false;
		//stripe.label = ofColor::fromHex(0x294c9e);
		//stripe.button = ofColor::fromHex(0xFF0000);
        init();
    }
};

class labelTheme : public ofxDatGuiTheme{
public:
    labelTheme(){
        font.size = 10;
		layout.height *=.75;
		color.background = hex( 0x444444);
		layout.upperCaseLabels = false;
		layout.vMargin = 2.0f;
		stripe.visible=false;
		//stripe.label = ofColor::fromHex(0x294c9e);
		//stripe.button = ofColor::fromHex(0xFF0000);
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
		inline static const string LBLLOCK = "Lock";
		
		myPanel(int pIndex, int pX, int pY, int pWidth):ofxDatGui(pX, pY){

			iSlot = pIndex;
			setWidth(pWidth);
			
			btnLock = addToggle(LBLLOCK);
			myPanel::btnLock->setLabel("Slot " + ofToString(pIndex));
			myPanel::btnLock->setTheme(new slotTheme() );
			
			myPanel::btnBrowse = ((myButton * )addButton(LBLBROWSE));
			myPanel::btnBrowse -> setTheme(new buttonTheme());

			myPanel::lblFolder = ((myButton * )addLabel(" --- "));
			myPanel::lblFolder -> setTheme(new labelTheme());

			myPanel::btnPlay = ((myButton * )addButton("Play"));
			myPanel::btnPlay -> setTheme( new buttonTheme);

			myPanel::btnPrevious = ((myButton * )addButton("Previous"));
			myPanel::btnPrevious -> setTheme(new buttonTheme());

			myPanel::btnNext = ((myButton * )addButton("Next"));
			myPanel::btnNext -> setTheme(new buttonTheme());

			myPanel::lblFilename = ((myButton * )addLabel("Filename"));
			myPanel::lblFilename -> setTheme(new labelTheme());

			myPanel::btnBrowse->setSlot(pIndex);
			myPanel::btnPlay->setSlot(pIndex);
			myPanel::btnPrevious->setSlot(pIndex);
			myPanel::btnNext->setSlot(pIndex);

			btnLock->onToggleEvent(this, &myPanel::btnActionTriggerToggle);
			btnNext->onButtonEvent(this, &myPanel::btnActionNextSample);
			btnPrevious->onButtonEvent(this, &myPanel::btnActionPreviousSample);
			btnPlay->onButtonEvent(this, &myPanel::btnActionPlaySample);
			btnBrowse->onButtonEvent(this, &myPanel::btnActionSetDirectory);

		}

		void btnActionTriggerToggle(ofxDatGuiToggleEvent e){
			cout << "ofApp.h TriggerToggle " << ofToString(iSlot) << " " << e.checked <<endl;
			bIsLocked = e.checked;
		}

		void btnActionNextSample(ofxDatGuiButtonEvent e){
			selectNextSample();
		}

		void btnActionPreviousSample(ofxDatGuiButtonEvent e){
			selectPreviousSample();
		}

		void btnActionPlaySample(ofxDatGuiButtonEvent e){
			play(127.0);
		}

		void btnActionSetDirectory(ofxDatGuiButtonEvent e){
			ofFileDialogResult result = ofSystemLoadDialog("Select Sample Folder", true);
			if(result.bSuccess) {
				string path = result.getPath();
				setDirectory(path);
			}
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
				setSelectedSampleIndex(iSelectedFile);
			}

			// When intentionally selecting a new Directory the slot will be unlocked
			setLock(false);
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
			if(bIsLocked){
				cout << "Pad " << iSlot << " is locked" << endl;
				return;
			}
			if(vFiles.size()>0){
				if(iSelectedFile<vFiles.size()-1){
					iSelectedFile++;
				}else{
					iSelectedFile=0;
				}
				setSelectedSampleIndex(iSelectedFile);
			}
		}

		void selectPreviousSample(){
			if(bIsLocked){
				cout << "Pad " << iSlot << " is locked" << endl;
				return;
			}
			if(vFiles.size()>0){
				if(iSelectedFile>0){
					iSelectedFile--;
				}else{
					iSelectedFile=vFiles.size()-1;
				}
				setSelectedSampleIndex(iSelectedFile);
			}
		}

		void randomize(){
			if(bIsLocked){
				cout << "Pad " << iSlot << " is locked" << endl;
				return;
			}
			if(vFiles.size()>0){
				int randomNumber = (int) ofRandom(0,vFiles.size());
				iSelectedFile = randomNumber;
				setSelectedSampleIndex(iSelectedFile);
			}
		}

		void play(int pVelocity){
			mySample.setVolume(pVelocity/127.0);
			mySample.play();
		}

		bool isPlaying(){
			return mySample.isPlaying();
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
				mySample.load( vFiles[iSelectedFile] );

				string tmp = "(" + ofToString(iSelectedFile+1) + ") ";
				tmp += fs::path( vFiles[iSelectedFile] ).filename();
				lblFilename->setLabel( tmp );
			}
		}

		int getSlot(){
			return iSlot;
		}

		bool getLock(){
			return bIsLocked;
		}

		void setLock(bool p){
			bIsLocked = p;
			btnLock->setChecked( p );
		}



	private:
		myButton* lblFolder;
		myButton* btnPlay;
		myButton* btnBrowse;
		myButton* btnPrevious;
		myButton* btnNext;
		myButton* lblFilename; 
		ofxDatGuiToggle* btnLock;

		string sDirectory;
		vector<string> vFiles;
		int iSelectedFile = -1;
		int iSlot;
		bool bIsLocked = false;

		bool endsWith(std::string const &str, std::string const &suffix) {
			if (str.length() < suffix.length()) {
				return false;
			}
			return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
		}

		ofSoundPlayer	mySample;
		
};
