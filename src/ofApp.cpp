#include "ofApp.h"
const int PADCOUNT=16;
//--------------------------------------------------------------
void ofApp::setup(){
	#ifdef TARGET_OSX
        addLog("System: OSX");
    #endif

	//ofSetEscapeQuitsApp(false);
	ofSetWindowShape(WIDTH, HEIGHT);
    ofSetWindowTitle(TITLE + " v" + VERSION);
	//ofSetFullscreen(true);
	ofSetBackgroundColor(ofColor(20, 20, 40));
    ofSetVerticalSync(true);
	loadPresets();
    
	buildGui();
    restoreSettings();

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	if(bShowGui){
    	showLog();
	}
}

//--------------------------------------------------------------
void ofApp::exit(){
	saveSettings();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key==' '){
        //bShowGui=!bShowGui;
        //gui->setVisible(bShowGui);
		stopAllSamples();
    }

	if(key=='-'){
		selectPreviousSample(iActivePad);
    }

	if(key=='+'){
		selectNextSample(iActivePad);
    }
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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    cout << "onDropdownEvent: "<< e.target->getName() << " " << e.target->getLabel() << " Selected" << endl;
	if(e.target->getName().compare(LBLCMBMIDI)==0){
		setMidiPort( e.target->getLabel() );
	}else if(e.target->getName().compare(LBLCMBPRESET)==0){
		setActivePreset( e.target->getLabel() );
	}
    
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    //cout << "onButtonEvent: Slot "<< ofToString( ((myButton * )e.target)->getSlot()) << " " << e.target->getLabel() << " Clicked" << endl;
	int slot = ((myButton * )e.target)->getSlot();
	if( e.target->getLabel().compare(myPanel::LBLBROWSE)==0){
		setSampleFolder( slot );
	}else if( e.target->getLabel().compare(myPanel::LBLPLAY)==0){
		playSample( slot, 127 );
	}else if( e.target->getLabel().compare(myPanel::LBLPREVIOUS)==0){
		selectPreviousSample( slot );
	}else if( e.target->getLabel().compare(myPanel::LBLNEXT)==0){
		selectNextSample( slot);
	}else if( e.target->getLabel().compare(LBL_EXPORTTOFOLDER)==0){
		exportToFolder();
	}else if( e.target->getLabel().compare(LBL_EXPORTASSINGLEWAV)==0){
		exportAsSingleWav();
	}
}


void ofApp::setSampleFolder(int pSlot){
	ofFileDialogResult result = ofSystemLoadDialog("Select Sample Folder", true);
	if(result.bSuccess) {
		string path = result.getPath();
		for(int i=0; i<PADCOUNT; i++){
			if( ((myPanel * )panel[i])->getSlot()==pSlot) {
				((myPanel * )panel[i])->setDirectory(path);
				break;
			}
		}
	}
}

void ofApp::playSample(int pSlot, int pVelocity){
	//cout << ((myPanel * )panel[pSlot])->getSelectedSample() << endl;
	for(int i=0; i<PADCOUNT; i++){
		if( ((myPanel * )panel[i])->getSlot()==pSlot) {
			((myPanel * )panel[i])->play(pVelocity);
			break;
		}
	}
}

void ofApp::stopAllSamples(){
	for(int i=0; i<PADCOUNT; i++){
		((myPanel * )panel[i])->stop();
	}
}

void ofApp::selectPreviousSample(int pSlot){
	for(int i=0; i<PADCOUNT; i++){
		if( ((myPanel * )panel[i])->getSlot()==pSlot ){
			((myPanel * )panel[i])->selectPreviousSample();
			break;
		}
	}
}

