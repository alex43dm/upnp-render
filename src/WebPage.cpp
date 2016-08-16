#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "WebPage.h"
#include "Config.h"
#include "Log.h"

//------------------------------------------------------------------------------
WebPage::WebPage(const std::string &path) {
    len = 0;
    loadFile(content, Config::Instance()->webServer + path);
    setContentType(path);
}
//------------------------------------------------------------------------------
WebPage::~WebPage() {
}
//------------------------------------------------------------------------------
bool WebPage::loadFile(std::string &dst, const std::string &path) {
    int fd = open(path.c_str(), O_RDONLY);
    if (!fd) {
            Log::err("%s: error open file: %s", __func__, path.c_str());
        return false;
    }

    struct stat st;
    fstat(fd, &st);

    len = st.st_size;

    char *buf = (char *)malloc(len);
    read(fd, buf, len);
    dst = std::string(buf);

    close(fd);

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
