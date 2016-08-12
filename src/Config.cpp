#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <upnp/ixml.h>
#include <upnp/upnp.h>

#include "Config.h"

#define UUID "uuid:Render-0000-0000-0000-00000000000"

Config::Config(const std::string &title,
                const unsigned char *pMacAddr) : m_bError(false) {
    if (pMacAddr) {
        char *ptr = (char *)malloc(40);
        sprintf(ptr, "uuid:Render-%02X%02X%02X%02X%02X%02X",
                pMacAddr[3], pMacAddr[2],
                pMacAddr[0], pMacAddr[1],
                pMacAddr[5], pMacAddr[4]);
        UDN = std::string(ptr);
    }
    else
        UDN = UUID;

    AdvertisementExpTimeOut = "3600";
    DocumentRoot = "./upnp-config";
    Name = title;
    volume = "70";
//    port = "49152";
    port = std::to_string(UpnpGetServerPort());

}
//---------------------------------------------------------------------------------------------------------------
unsigned short Config::getPort() {
    return atoi(port.c_str());
}

//---------------------------------------------------------------------------------------------------------------
IXML_Node *Config::setVal(std::string name, std::string val, IXML_Document *doc) {
    IXML_Element *el = ixmlDocument_createElement(doc, name.c_str());
    IXML_Element *v = ixmlDocument_createElement(doc, val.c_str());
    (&v->n)->nodeType = eTEXT_NODE;
    ixmlNode_setNodeValue(&v->n, val.c_str());
    ixmlNode_appendChild(&el->n, &v->n);
    return &el->n;
}
//---------------------------------------------------------------------------------------------------------------
bool Config::CreateFile(std::string fName) {
    IXML_Document *doc;
    doc = ixmlDocument_createDocument();

    IXML_Element *root = ixmlDocument_createElement(doc, "root");
    ixmlNode_appendChild(&root->n, setVal("port",  port, doc));
    ixmlNode_appendChild(&root->n, setVal("UDN",  UDN, doc));
    ixmlNode_appendChild(&root->n, setVal("AdvertisementExpTimeOut",  AdvertisementExpTimeOut, doc));
    ixmlNode_appendChild(&root->n, setVal("Name",  Name, doc));
    ixmlNode_appendChild(&root->n, setVal("mime",  mime, doc));
    ixmlNode_appendChild(&root->n, setVal("equalizer",  equalizer, doc));

    //IXML_Element *devroot = ixmlDocument_createElement(doc, "devroot");

    //ixmlNode_appendChild( &root->n, &devroot->n);
    ixmlNode_appendChild(&doc->n, &root->n);

    std::ofstream configFile(fName.c_str());
    configFile << ixmlDocumenttoString(doc);
    configFile.close();
    return true;
}
//---------------------------------------------------------------------------------------------------------------
Config::~Config() {
    //dtor
}

const std::string Config::mime(
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG;DLNA.ORG_OP=01;DLNA.ORG_FLAGS=00d00000000000000000000000000000,\n"
    "http-get:*:image/png:DLNA.ORG_PN=PNG_LRG;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=00d00000000000000000000000000000,\n"
    "http-get:*:image/gif:DLNA.ORG_PN=GIF_LRG;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=00d00000000000000000000000000000,\n"
    "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=01;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMABASE;DLNA.ORG_OP=11;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:video/mp4:DLNA.ORG_PN=AVC_MP4_BL_CIF15_AAC_520;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_EU_ISO;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=8d700000000000000000000000000000,\n"
    "http-get:*:video/avi:DLNA.ORG_PN=AVI;DLNA.ORG_OP=01;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:video/3gpp:DLNA.ORG_PN=AMR_3GPP;DLNA.ORG_OP=11;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:video/quicktime:DLNA.ORG_PN=QUICKTIME;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_BASE;DLNA.ORG_OP=11;DLNA.ORG_FLAGS=01700000000000000000000000000000,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_NA_ISO;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=8d700000000000000000000000000000,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_KO_ISO;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=8d700000000000000000000000000000,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_JP_ISO;DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=8d700000000000000000000000000000,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_PRO,\n"
    "http-get:*:video/x-ms-asf:DLNA.ORG_PN=MPEG4_P2_ASF_SP_G726,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_FULL,\n"
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_BASE,\n"
    "http-get:*:audio/L16;rate=44100;channels=1:DLNA.ORG_PN=LPCM,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_NTSC,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHIGH_PRO,\n"
    "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM,\n"
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM,\n"
    "http-get:*:video/x-ms-asf:DLNA.ORG_PN=VC1_ASF_AP_L1_WMA,\n"
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMDRM_WMABASE,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHIGH_FULL,\n"
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMAFULL,\n"
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMABASE,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVSPLL_BASE,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_NTSC_XAC3,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMDRM_WMVSPLL_BASE,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVSPML_BASE,\n"
    "http-get:*:video/x-ms-asf:DLNA.ORG_PN=MPEG4_P2_ASF_ASP_L5_SO_G726,\n"
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG,\n"
    "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL_XAC3,\n"
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMAPRO,\n"
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG1,\n"
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_TN,\n"
    "http-get:*:video/x-ms-asf:DLNA.ORG_PN=MPEG4_P2_ASF_ASP_L4_SO_G726,\n"
    "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM,\n"
    "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3X,\n"
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVSPML_MP3,\n"
    "http-get:*:video/x-ms-wmv:*,\n"
    "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_OP=01,\n"
    "http-get:*:audio/mpeg:DLNA.ORG_PN=MP2;DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMABASE;DLNA.ORG_OP=01,\n"
    "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO;DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-flac:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-aiff:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-ogg:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/wav:DLNA.ORG_PN=WAV;DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-ape:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-wavpack:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-musepack:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/L16;rate=44100;channels=1:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=1,\n"
    "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=1,\n"
    "http-get:*:audio/L16;rate=48000;channels=1:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=1,\n"
    "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM;DLNA.ORG_OP=01;DLNA.ORG_CI=1,\n"
    "http-get:*:audio/L24:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/lpcm:DLNA.ORG_OP=01,\n"
    "http-get:*:audio/x-mpegurl,\n"
    "http-get:*:audio/x-mpeg-url,\n"
    "http-get:*:audio/x-x-winamp-playlist,\n"
    "http-get:*:audio/scpls,\n"
    "http-get:*:audio/x-scpls"
);

const std::string Config::equalizer(
    "	<equalizer>"
    "		<gain freq=\"50\">6.0</gain>"
    "		<gain freq=\"100\">6.0</gain>"
    "		<gain freq=\"156\">6.0</gain>"
    "		<gain freq=\"220\">4.0</gain>"
    "		<gain freq=\"311\">3.0</gain>"
    "		<gain freq=\"440\">2.0</gain>"
    "		<gain freq=\"622\">1.0</gain>"
    "		<gain freq=\"880\">-2.0</gain>"
    "		<gain freq=\"1250\">-2.0</gain>"
    "		<gain freq=\"1750\">-3.0</gain>"
    "		<gain freq=\"2500\">-5.0</gain>"
    "		<gain freq=\"3500\">-2.0</gain>"
    "		<gain freq=\"5000\">-1.0</gain>"
    "		<gain freq=\"10000\">3.0</gain>"
    "		<gain freq=\"20000\">4.0</gain>"
    "		<gain freq=\"25000\">6.0</gain>"
    "	</equalizer>"
);
