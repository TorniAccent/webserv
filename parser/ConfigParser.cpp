#include "ConfigParser.hpp"

static std::string noComment(std::fstream fin)
{
    std::string s;
    for (; fin >> s, s == "#"; fin.get() != '\n');
    return s;
}

static void throwIn(std::fstream fin, const std::string &ref)
{
    std::string s;
    s = noComment(fin);
    if (s != ref)
        throw "No \"" + ref + "\" identifier\n";
}

Config::Config(char *config)
{
    std::fstream fin;
    fin.open(config, std::fstream::in);
    if (fin.is_open() == false)
        throw "File did not open\n";

    // for (std::vector<Server>::iterator it = server.begin(); ; it++)
    for (; fin.eof(); )
    {
        Server elem;
        throwIn(fin, "server");
        throwIn(fin, "{");
        throwIn(fin, "host");
        fin >> elem.host;
        throwIn(fin, "port");
        fin >> elem.port;
        for (std::string location, key; key != "}"; )
        {
            if (key != "location")
                throwIn(fin, "location");
            fin >> location;
            Location l;
            for (std::string val; ; )
            {
                key = noComment(fin);
                if (key == "location" || key == "}")
                    break;
                fin >> val;
                l.method.insert(key, val);
            }
            elem.location.insert(location, l);
        }
        this->server.push_back(elem);
    }
}