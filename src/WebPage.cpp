#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "WebPage.h"
#include "Config.h"

//------------------------------------------------------------------------------
WebPage::WebPage(const std::string &path, const std::string &vpath) {
    setContentType(path);
    std::string fpath(Config::Instance()->webServer);
    fpath += path;
    loadFile(content, fpath);
    fName = vpath;
}
//------------------------------------------------------------------------------
WebPage::~WebPage() {
}
//------------------------------------------------------------------------------
bool WebPage::loadFile(std::string &dst, const std::string &path) {
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
    switch(contentType)
    {
        case ContentTypes::png:
            return "image/png";
        case ContentTypes::xml:
            return "text/xml";
        case ContentTypes::unknown:
        default:
            return "unknow";
    }
}
