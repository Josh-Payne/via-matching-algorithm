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

std::vector<std::vector<std::pair<std::pair<int, int>, int>>> matches;

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
            if (!lf.compare(lp)) counter++;
        }
    }
    if (counter > 3) counter = 3;
    return counter;
}

void createMatches(std::vector<fellow>& FellowList, std::vector<project>& ProjectList) {
    for (fellow f : FellowList) {
        std::vector<std::pair<std::pair<int, int>,int>> projectMatchesPerFellow;
        for (project p : ProjectList) {
            int score = 0;
            if (locationMatches(f,p)) score += LOCATION_MATCH;
            if (industryMatches(f,p) == 1) score += INDUSTRY_MATCH1; //3
            if (industryMatches(f,p) == 2) score += INDUSTRY_MATCH2; //4
            if (industryMatches(f,p) == 3) score += INDUSTRY_MATCH3; //5
            std::pair<std::pair<int, int>,int> pVal;
            pVal = std::make_pair(std::make_pair(f.id,p.id), score); // (fellow/project match, score)
            projectMatchesPerFellow.push_back(pVal);
        }
        matches.push_back(projectMatchesPerFellow);
    }
}

int max = 0;

int getScore(int fel, int proj) {
    for (std::vector<std::pair<std::pair<int, int>, int>> matchRow : matches) {
        for (std::pair<std::pair<int, int>, int> match : matchRow) {
            if (match.first.first == fel && match.first.second == proj) {
                return match.second;
            }
        }
    }
    return 0;
}

std::vector<project> removeProject(std::vector<project>& pList, project rmP) {
    std::vector<project> newList;
    for (project p : pList) {
        if (p.id != rmP.id) {
            newList.push_back(p);
        }
    }
    return newList;
}

std::vector<std::pair<int, int>> optimalMatches(std::vector<fellow>& workingFellowList, std::vector<project>& workingProjectList, int index, int score, std::vector<std::pair<int, int>> path) {
    if (index == 1) {
        std::cout << "base index: " << index << std::endl;
        for (project p : workingProjectList) {
            int scoreFinal = score;
            scoreFinal = score + getScore(workingFellowList[0].id, p.id);
            if (scoreFinal > max) {
                max = scoreFinal;
                path[1] = std::make_pair(workingFellowList[0].id, p.id);
            }
        }
        return path;
    }

    else {
        for (project p : workingProjectList) {
            std::cout << "fellowlist size: " << workingFellowList.size() << std::endl;
            std::cout << "index: " << index << std::endl;
            std::vector<project> newProjectList = removeProject(workingProjectList, p);
            path.push_back(std::make_pair(workingFellowList[index].id, p.id));
            score = score + getScore(workingFellowList[index].id, p.id);
            return optimalMatches(workingFellowList, newProjectList, index - 1, score, path);
        }
    }
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
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> j = optimalMatches(FellowList, ProjectList, int(FellowList.size()), 0, path);
    for (std::pair<int, int> p : path) {
        std::cout << p.first <<", "<< p.second << std::endl;
    }

    std::cout << "Finished running." << std::endl;

    return a.exec();
}
