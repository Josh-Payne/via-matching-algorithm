#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <streambuf>
#include <string>

using json = nlohmann::json;

json companyJson;
json proJson;

struct project {
    std::string company;
    std::string name;
    int start;
    int end;
    std::string geo;
    std::string industries[4];
};

struct fellow {
    std::string id;
    std::string name;
    std::string start;
    std::string end;
    std::string geo[5];
    std::string ind1;
    std::string ind2;
    std::string ind3;
    std::string industries[5];
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::ifstream company("/Users/joshpayne1/via-console/company.json");
    std::ifstream pro("/Users/joshpayne1/via-console/pro.json");
    company >> companyJson;
    pro >> proJson;


    std::cout << proJson["id"][5] << std::endl;


    return a.exec();
}
