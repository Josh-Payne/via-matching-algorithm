import json as json
import numpy as np

GeoMatch = 3
IndustryMatch = 3
IndustryMatch2 = 1
IndustryMatch3Plus = 1

with open('company.json', 'r') as jsonFile:
	companyData=json.load(jsonFile)
with open('pro.json', 'r') as jsonFile:
	proData=json.load(jsonFile)
dim = max(len(companyData), len(proData))
graph = np.zeros((dim,dim))
i = 0
for company in companyData:
	j = 0
	for pro in proData:
		matchScore = 0
		# add geography matching score
		proGeos = [pro["Geo1"],pro["Geo2"],pro["Geo3"],pro["Geo4"],pro["Geo5"],pro["Geo6"]]
		companyGeos = [company["City"],company["Region"],company["Country"]]
		for proGeo in proGeos:
			for companyGeo in companyGeos:
				if proGeo == companyGeo and proGeo != "":
					matchScore = GeoMatch
		# add industry matching score
		indScore = 0
		proIndustries = [pro["Industry1"],pro["Industry2"],pro["Industry3"],pro["Industry4"],pro["Industry5"]]
		companyIndustries = [company["Industry1"],company["Industry2"],company["Industry3"],company["Industry4"],company["Industry5"]]
		for proIndustry in proIndustries:
			for companyIndustry in companyIndustries:
				if proIndustry == companyIndustry and proIndustry != "":
					indScore +=1
		if indScore >= 1:
			matchScore += IndustryMatch
		if indScore >= 2:
			matchScore += IndustryMatch2
		if indScore >= 3:
			matchScore += IndustryMatch3Plus
		graph[i][j] += matchScore
		j += 1
	i += 1
