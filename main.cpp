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
    int id;
    int projectID;
    std::string name;
    std::string geo[2];
    std::string industries[4];
};

struct fellow {
    int id;
    std::string name;
    std::string geo[5];
    std::string openGeo;
    std::string industries[4];
    std::string openInd;
};

void populateProjectList(const json& companyJson, std::vector<project>& ProjectList) {
    for (int i = 0; i < int(companyJson.size()); i++) {
        project p;
        p.id = companyJson[i].at("#").get<int>();
        p.projectID = companyJson[i].at("ProjectID").get<int>();
        p.name = companyJson[i].at("Company").get<std::string>();
        p.geo[0] = companyJson[i].at("Region").get<std::string>();
        p.geo[1] = companyJson[i].at("Country").get<std::string>();
        p.geo[2] = companyJson[i].at("City").get<std::string>();
        for (int j = 0; j < 4; j++) {
            p.industries[j] = companyJson[i].at("Industry"+std::to_string(j+1)).get<std::string>();
        }
        ProjectList.push_back(p);
    }
}

void populateFellowList(const json& proJson, std::vector<fellow>& FellowList) {
    for (int i = 0; i < int(proJson.size()); i++) {
        fellow f;
        f.id = proJson[i].at("#").get<int>();
        f.name = proJson[i].at("Name").get<std::string>();
        f.openGeo = proJson[i].at("OPENGEO").get<std::string>();
        f.openInd = proJson[i].at("OPENIND").get<std::string>();
        for (int j = 0; j <= 5; j++) {
            f.geo[j] = proJson[i].at("Geo"+std::to_string(j+1)).get<std::string>();
        }
        for (int j = 0; j <= 4; j++) {
            f.industries[j] = proJson[i].at("Industry"+std::to_string(j+1)).get<std::string>();
        }
        FellowList.push_back(f);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::ifstream company("/Users/joshpayne1/via-console/company.json");
    std::ifstream pro("/Users/joshpayne1/via-console/pro.json");
    company >> companyJson;
    pro >> proJson;
    std::vector<fellow> FellowList;
    std::vector<project> ProjectList;
    std::cout << "a" << std::endl;
    populateFellowList(proJson, FellowList);
    populateProjectList(companyJson, ProjectList);

    std::cout << FellowList[2].name << std::endl;

    return a.exec();
}
