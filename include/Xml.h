#ifndef XML_H
#define XML_H

typedef enum {
    DATATYPE_STRING,
    DATATYPE_BOOLEAN,
    DATATYPE_I2,
    DATATYPE_I4,
    DATATYPE_UI2,
    DATATYPE_UI4,
    DATATYPE_UNKNOWN,
    DATATYPE_COUNT
} paramDataType;



class Xml {
    public:
        Xml();
        virtual ~Xml();
        static void escape_real(const char *str, char *target, int *length,  int attribute);
        static char *escape(const char *str, int attribute);
    protected:
    private:
};

#endif // XML_H
