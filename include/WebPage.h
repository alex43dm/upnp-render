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

        WebPage(std::string virtual_fname,  std::string _contents,  std::string content_type);
        WebPage(std::string path,  std::string content_type);
        WebPage(std::string path);
        virtual ~WebPage();

        const char *getContentType();

    protected:
    private:
        ContentTypes getContentType(std::string t);
        void setContentType(std::string t);
};

#endif // WEBPAGE_H
