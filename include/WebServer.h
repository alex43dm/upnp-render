#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>

#include <upnp/upnp.h>

#include "WebPage.h"

class WebServer {
    public:
        WebServer();
        virtual ~WebServer();
        bool registerPage(std::string path, std::string contents,  std::string contentType);
        bool registerFile(const std::string &path, const std::string &vPath);

        static int get_info(const char *filename, struct File_Info *info);
        static UpnpWebFileHandle open(const char *filename, enum UpnpOpenFileMode mode);
        static int read(UpnpWebFileHandle fh, char *buf, size_t buflen);
        static int write(UpnpWebFileHandle fh, char *buf, size_t buflen);
        static int seek(UpnpWebFileHandle fh, off_t offset, int origin);
        static int close(UpnpWebFileHandle fh);
        static UpnpVirtualDirCallbacks *callbacks(void);
        WebPage *getPage(const char *filename);

    protected:
    private:
        std::string path;
        bool registerFile(std::string path);
        void loadPages(const char *dirname);
        bool strReplace(IXML_Document *doc, IXML_Element* rootElement,
                           std::string whatReplace,
                           std::string toReplace);
};

#endif // WEBSERVER_H
