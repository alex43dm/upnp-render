#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

//#include "tools.h"

#include <iostream>

#include "WebServer.h"
#include "Log.h"

static std::map<std::string, WebPage *> *pages;

typedef struct {
    off_t pos;
    const char *contents;
    size_t len;
} WebServerFile;
//---------------------------------------------------------------------------------------------------------------
WebServer::WebServer(Config *_cfg) {
    cfg = _cfg;
    pages = new std::map<std::string, WebPage *>();

    registerFile("upnp_description.xml",  "description.xml");
    registerFile("upnp_128x128.png",  "/upnp/128x128.png");
    registerFile("upnp_64x64.png",  "/upnp/64x64.png");
    registerFile("upnp_renderconnmgr_scpd.xml",  "/upnp/renderconnmgrSCPD.xml");
    registerFile("upnp_rendertransport_scpd.xml",  "/upnp/rendertransportSCPD.xml");
    registerFile("upnp_rendercontrol_scpd.xml",  "/upnp/rendercontrolSCPD.xml");
}

//---------------------------------------------------------------------------------------------------------------
void WebServer::loadPages(const char *dirname) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirname);
    if (!dir) {
        perror("diropen");
        exit(1);
    };

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG)
        {
            loadPages(entry->d_name);
        }
        //printf("%d - %s [%d] %d\n",entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
    };

    closedir(dir);
    /*
        CFileIterator fit;
        u8 bDir;
            while (const char* psFileName = fit.getFirstFile(dirname, bDir))
            {
                if (*psFileName == '.')
                    continue;

                if(bDir)
                {
                    loadPages(psFileName);
                    continue;
                }

                std::string url;
                if( strcmp(dirname,"web") != 0)
                {
                    url = dirname;
                    url += "/";
                }
                url  += psFileName;
                registerFile(url, psFileName);
            }*/
}
//---------------------------------------------------------------------------------------------------------------
WebServer::~WebServer() {
    for (std::map<std::string, WebPage *>::iterator it = pages->begin(); it != pages->end(); ++it) {
        delete(*it).second;
    }
    delete pages;
}
//---------------------------------------------------------------------------------------------------------------
bool WebServer::registerPage(std::string path, std::string contents,  std::string contentType) {
    WebPage *entry = new WebPage(path, contents, contentType);

    if (entry->content.length() > 0) {
        pages->insert(std::pair<const char *, WebPage *>(entry->fName.c_str(), entry));
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------------------------------------------
bool WebServer::strReplace(string *str, string whatReplace, string toReplace) {
    int start = -1;
    start = str->find(whatReplace, 0);
    if (start > -1) {
        str->replace(start, whatReplace.size(), toReplace);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------------------------------------------
bool WebServer::registerFile(std::string path,  std::string key) {
    WebPage *entry = new WebPage(path);

    if (key == "description.xml") {
        //port fix
        strReplace(&entry->content, "49152", cfg->port);
        strReplace(&entry->content, "MediaRender", cfg->Name);
        strReplace(&entry->content, "uuid:MediaRender-0000-0000-0000-000000000001", cfg->UDN);
    }


    if (entry->content.length() > 0) {
        pages->insert(std::pair<std::string, WebPage *>(key, entry));
        return true;
    }
    else
        delete entry;
    return false;
}
//---------------------------------------------------------------------------------------------------------------
bool WebServer::registerFile(std::string path) {
    WebPage *entry = new WebPage(path);
    if (entry->content.length() > 0) {
        pages->insert(std::pair<const char *, WebPage *>(entry->fName.c_str(), entry));
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------------------------------------------
WebPage *WebServer::getPage(const char *filename) {
    WebPage *p = (*pages)[filename];
    return p;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::get_info(const char *filename, struct File_Info *info) {
    WebPage *entry = (*pages)[filename];
    if (entry) {
        info->file_length = entry->content.length();
        info->last_modified = 0;
        info->is_directory = 0;
        info->is_readable = 1;
        info->content_type =  ixmlCloneDOMString(entry->getContentType());
        return 0;
    }
    Log::err("%s  failed: file name: %s", __FUNCTION__, filename);
    return 1;
}
//---------------------------------------------------------------------------------------------------------------
UpnpWebFileHandle WebServer::open(const char *filename, enum UpnpOpenFileMode mode) {
    WebServerFile *file = NULL;

    if (mode != UPNP_READ) {
        return NULL;
    }

    WebPage *entry = (*pages)[filename];
    if (entry) {
        file = (WebServerFile *)malloc(sizeof(WebServerFile));
        file->pos = 0;
        file->len = entry->content.length();
        file->contents = entry->content.c_str();
        return file;
    }
    Log::err("%s  failed: file name: %s", __FUNCTION__, filename);
    return NULL;
}
//---------------------------------------------------------------------------------------------------------------
static inline int minimum(int a, int b) {
    return (a < b) ? a : b;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::read(UpnpWebFileHandle fh, char *buf, size_t buflen) {
    WebServerFile *file = (WebServerFile *) fh;
    int len = -1;

    len = minimum(buflen, file->len - file->pos);
    memcpy(buf, file->contents + file->pos, len);

    if (len < 0) {
        Log::err("%s  failed", __FUNCTION__);
    }
    else {
        file->pos += len;
    }
    return len;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::write(UpnpWebFileHandle fh, char *buf, size_t buflen) {
    return -1;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::seek(UpnpWebFileHandle fh, off_t offset, int origin) {
    WebServerFile *file = (WebServerFile *) fh;
    off_t newpos = -1;
    int result = -1;

    switch (origin) {
        case SEEK_SET:
            newpos = offset;
            break;
        case SEEK_CUR:
            newpos = file->pos + offset;
            break;
        case SEEK_END:
            newpos = file->len + offset;
            break;
    }

    if (newpos < 0 || (unsigned)newpos > file->len) {
        Log::err("%s  failed", __FUNCTION__);
        goto out;
    }

    file->pos = newpos;
    result = 0;
out:
    return result;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::close(UpnpWebFileHandle fh) {
    WebServerFile *file = (WebServerFile *) fh;
    free(file);
    return 0;
}
//---------------------------------------------------------------------------------------------------------------
UpnpVirtualDirCallbacks *WebServer::callbacks(void) {
    static UpnpVirtualDirCallbacks vcallbacks = {
        WebServer::get_info,
        WebServer::open,
        WebServer::read,
        WebServer::write,
        WebServer::seek,
        WebServer::close
    };
    return &vcallbacks;
}
