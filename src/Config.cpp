#include <iostream>
#include <fstream>
#include <regex>

#include <stdio.h>
#include <stdlib.h>
#include <upnp/ixml.h>
#include <upnp/upnp.h>
#include <string.h>

#include <ifaddrs.h>
#include <netpacket/packet.h>

#include <sys/utsname.h>

#include "Config.h"

#define UUID "uuid:Render-0000-0000-0000-%s"

Config* Config::mInstance = NULL;

//------------------------------------------------------------------------------
Config* Config::Instance()
{
    if (!mInstance)
        mInstance = new Config();

    return mInstance;
}
//------------------------------------------------------------------------------
Config::Config()
{
    mIsInited = false;
}
//------------------------------------------------------------------------------
Config::~Config()
{
    //dtor
}
//------------------------------------------------------------------------------
bool Config::LoadConfig(const std::string fName)
{
    fileName = fName;
    return Load();
}
//------------------------------------------------------------------------------
bool Config::Load()
{
    IXML_Document *doc = ixmlLoadDocument(fileName.c_str());
    if( !doc  )
    {
        std::cerr<<"does not found config file: "<<fileName<<std::endl;
        return false;
    }

    IXML_NodeList* rootList = ixmlDocument_getElementsByTagName( doc, "root" );
    if ( !rootList )
    {
        std::cerr<<"does not found root section in file: "<<fileName<<std::endl;
    }

    IXML_Element* rootElement = ( IXML_Element* )ixmlNodeList_item( rootList, 0);

    ifa = xmlGetChildElementValue( rootElement, "ifa");
    port = xmlGetChildElementValue( rootElement, "port");
    UDN = xmlGetChildElementValue( rootElement, "UDN");
    AdvertisementExpTimeOut =
        xmlGetChildElementValue( rootElement, "AdvertisementExpTimeOut");
    DocumentRoot = xmlGetChildElementValue( rootElement, "DocumentRoot");
    mimeFile = xmlGetChildElementValue( rootElement, "mime");

    volume = "70";

    setName();
    getNet();
    mimeLoad();

    mIsInited = true;
    return mIsInited;
}

