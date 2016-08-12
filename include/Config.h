#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include <upnp/ixml.h>

class Config {
    public:
        std::string port;
        std::string DocumentRoot;
        std::string UDN;
        std::string AdvertisementExpTimeOut;
        std::string Name;
        static const std::string mime;
        static const std::string equalizer;
        std::string volume;
        bool m_bError;

        Config(const std::string &title, const unsigned char *pMacAddr);
        bool CreateFile(std::string fName);
        unsigned short getPort();

        virtual ~Config();
    protected:
    private:
        IXML_Node *setVal(std::string name, std::string val, IXML_Document *doc);
        std::string fileName;
};

#endif // CONFIG_H
