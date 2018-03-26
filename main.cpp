#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <json.hpp>
#include <streambuf>
#include <string>

using json = nlohmann::json;

json companyJson;
json proJson;

static int LOCATION_MATCH = 5;
static int INDUSTRY_MATCH1 = 3;
static int INDUSTRY_MATCH2 = 1;
static int INDUSTRY_MATCH3 = 1;
static int FIRST_MATCH = 5;

std::vector<std::vector<std::pair<std::pair<int, int>, int>>> matches; // this is the grid. If a scorePair is a pair of ([fellow to project match] and [score for that matching]), it's an array of scorePair arrays.

struct project {
    int id;
    int projectID;
    std::string name;
    std::string geo[2];
    std::string industries[4];
    int candidates = 2;
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

bool locationMatches(fellow f, project p) {
    for (std::string lf : f.geo) {
        for (std::string lp : p.geo) {
            if (!lf.compare(lp)) return true;
        }
    }
    return false;
}

int industryMatches(fellow f, project p) {
    int counter = 0;
    for (std::string lf : f.industries) {
        for (std::string lp : p.industries) {
            if (lf.compare(lp)==0&&lp.compare("")!=0) {
                counter++;
            }
        }
    }
    return counter;
}

void createMatches(std::vector<fellow>& FellowList, std::vector<project>& ProjectList) {
    for (fellow f : FellowList) {
        std::vector<std::pair<std::pair<int, int>,int>> projectMatchesPerFellow;
        for (project p : ProjectList) {
            int score = 0;
            if (locationMatches(f,p)) score += LOCATION_MATCH;
            int iM = industryMatches(f,p);
            if (iM == 1) score += INDUSTRY_MATCH1; //3
            if (iM == 2) score += INDUSTRY_MATCH2; //4
            if (iM >= 3) score += INDUSTRY_MATCH3; //5
            std::pair<std::pair<int, int>,int> pVal;
            pVal = std::make_pair(std::make_pair(f.id,p.id), score); // (fellow/project match, score)
            projectMatchesPerFellow.push_back(pVal);
        }
        matches.push_back(projectMatchesPerFellow);
    }
}

int max = 0;
std::vector<std::pair<int, int>> finalPath;

std::vector<std::pair<int, int>> optimalMatches(std::vector<std::vector<std::pair<std::pair<int, int>, int>>> matches, int index, int score, std::vector<std::pair<int, int>> path) {
    if (index == 0) {
        std::cout << "base index: " << index << std::endl;

            for (std::pair<std::pair<int, int>, int> p : matches[index]) {
                bool check = true;
                for (std::pair<int, int> match : path) {
                    if (match.second==p.first.second||match.first==p.first.first) {
                        check = false;
                    }
                }
                if (check) {
                    int scoreFinal;
                    scoreFinal = score + p.second;
                    if (scoreFinal > max) {
                        max = scoreFinal;
                        std::cout << "af" << max << std::endl;
                        path.push_back(p.first);
                        finalPath = path;
                    }
                }
            }
        return path;
    }
    else {
        std::cout << matches[index][0].second << std::endl;
        for (std::pair<std::pair<int, int>, int> p : matches[index]) {
            bool check = true;
            std::cout << "a" << std::endl;
            for (std::pair<int, int> match : path) {
                if (match.second==p.first.second||match.first==p.first.first) {
                    check = false;
                }
            }
            if (check) {
                path.push_back(p.first);
                score = score + p.second;

                return optimalMatches(matches, index - 1, score, path);

            }

        }

    }
//    for (std::vector<std::pair<std::pair<int, int>, int>> m : matches) {
//        for (std::pair<std::pair<int, int>, int> p : m) {

//            //std::cout << p.first.first << ", " << p.first.second << ": " << p.second << std::endl;
//        }
//    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    std::ifstream company("/Users/joshpayne1/via-console/companySmall.json");
    std::ifstream pro("/Users/joshpayne1/via-console/proSmall.json");
    company >> companyJson;
    pro >> proJson;
    std::vector<fellow> FellowList;
    std::vector<project> ProjectList;
    populateFellowList(proJson, FellowList);
    populateProjectList(companyJson, ProjectList);
    createMatches(FellowList, ProjectList);
    std::cout << "------------------" << std::endl;
    for (std::vector<std::pair<std::pair<int, int>, int>> m : matches) {
        for (std::pair<std::pair<int, int>, int> p : m) {
            std::cout << p.first.first << ", " << p.first.second << ": " << p.second << std::endl;
        }
    }
    std::cout << "------------------" << std::endl;
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> j = optimalMatches(matches, int(FellowList.size())-1, 0, path);
    for (std::pair<int, int> p : finalPath) {
        std::cout << p.first <<", "<< p.second << std::endl;
    }
    std::cout << "Finished running." << std::endl;

    return a.exec();
}
