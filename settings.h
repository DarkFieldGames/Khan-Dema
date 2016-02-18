class Settings {
public:
    float soundvol;
    float musicvol;
    string rendername;
    core::dimension2d<u32> resolution;
    int windowed;
    void get();
    void defaults();
    void write();
    Settings();
};

Settings::Settings () {
    get();
};

void Settings::defaults() {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        resolution = nulldevice->getVideoModeList()->getDesktopResolution();
        windowed = 0;
        nulldevice -> drop();
        soundvol = 0.50;
        musicvol = 0.15;
        rendername = "opengl";
}

void Settings::write() {
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );  
    doc.LinkEndChild( decl );  
 
    TiXmlElement * settingsxml = new TiXmlElement( "settings" );  
    doc.LinkEndChild( settingsxml );  
    
    TiXmlElement * resolutionxml;
    resolutionxml = new TiXmlElement( "resolution" );  
    settingsxml->LinkEndChild( resolutionxml );  
    resolutionxml->SetAttribute("width", resolution.Width);
    resolutionxml->SetAttribute("height", resolution.Height);
    if (windowed) {
        resolutionxml->SetAttribute("windowed", "true");
    } else {
        resolutionxml->SetAttribute("windowed", "false");
    }
    
    TiXmlComment * comment = new TiXmlComment();
    comment->SetValue(" display options are 'opengl, 'software' or 'software2'. Maybe 'directx' will be available in future." );  
    settingsxml->LinkEndChild( comment );  
    
    TiXmlElement * displayxml;
    displayxml = new TiXmlElement( "display" );  
    settingsxml->LinkEndChild( displayxml );  
    displayxml->SetAttribute("render", rendername);
    
    TiXmlElement * volumexml;
    volumexml = new TiXmlElement( "volume" );  
    settingsxml->LinkEndChild( volumexml );  
    char soundvolxml[32];
    char musicvolxml[32];
    sprintf(soundvolxml, "%f", soundvol);
    volumexml->SetAttribute("sound", soundvolxml);
    sprintf(musicvolxml, "%f", musicvol);
    volumexml->SetAttribute("music", musicvolxml);
 
    doc.SaveFile( "settings.xml" );  
}

void Settings::get() {
    TiXmlDocument doc("settings.xml");
    if (!doc.LoadFile()) {
        defaults();
    } else {
        TiXmlHandle hDoc(&doc);
        TiXmlElement* pElem;
        TiXmlHandle hRoot(0);
        
        pElem=hDoc.FirstChildElement().Element();
        if (!pElem) {
            defaults();
        };
        hRoot=TiXmlHandle(pElem);
        soundvol = atof(hRoot.FirstChild( "volume" ).Element()->Attribute("sound"));
        musicvol = atof(hRoot.FirstChild( "volume" ).Element()->Attribute("music"));
        rendername = hRoot.FirstChild( "display" ).Element()->Attribute("render");
        resolution.Width = atoi(hRoot.FirstChild( "resolution" ).Element()->Attribute("width"));
        resolution.Height = atoi(hRoot.FirstChild( "resolution" ).Element()->Attribute("height"));
        string windowedt = hRoot.FirstChild( "resolution" ).Element()->Attribute("windowed");
        if (windowedt == "true") {
            windowed = true;
        } else {
            windowed = false;
        }
    
    }
};