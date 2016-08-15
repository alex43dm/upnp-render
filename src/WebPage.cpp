#include <map>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "WebPage.h"
#include "Config.h"

static std::map<ContentTypes, std::string> ContentTypesNames;

loadResourceFn *g_loadResourceFn = 0;

//------------------------------------------------------------------------------
bool loadFile(std::string &dst, const std::string &path) {

    if (g_loadResourceFn)
        return g_loadResourceFn(dst, path);


    int fd = open(path.c_str(), O_RDONLY);
    if (!fd) {
        return false;
    }

    struct stat st;
    fstat(fd, &st);

    char *buf = (char *)malloc(st.st_size);
    read(fd, buf, st.st_size);
    dst = std::string(buf);

    return true;
}

//------------------------------------------------------------------------------
WebPage::WebPage(const std::string &virtual_fname,
                 const std::string &_contents,
                 const std::string &content_type) {
    if (!ContentTypesNames.size()) {
        ContentTypesNames[png] = "image/png";
        ContentTypesNames[xml] = "text/xml";
        ContentTypesNames[unknown] = "";
    }
    fName = virtual_fname;
    content = _contents;
    contentType = getContentType(content_type);
}
//------------------------------------------------------------------------------
WebPage::WebPage(std::string &path,
                 const std::string &content_type) {
    std::string fpath(Config::Instance()->webServer);
    fpath += path.erase(0, 5);
    loadFile(content, fpath);
    fName = path;
    contentType = getContentType(content_type);
}
//------------------------------------------------------------------------------
WebPage::WebPage(const std::string &path, const std::string &vpath) {
    setContentType(path);
    std::string fpath(Config::Instance()->webServer);
    fpath += path;
    loadFile(content, fpath);
    fName = vpath;
}
//------------------------------------------------------------------------------
ContentTypes WebPage::getContentType(const std::string &t) {
    for (std::map<ContentTypes, std::string>::iterator
         it = ContentTypesNames.begin(); it != ContentTypesNames.end(); ++it) {
        if ((*it).second == t)
            return (*it).first;
    }
    return unknown;
}
//------------------------------------------------------------------------------
void WebPage::setContentType(const std::string &t) {
    if (t.find(".xml") != std::string::npos) {
        contentType = xml;
    }
    if (t.find(".png") != std::string::npos) {
        contentType = png;
    }
}
//------------------------------------------------------------------------------
const char *WebPage::getContentType() {
    return ContentTypesNames[contentType].c_str();
}
//------------------------------------------------------------------------------
WebPage::~WebPage() {
}