void ofApp::selectNextSample(int pSlot){
	for(int i=0; i<PADCOUNT; i++){
		if( ((myPanel * )panel[i])->getSlot()==pSlot) {
			((myPanel * )panel[i])->selectNextSample();
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::setMidiPort(string pPortName){
    midiIn.closePort();
    midiIn.removeListener(this);
    
    midiIn.openPort( pPortName ); // open a virtual port
    //midiIn.ignoreTypes(false, // sysex  <-- don't ignore timecode messages!
    //                   false, // timing <-- don't ignore clock messages!
    //                   true); // sensing
    
    // add ofApp as a listener
    midiIn.addListener(this);
    cout << "setMidiPort: " << pPortName << " Selected" << endl;
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& message) {
   //addLog("Midi INcoming:" + message.getStatusString(message.status));
   
    switch(message.status) {
        case MIDI_TIME_CLOCK:
            break;
        case MIDI_START: case MIDI_CONTINUE:
            break;
        case MIDI_STOP:
            break;
        case MIDI_NOTE_ON:
            processMidi_NoteOn(message);
            break;
        case MIDI_NOTE_OFF:
            //processMidi_NoteOff(message);
            break;
        case MIDI_CONTROL_CHANGE:
            processMidi_ControlChange(message);
            break;
        default:
            break;
    }
}

void ofApp::processMidi_ControlChange(ofxMidiMessage& message){
	//addLog("Midi in:" + ofToString(message.control) + ' ' + ofToString(message.value));
	if(message.control==99){

	}

}

void ofApp::processMidi_NoteOn(ofxMidiMessage& message){
	// everything is stored in vecPadmapping,
	for(int i=0; i<vecPadmapping.size(); i++){
		if((message.channel == vecPadmapping[i].iChannel) &&
		(message.pitch == vecPadmapping[i].iPlay) && (vecPadmapping[i].iAction == PAD_PLAYSAMPLE)){
			playSample(vecPadmapping[i].iPad, message.velocity);
			iActivePad = i;
			break;
		}else if((message.channel == vecPadmapping[i].iChannel) &&
		(vecPadmapping[i].iAction == PAD_CONTROL_PREV)){
			cout << "PREVIOUS Sample" << endl;
			selectPreviousSample(iActivePad);
			break;
		}else if((message.channel == vecPadmapping[i].iChannel) &&
		(vecPadmapping[i].iAction == PAD_CONTROL_NEXT)){
			cout << "NEXT Sample" << endl;
			selectNextSample(iActivePad);
			break;
		}

	}
}

void ofApp::buildGui(){
    font.load(OF_TTF_MONO, 16);

    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    vector<string> opts = {midiIn.getInPortList()/*"option - 1", "option - 2", "option - 3", "option - 4"*/};
    cmbMidiIn = gui->addDropdown(LBLCMBMIDI, opts);
	cmbPresets = gui->addDropdown(LBLCMBPRESET, vecPresets);

    // once the gui has been assembled, register callbacks to listen for component specific events //
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);

    gui->setTheme(new myCustomTheme() );
    gui->setWidth( ofGetScreenWidth() );

	int iTop = 120;
	int iPadding = 15;
	panel[0] = new myPanel(12, 12, iTop, WIDTH/4 -12);
	panel[1] = new myPanel(13, 12+WIDTH/4, iTop, WIDTH/4 -12);
	panel[2] = new myPanel(14, 12+2*WIDTH/4, iTop, WIDTH/4 -12);
	panel[3] = new myPanel(15, 12+3*WIDTH/4, iTop, WIDTH/4 -24);

	panel[4] = new myPanel(8, 12, panel[0]->getPosition().y + panel[0]->getHeight()*.75 +iPadding, WIDTH/4 -12);
	panel[5] = new myPanel(9, 12+WIDTH/4, panel[0]->getPosition().y + panel[0]->getHeight()*.75 +iPadding, WIDTH/4 -12);
	panel[6] = new myPanel(10, 12+2*WIDTH/4, panel[0]->getPosition().y + panel[0]->getHeight()*.75 +iPadding, WIDTH/4 -12);
	panel[7] = new myPanel(11, 12+3*WIDTH/4, panel[0]->getPosition().y + panel[0]->getHeight()*.75 +iPadding, WIDTH/4 -24);

	panel[8] = new myPanel(4, 12, panel[4]->getPosition().y + panel[4]->getHeight()*.75 +iPadding, WIDTH/4-12);
	panel[9] = new myPanel(5, 12+WIDTH/4, panel[4]->getPosition().y + panel[4]->getHeight()*.75 +iPadding, WIDTH/4 -12);
	panel[10] = new myPanel(6, 12+2*WIDTH/4, panel[4]->getPosition().y + panel[4]->getHeight()*.75 +iPadding, WIDTH/4 -12);
	panel[11] = new myPanel(7, 12+3*WIDTH/4, panel[4]->getPosition().y + panel[4]->getHeight()*.75 +iPadding, WIDTH/4 -24);

	panel[12] = new myPanel(0, 12, panel[8]->getPosition().y + panel[8]->getHeight()*.75 +iPadding, WIDTH/4-12);
	panel[13] = new myPanel(1, 12+WIDTH/4, panel[8]->getPosition().y + panel[8]->getHeight()*.75 +iPadding, WIDTH/4-12 );
	panel[14] = new myPanel(2, 12+2*WIDTH/4, panel[8]->getPosition().y + panel[8]->getHeight()*.75 +iPadding, WIDTH/4-12 );
	panel[15] = new myPanel(3, 12+3*WIDTH/4, panel[8]->getPosition().y + panel[8]->getHeight()*.75 +iPadding, WIDTH/4-24);

	for(int i=0; i<PADCOUNT; i++){
		panel[i]->onButtonEvent(this, &ofApp::onButtonEvent);	
	}

	bottomGui = new ofxDatGui( ofxDatGuiAnchor::BOTTOM_LEFT );
	btnExportToFolder = bottomGui->addButton(LBL_EXPORTTOFOLDER);
//	btnExportAsSingleWav = bottomGui->addButton(LBL_EXPORTASSINGLEWAV);
	bottomGui->addBreak()->setHeight(10.0f);
    bottomGui->onButtonEvent(this, &ofApp::onButtonEvent);
    bottomGui->setTheme(new myCustomTheme() );
    bottomGui->setWidth( ofGetScreenWidth() );
}

//--------------------------------------------------------------
void ofApp::addLog(string p){
    if(logText.size()>17){
        logText.erase( logText.begin() );
    }
    logText.push_back( p );
}

//--------------------------------------------------------------
void ofApp::showLog(){
    string sTmp;
    //string sOscInfo="OSC Format: NoteOn/Channel/Pitch  ControlChange/Channel/Value\n\r";
    //sOscInfo += "OSC In Port:" + ofToString(incomingPortOsc) + "   OSC Out Port:" + ofToString(outGoingPortOsc) + " (see settings.xml)";
    //font.drawString(sOscInfo, 10,panel[8]->getPosition().y + panel[8]->getHeight()*.75 + 10);
    
    for(int i=0; i<logText.size(); i++){
        sTmp += logText[i] + "\n";
    }
    //font.drawString(sTmp, 10,btnClear->getY() + btnClear->getHeight() +25);
	font.drawString(sTmp, 10,panel[12]->getPosition().y + panel[12]->getHeight()*.75 +100);
}

void ofApp::restoreSettings(){
    if (xmlSettings.loadFile("settings.xml")) {
        addLog("XML loaded");
    }else{
        addLog("Could not load xml. Reverting to default values.");
    }
    
    string sMidiInPort = xmlSettings.getValue("midiInPort", "");
    if((!sMidiInPort.empty()) && (sMidiInPort.compare(LBL_NONE)!=0) ){
        vector<string> optsMidi_In = {midiIn.getInPortList()};
        optsMidi_In.push_back(LBL_NONE);

        for(int i=0; i<optsMidi_In.size(); i++){
            if(optsMidi_In[i] == sMidiInPort){
                cmbMidiIn->select(i);
                setMidiPort(sMidiInPort);
                cmbMidiIn->setLabel(LBL_CMD_MIDI_IN + sMidiInPort);
                break;
            }
        }
    }else{
        cout << "LoadFromSettings: Midi in disabled";
        cmbMidiIn->setLabel(LBL_CMD_MIDI_IN + sMidiInPort);
        //bMidiIn_Active = false;
    }

	for(int i=0; i<PADCOUNT; i++){
		string tmp=xmlSettings.getValue("Pad_" + ofToString(i) + "_Directory", "");
		if(ofDirectory::doesDirectoryExist(tmp)){
			if(tmp.length()>0){
				((myPanel * )panel[i])->setDirectory(tmp);
			}
		}
	}

	for(int i=0; i<PADCOUNT; i++){
		string dir=xmlSettings.getValue("Pad_" + ofToString(i) + "_Directory", "");
		if(ofDirectory::doesDirectoryExist(dir)){
			string tmp=xmlSettings.getValue("Pad_" + ofToString(i) + "_Sample", "");
			if(ofToInt(tmp)!=0){
				((myPanel * )panel[i])->setSelectedSampleIndex((ofToInt(tmp))-1);
			}
		}
	}

	string tmp=xmlSettings.getValue("controlpreset", "");
	if(tmp.length()>0){
		setActivePreset(tmp);
		addLog("Control-Preset " + tmp + " loaded.");
		cmbPresets->setLabel(tmp);
    }else{
		addLog("Could not load Control-Preset");
	}
}

void ofApp::saveSettings(){
    if(!cmbMidiIn->getSelected()->getName().empty()){
        xmlSettings.setValue("midiInPort", cmbMidiIn->getSelected()->getName() );
	}
	for(int i=0; i<PADCOUNT; i++){
		xmlSettings.setValue("Pad_" + ofToString(i) + "_Directory",((myPanel * )panel[i])->getDirectory());
		xmlSettings.setValue("Pad_" + ofToString(i) + "_Sample",((myPanel * )panel[i])->getSelectedSampleIndex()+1); //+1 wegen returncode von ofToString beim restore()
	}
	xmlSettings.setValue("controlpreset", sActivePreset);
    xmlSettings.save();
}

void ofApp::loadPresets(){
	if (xmlPresets.loadFile("inputPresets.xml")) {
        addLog("Presetfile loaded");
		//cout << "Tags:" << ofToString(xmlPresets.getNumTags("preset")) << endl;
		//cout << "Tags:" << ofToString(xmlPresets.getAttribute("preset","name","",0)) << endl;

		for(int i=0; i<xmlPresets.getNumTags("preset"); i++){
			vecPresets.push_back(ofToString(xmlPresets.getAttribute("preset","name","",i)));
		}

    }else{
        addLog("Could not load presets. Control app with mouse");
    }
}

void ofApp::setActivePreset(string pName){
	sActivePreset = pName;

	int iWhich;

	// Get the index of the selected preset via the position of
	// its name within the vector that stores the presetnames.
	for(int i=0; i<vecPresets.size(); i++){
		if(pName.compare(vecPresets[i])==0){
			iWhich=i;
			break;
		}
	}

	xmlPresets.pushTag("preset",iWhich);
	for(int i=0; i<PADCOUNT; i++){
		xmlPresets.pushTag("trigger", i);
		
		padData tmp;
		tmp.iPad = xmlPresets.getValue("pad", 0);
		tmp.iChannel = xmlPresets.getValue("midiChannel", 0);
		tmp.iPlay = xmlPresets.getValue("note", 0);
		tmp.iAction = PAD_PLAYSAMPLE;
		vecPadmapping.push_back(tmp);
		xmlPresets.popTag();
	}

	xmlPresets.pushTag("control_previous");
	{
		padData tmp;
		tmp.iChannel = xmlPresets.getValue("midiChannel", 0);
		tmp.iPlay = xmlPresets.getValue("note", 0);
		tmp.iAction = PAD_CONTROL_PREV;
		vecPadmapping.push_back(tmp);
		xmlPresets.popTag();
	}
	
	xmlPresets.pushTag("control_next");
	{	
		padData tmp;
		tmp.iChannel = xmlPresets.getValue("midiChannel", 0);
		tmp.iPlay = xmlPresets.getValue("note", 0);
		tmp.iAction = PAD_CONTROL_NEXT;
		vecPadmapping.push_back(tmp);
		xmlPresets.popTag();
	}
}

void ofApp::exportToFolder(){
	ofFileDialogResult result = ofSystemLoadDialog("Select Export Folder", true);
	if(result.bSuccess) {
		string path = result.getPath();
		for(int i=0; i<PADCOUNT; i++){
			string tmpFiles = ((myPanel * )panel[i])->getSelectedSample();
			if(tmpFiles.size()>0){
				cout << tmpFiles << endl;
				string tmpFilename = fs::path( tmpFiles ).filename();
				ofFile::copyFromTo(tmpFiles,path + std::filesystem::path::preferred_separator + tmpFilename , false);
			}
		}
	}
}

void ofApp::exportAsSingleWav(){

}