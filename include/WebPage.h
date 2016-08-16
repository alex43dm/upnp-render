#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <string>

enum ContentTypes {
    png,
    xml,
    unknown
};

class WebPage {
    public:
        std::string content;
        ContentTypes contentType;
        size_t len;

        WebPage(const std::string &path);
        virtual ~WebPage();

        const char *getContentType();

    protected:
    private:
        void setContentType(const std::string &t);
        bool loadFile(std::string &dst, const std::string &path);
};

#endif // WEBPAGE_H
