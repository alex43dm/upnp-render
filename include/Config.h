#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include <upnp/ixml.h>

class Config {
    public:
        std::string ifa;
        std::string ip;
        std::string port;
        std::string DocumentRoot;
        std::string UDN;
        std::string AdvertisementExpTimeOut;
        std::string Name;
        std::string mime;
        std::string volume;
        std::string webServer;

        static Config* Instance();
        bool LoadConfig(const std::string fName);

        bool CreateFile(std::string fName);
        unsigned short getPort();
        std::string url(){return "http://"+ip+":"+port;}


        virtual ~Config();
    protected:
        bool Load();
    private:
        bool mIsInited;
        static Config* mInstance;
        Config();

        bool getNet();
        void setName();
        bool mimeLoad();
        std::string xmlGetChildElementValue(IXML_Element* parent,
                                const std::string &tagName );
        std::string xmlGetChildElement(IXML_Element* parent, const char* tagName);
        std::string xmlGetChildElementAttr(IXML_Element* parent,
                               const char* tagName, const char* attrName);
        IXML_Node *setVal(std::string name, std::string val, IXML_Document *doc);
        std::string fileName;
        std::string mac;
        std::string mimeFile;
};
#endif // CONFIG_H
