
class Pos {
public:
    int X;
    int Y;
    int Z;
    void set(int,int,int);
    Pos (int,int,int);
    Pos ();
};

Pos::Pos () {
    X=0;Y=0;Z=0;
}

Pos::Pos (int a, int b, int c) {
  X=a;Y=b;Z=c;
};

void Pos::set (int a, int b, int c) {
  X=a;Y=b;Z=c;
};

class Mesh {
public:
    Pos pos;
    string file;
    string texture;
    float scale;
};

class Light {
public:
    Pos pos;
    int colour [3];
    int range;
};

class Billboard {
public:
    Pos pos;
    int size [2];
    string texture;
};



class Archetype {
public:
    string name;
    int duration;
    int size [2];
    int health;
    int pointvalue;
    int damagenumber;
    vector<int> damagedata;
    list<Mesh> meshes;
    list<Light> lights;
    list<Billboard> billboards;
    float smoothness;
    vector<Pos> path;
};

vector<int> string_split_comma(string sizestr) {
    vector<int> size;
    string sizenumst;
    stringstream stream(sizestr);
    while( getline(stream, sizenumst, ',') ) {
        size.push_back(atoi(sizenumst.c_str()));
    }
    return size;
}

map<string,Archetype> parse_factory (const char* filename) {
    map<string,Archetype> factory;
    map<string,Archetype> empty;
    
    TiXmlDocument doc(filename);
    if (!doc.LoadFile()) return empty;
    
    TiXmlHandle hDoc(&doc);
    TiXmlElement* pElem;
    TiXmlHandle hRoot(0);
    
    pElem=hDoc.FirstChildElement().Element();
    if (!pElem) return empty;
    string m_name=pElem->Value();

    // save this for later
    hRoot=TiXmlHandle(pElem);
    
    int damagetypes = atoi(hRoot.Element()->Attribute("damagetypes"));
    
    TiXmlElement* pArchetypeNode=hRoot.FirstChild( "archetype" ).Element();
    for( pArchetypeNode; pArchetypeNode; pArchetypeNode=pArchetypeNode->NextSiblingElement())
    {
        Archetype currArch;
        currArch.name = pArchetypeNode->Attribute("name");
        currArch.duration = atoi(pArchetypeNode->Attribute("duration"));
        
        string sizestr = pArchetypeNode->FirstChildElement("data")->Attribute("size");
        // split size (eg. "70,60"), convert to vector of ints
        // sort out +r1 later
        vector<int> size;
        string sizenumst;
        stringstream stream(sizestr);
        while( getline(stream, sizenumst, ',') ) {
            istringstream intstream(sizenumst);
            int foo;
            intstream >> foo;
            size.push_back(foo);
        };
        currArch.size[0] = size[0];
        currArch.size[1] = size[1];
        
        currArch.health = atoi(pArchetypeNode->FirstChildElement("data")->Attribute("health"));
        currArch.pointvalue = atoi(pArchetypeNode->FirstChildElement("data")->Attribute("pointvalue"));
        currArch.damagenumber = atoi(pArchetypeNode->FirstChildElement("data")->Attribute("damagenumber"));
        
        for(int i= 0; i <= damagetypes; i++) {
            currArch.damagedata.push_back(0);
        }
        
        
        TiXmlElement* pData=pArchetypeNode->FirstChildElement( "data" )->FirstChildElement("damage");
        for( pData; pData; pData=pData->NextSiblingElement()) {
            int ref = atoi(pData->Attribute("num"));
            currArch.damagedata.at(atoi(pData->Attribute("num"))) = atoi(pData->Attribute("strength"));
        }
        
        // Iterating over meshes
        TiXmlElement* pMesh=pArchetypeNode->FirstChildElement( "structure" )->FirstChildElement("mesh");
        for( pMesh; pMesh; pMesh=pMesh->NextSiblingElement("mesh")) {
            
            Mesh currMesh;
            
            vector<int> pos = string_split_comma(pMesh->Attribute("pos"));
            currMesh.pos.set(pos.at(0),pos.at(1),pos.at(2));
            
            currMesh.file = pMesh->Attribute("file");
            
            currMesh.texture = pMesh->Attribute("texture");
            
            if (pMesh->Attribute("scale")) {
                currMesh.scale = atof(pMesh->Attribute("scale"));
            } else {
                currMesh.scale = 1.0;
            }
            currArch.meshes.push_back(currMesh);
        }
        
        // Iterating over lights
        TiXmlElement* pLight=pArchetypeNode->FirstChildElement( "structure" )->FirstChildElement("light");
        for( pLight; pLight; pLight=pLight->NextSiblingElement("light")) {
            Light currLight;
            
            vector<int> pos = string_split_comma(pLight->Attribute("pos"));
            currLight.pos.set(pos.at(0),pos.at(1),pos.at(2));
            
            vector<int> color = string_split_comma(pLight->Attribute("colour"));
            currLight.colour[0] = color.at(0);
            currLight.colour[1] = color.at(1);
            currLight.colour[2] = color.at(2);
            
            currLight.range = atoi(pLight->Attribute("range"));
            
            currArch.lights.push_back(currLight);
        }
        
        // Iterating over billboards
        TiXmlElement* pBillb=pArchetypeNode->FirstChildElement( "structure" )->FirstChildElement("billboard");
        for( pBillb; pBillb; pBillb=pBillb->NextSiblingElement("billboard")) {
            Billboard currBillb;
            
            vector<int> siz = string_split_comma(pBillb->Attribute("size"));
            currBillb.size[0] = siz.at(0);
            currBillb.size[1] = siz.at(1);
            
            currBillb.texture = pBillb->Attribute("texture");
            
            currArch.billboards.push_back(currBillb);
        }
        
        currArch.smoothness = atof(pArchetypeNode->FirstChildElement("path")->Attribute("smoothness"));
        
        TiXmlElement* pPoint=pArchetypeNode->FirstChildElement( "path" )->FirstChildElement("point");
        for( pPoint; pPoint; pPoint=pPoint->NextSiblingElement("point")) {
            Pos currPos;
            vector<int> posn = string_split_comma(pPoint->Attribute("pos"));
            currPos.set(posn.at(0),posn.at(1),posn.at(2));
            
            currArch.path.push_back(currPos);
        }
        
        factory[currArch.name] = currArch;
    }
    
    // print things
    for ( std::map<string, Archetype>::const_iterator iter = factory.begin(); iter != factory.end(); ++iter ) {

        cout << iter->first << endl;
        
        cout << iter->second.name << endl;
        cout << iter->second.duration << endl;
        cout << iter->second.size[0] << " " << iter->second.size[1] << endl;
        cout << iter->second.health << endl;
        cout << iter->second.pointvalue << endl;
        cout << iter->second.damagenumber << endl;
        
        cout << "Damagedata:" << endl;
        for (vector<int>::const_iterator mes = iter->second.damagedata.begin(); mes != iter->second.damagedata.end(); mes++) {
            cout << "\t" << *mes << endl;
        }
        
        cout << "Meshes:" << endl;
        for (list<Mesh>::const_iterator mes = iter->second.meshes.begin(); mes != iter->second.meshes.end(); mes++) {
            cout << "\t" << mes->pos.X << " "
                         << mes->pos.X << " "
                         << mes->pos.Z << " "
                         << endl;
            cout << "\t" << mes->file << endl;
            cout << "\t" << mes->texture << endl;
            cout << "\t" << mes->scale << endl;
            cout << "-" << endl;
        }
        
        cout << "Lights:" << endl;
        for (list<Light>::const_iterator mes = iter->second.lights.begin(); mes != iter->second.lights.end(); mes++) {
            cout << "\t" << mes->pos.X << " "
                         << mes->pos.X << " "
                         << mes->pos.Z << " "
                         << endl;
            cout << "\t" << mes->colour[0] << " "
                         << mes->colour[1] << " "
                         << mes->colour[2] << " "
                         << endl;
            cout << "\t" << mes->range << endl;
            cout << "-" << endl;
        }
        
        cout << "Billboards:" << endl;
        for (list<Billboard>::const_iterator mes = iter->second.billboards.begin(); mes != iter->second.billboards.end(); mes++) {
            cout << "\t" << mes->pos.X << " "
                         << mes->pos.X << " "
                         << mes->pos.Z << " "
                         << endl;
            cout << "\t" << mes->texture << endl;
            cout << "\t" << mes->size[0] << " "
                         << mes->size[1] << " "
                         << endl;
            cout << "-" << endl;
        }
        cout << "Path:" << endl;
        cout << iter->second.smoothness << endl;
        
        for (vector<Pos>::const_iterator mes = iter->second.path.begin(); mes != iter->second.path.end(); mes++) {
            cout << "\t" << mes->X << " "
                         << mes->Y << " "
                         << mes->Z << " "
                         << endl;
        }
        
        cout << endl;
        cout << "---" << endl;
        cout << endl;

/*        float smoothness;
        vector<Pos> path; */
        
        
        /*cout << iter->second.smoothness << endl;
        for (vector<Pos>::const_iterator mes = iter->second.path.begin(); mes != iter->second.path.end(); mes++) {
//        for (list<Light>::const_iterator mes = iter->second.lights.begin(); mes != iter->second.lights.end(); mes++) {
            cout << mes->X << " " << mes->Y << " " << mes->Z << endl;
        }*/
    };

   return factory;
}
