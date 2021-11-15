#include <string>
#include <utf8cpp/utf8.h>

int utf8_strlen(const std::string& str)
{
    int c = 0;
    int i = 0;
    int ix = 0;
    int q = 0;
    int q1 = 0;

    q1 = utf8::distance(str.begin(), str.end());


    for(q = 0, i = 0, ix = str.length(); i < ix; i++, q++)
    {
        c = (unsigned char)str[i];
        if(c >= 0 && c <= 127)
        {
            i += 0;
        }
        else if((c & 0xE0) == 0xC0)
        {
            i += 1;
        }
        else if((c & 0xF0) == 0xE0)
        {
            i += 2;
        }
        else if((c & 0xF8) == 0xF0)
        {
            i += 3;
        }
        else
        {
            return 0; //invalid utf8
        }
    } 
    return q;
}