//------------------------------------------------------------------------------
std::string
Config::xmlGetChildElementValue(IXML_Element* parent,
                                const std::string &tagName )
{
    if ( !parent ) return "";
    if ( tagName.empty() ) return "";


    IXML_NodeList* nodeList =
        ixmlElement_getElementsByTagName( parent, tagName.c_str());
    if ( !nodeList ) return "";

    IXML_Node* element = ixmlNodeList_item( nodeList, 0 );
    ixmlNodeList_free( nodeList );
    if ( !element ) return "";

    IXML_Node* textNode = ixmlNode_getFirstChild( element );
    if ( !textNode ) return "";
    const char* ret = ixmlNode_getNodeValue( textNode );
    if( ret )
    {
        return ret;
    }

    return "";
}
//------------------------------------------------------------------------------
std::string
Config::xmlGetChildElement(IXML_Element* parent, const char* tagName )
{
    if ( !parent ) return "";
    if ( !tagName ) return "";

    char* s = strdup( tagName );
    IXML_NodeList* nodeList = ixmlElement_getElementsByTagName( parent, s );
    free( s );

    IXML_Node* element = ixmlNodeList_item( nodeList, 0 );
    ixmlNodeList_free( nodeList );
    if ( !element ) return "";

    const char* ret = ixmlNodetoString( element );
    if( ret )
    {
        return ret;
    }

    return "";
}
///------------------------------------------------------------------------------
std::string
Config::xmlGetChildElementAttr(IXML_Element* parent,
                               const char* tagName, const char* attrName )
{
    if ( !parent ) return "";
    if ( !tagName ) return "";
    if ( !attrName ) return "";

    char* s = strdup( tagName );
    IXML_NodeList* nodeList = ixmlElement_getElementsByTagName( parent, s );
    free( s );

    IXML_Node* element = ixmlNodeList_item( nodeList, 0 );
    ixmlNodeList_free( nodeList );
    if ( !element ) return "";

    const char* ret =
        ixmlElement_getAttribute((IXML_Element* )element,  attrName);
    if( ret )
    {
        return ret;
    }

    return "";
}
//------------------------------------------------------------------------------
IXML_Node* Config::setVal(std::string name, std::string val, IXML_Document* doc)
{
    IXML_Element *el = ixmlDocument_createElement(doc, name.c_str());
    IXML_Element *v = ixmlDocument_createElement(doc, val.c_str());
    (&v->n)->nodeType = eTEXT_NODE;
    ixmlNode_setNodeValue(&v->n,val.c_str());
    ixmlNode_appendChild( &el->n, &v->n );
    return &el->n;
}
//------------------------------------------------------------------------------
void Config::setName()
{
    struct utsname unameVal;

    if(uname(&unameVal) == 0)
    {
        Name = unameVal.nodename;
    }
    else
        Name = "Linux";
}
//------------------------------------------------------------------------------
bool Config::getNet()
{
    struct ifaddrs *addrList=NULL;
    struct ifaddrs *ifa = NULL;
    struct sockaddr_in *sa;
    char h[13];

    std::regex e(Config::ifa);
    memset(h,0,sizeof(h));

    if (getifaddrs(&addrList) == -1)
    {
        perror("getifaddrs");
        return false;
    }
    else
    {
        for (ifa = addrList; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (!ifa->ifa_addr)
                continue;

            if (ifa->ifa_addr->sa_family == AF_PACKET
                    && std::regex_match(ifa->ifa_name,e))
            {
                struct sockaddr_ll *m = (struct sockaddr_ll*)ifa->ifa_addr;

                sprintf(h, "%02X%02X%02X%02X%02X%02X",
                        m->sll_addr[3], m->sll_addr[1],
                        m->sll_addr[2], m->sll_addr[4],
                        m->sll_addr[5], m->sll_addr[0]);
                mac = h;
                continue;
            }

            if (ifa->ifa_addr->sa_family == AF_INET
                    && std::regex_match(ifa->ifa_name,e))
            {
                sa = (struct sockaddr_in *) ifa->ifa_addr;
                ip = inet_ntoa(sa->sin_addr);
            }
        }
        freeifaddrs(addrList);
    }
    return true;
}
//------------------------------------------------------------------------------
unsigned short Config::getPort()
{
    return atoi(port.c_str());
}
//------------------------------------------------------------------------------
bool Config::CreateFile(std::string fName)
{
    IXML_Document *doc;
    doc = ixmlDocument_createDocument();

    IXML_Element *root = ixmlDocument_createElement(doc, "root");
    ixmlNode_appendChild(&root->n, setVal("port",  port, doc));
    ixmlNode_appendChild(&root->n, setVal("UDN",  UDN, doc));
    ixmlNode_appendChild(&root->n, setVal("AdvertisementExpTimeOut",
                                          AdvertisementExpTimeOut, doc));
    ixmlNode_appendChild(&root->n, setVal("Name",  Name, doc));
    ixmlNode_appendChild(&root->n, setVal("mime",  mime, doc));
//    ixmlNode_appendChild(&root->n, setVal("equalizer",  equalizer, doc));

    //IXML_Element *devroot = ixmlDocument_createElement(doc, "devroot");

    //ixmlNode_appendChild( &root->n, &devroot->n);
    ixmlNode_appendChild(&doc->n, &root->n);

    std::ofstream configFile(fName.c_str());
    configFile << ixmlDocumenttoString(doc);
    configFile.close();
    return true;
}
//------------------------------------------------------------------------------
bool Config::mimeLoad()
{
    unsigned long i, length;

    IXML_Document *doc = ixmlLoadDocument(mimeFile.c_str());
    if( !doc  )
    {
        std::cerr<<"does not found config file: "<<mimeFile<<std::endl;
        return false;
    }

    IXML_NodeList* rootList = ixmlDocument_getElementsByTagName( doc, "root" );
    if ( !rootList )
    {
        std::cerr<<"does not found root section in file: "<<mimeFile<<std::endl;
    }

    IXML_Element* rootElement = ( IXML_Element* )ixmlNodeList_item( rootList, 0);

    IXML_NodeList* nodeList =
        ixmlElement_getElementsByTagName( rootElement, "url");
    if ( !nodeList ) return false;
    length = ixmlNodeList_length(nodeList);
    for(i = 0; i < length; i++)
    {
        IXML_Node* element = ixmlNodeList_item( nodeList, i );
        if ( !element ) continue;

        IXML_Node* textNode = ixmlNode_getFirstChild( element );
        if ( !textNode ) continue;
        const char* ret = ixmlNode_getNodeValue( textNode );
        if( ret )
        {
            mime += ret;
            if (i < length-1)
                mime += ",\n";
            else
                mime += "\n";

            free((void*)ret);
        }
    }

    ixmlNodeList_free( nodeList );

    //mime += "\n";

    std::cout<<"Mime: "<<mime<<std::endl;

    return true;
}
