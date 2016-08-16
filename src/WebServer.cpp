#include <map>

#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "Config.h"
#include "WebServer.h"
#include "Log.h"

static std::map<std::string, WebPage *> *pages;

typedef struct
{
    off_t pos;
    const char *contents;
    size_t len;
} WebServerFile;

//------------------------------------------------------------------------------
WebServer::WebServer()
{
    pages = new std::map<std::string, WebPage *>();

    registerFile("/description.xml", "/description.xml");
    registerFile("/upnp/128x128.png", "/upnp/128x128.png");
    registerFile("/upnp/64x64.png", "/upnp/64x64.png");
    registerFile("/upnp/renderconnmgrSCPD.xml", "/upnp/renderconnmgrSCPD.xml");
    registerFile("/upnp/rendertransportSCPD.xml", "/upnp/rendertransportSCPD.xml");
    registerFile("/upnp/rendercontrolSCPD.xml", "/upnp/rendercontrolSCPD.xml");
}
//------------------------------------------------------------------------------
WebServer::~WebServer()
{
    for (std::map<std::string, WebPage *>::iterator it = pages->begin();
            it != pages->end(); ++it)
    {
        delete(*it).second;
    }
    delete pages;
}
//------------------------------------------------------------------------------
bool WebServer::strReplace(IXML_Document *doc, IXML_Element* rootElement,
                           const std::string &whatReplace,
                           const std::string &toReplace)
{
    int ret;

    IXML_NodeList* nodeList =
        ixmlElement_getElementsByTagName(rootElement, whatReplace.c_str());
    if ( !nodeList )
    {
        Log::err("%s: Does not found UDN section", __func__);
        return false;
    }


    IXML_Node* oldNode = ixmlNodeList_item( nodeList, 0 );
    ixmlNodeList_free( nodeList );
    if ( !oldNode )
    {
        Log::err("%s: Does not found UDN node", __func__);
        return false;
    }

    IXML_Element *newElement = ixmlDocument_createElement(doc, whatReplace.c_str());
    IXML_Element *newVal = ixmlDocument_createElement(doc, toReplace.c_str());
    (&newVal->n)->nodeType = eTEXT_NODE;
    ixmlNode_setNodeValue(&newVal->n, toReplace.c_str());
    ixmlNode_appendChild( &newElement->n, &newVal->n );

    ret = ixmlNode_replaceChild(oldNode->parentNode, &newElement->n, oldNode, NULL);
    if (IXML_SUCCESS != ret)
    {
        Log::err("%s: Cann't replace UDN node: %d", __func__, ret);
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool WebServer::registerFile(const std::string &path, const std::string &vPath)
{
    WebPage *entry = new WebPage(path);

    if (path == "/description.xml")
    {
        IXML_Document *doc = 0;
        int ret = ixmlParseBufferEx(entry->content.c_str(), &doc);
        if( ret !=  IXML_SUCCESS )
        {
            Log::err("%s: Error parse: %s : %d", __func__, path.c_str(), ret);
            printf("parse: %lu\n %s\n", entry->len, entry->content.c_str());
            exit(1);
        }

        IXML_NodeList* rootList = ixmlDocument_getElementsByTagName(doc, "root");
        if ( !rootList )
        {
            Log::err("%s: Does not found root section", __func__);
            return false;
        }

        IXML_Node *rootNode = ixmlNodeList_item(rootList, 0);
        IXML_Element* rootElement = ( IXML_Element*)rootNode;

        strReplace(doc, rootElement, "UDN", Config::Instance()->UDN);
        strReplace(doc, rootElement, "URLBase", Config::Instance()->url());

        entry->content = ixmlDocumenttoString(doc);
        entry->len = entry->content.size();
    }


    if (entry->len > 0)
    {
        pages->insert(std::pair<std::string, WebPage *>(vPath, entry));
        return true;
    }
    else
    {
        Log::err("%s: error insert page: %s", path.c_str());
        exit(1);
    }

    return false;
}
//---------------------------------------------------------------------------------------------------------------
WebPage *WebServer::getPage(const char *vPath)
{
    WebPage *p = (*pages)[vPath];
    if (!p)
    {
        Log::err("%s: path: %s not found",__func__,vPath);
    }
    return p;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::get_info(const char *filename, struct File_Info *info)
{
    WebPage *entry = (*pages)[filename];
    if (entry)
    {
        info->file_length = entry->len;
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
UpnpWebFileHandle WebServer::open(const char *filename, enum UpnpOpenFileMode mode)
{
    WebServerFile *file = NULL;

    if (mode != UPNP_READ)
    {
        return NULL;
    }

    WebPage *entry = (*pages)[filename];
    if (entry)
    {
        file = (WebServerFile *)malloc(sizeof(WebServerFile));
        file->pos = 0;
        file->len = entry->len;
        file->contents = entry->content.c_str();
        return file;
    }
    Log::err("%s  failed: file name: %s", __FUNCTION__, filename);
    return NULL;
}
//---------------------------------------------------------------------------------------------------------------
static inline int minimum(int a, int b)
{
    return (a < b) ? a : b;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::read(UpnpWebFileHandle fh, char *buf, size_t buflen)
{
    WebServerFile *file = (WebServerFile *) fh;
    int len = -1;

    len = minimum(buflen, file->len - file->pos);
    memcpy(buf, file->contents + file->pos, len);

    if (len < 0)
    {
        Log::err("%s  failed", __FUNCTION__);
    }
    else
    {
        file->pos += len;
    }
    return len;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::write(UpnpWebFileHandle fh, char *buf, size_t buflen)
{
    return -1;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::seek(UpnpWebFileHandle fh, off_t offset, int origin)
{
    WebServerFile *file = (WebServerFile *) fh;
    off_t newpos = -1;
    int result = -1;

    switch (origin)
    {
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

    if (newpos < 0 || (unsigned)newpos > file->len)
    {
        Log::err("%s  failed", __FUNCTION__);
        goto out;
    }

    file->pos = newpos;
    result = 0;
out:
    return result;
}
//---------------------------------------------------------------------------------------------------------------
int WebServer::close(UpnpWebFileHandle fh)
{
    WebServerFile *file = (WebServerFile *) fh;
    free(file);
    return 0;
}
//---------------------------------------------------------------------------------------------------------------
UpnpVirtualDirCallbacks *WebServer::callbacks(void)
{
    static UpnpVirtualDirCallbacks vcallbacks =
    {
        WebServer::get_info,
        WebServer::open,
        WebServer::read,
        WebServer::write,
        WebServer::seek,
        WebServer::close
    };
    return &vcallbacks;
}
