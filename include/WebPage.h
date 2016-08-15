#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <string>

enum ContentTypes {
    png,
    xml,
    unknown
};

typedef bool loadResourceFn(std::string &dst, const std::string &path);
extern loadResourceFn *g_loadResourceFn;

class WebPage {
    public:
        std::string fName;
        std::string content;
        ContentTypes contentType;

        WebPage(const std::string &virtual_fname,
                const std::string &_contents,
                const std::string &content_type);
        WebPage(std::string &path,
                const std::string &content_type);
        WebPage(const std::string &path,
                const std::string &vPath);
        virtual ~WebPage();

        const char *getContentType();

    protected:
    private:
        ContentTypes getContentType(const std::string &t);
        void setContentType(const std::string &t);
};

#endif // WEBPAGE_H
