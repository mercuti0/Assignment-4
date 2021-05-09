/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "strlib.h"
#include "prototypes.h"
#include "testing/SimpleTest.h"

#include "map.h"
#include <fstream>

using namespace std;

// BEGIN PART 1 CODE

/*
 * This provided helper function takes in the name of the file containing historical
 * US election district map data, parses the file, and builds a map representing the
 * district map data on a state by state basis. You should not have to modify the
 * contents of this file at all, but if you suspect there is a bug or inconsistency,
 * feel free to let the teaching team know!
 */
Map<string, Vector<District>> readUSDistrictDataFromFile(string filename) {
    Map<string, Vector<District>> usDistrictData;
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        Vector<string> contents = stringSplit(line, ",");
        string stateName = toLowerCase(contents[0]);
        contents = contents.subList(1);
        Vector<District> districts;
        for (int i = 0; i < contents.size(); i += 3) {
            districts.add(District(contents[i],
                                   stringToInteger(contents[i+1]),
                                   stringToInteger(contents[i+2])));
        }
        usDistrictData[stateName] = districts;
    }
    return usDistrictData;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment. Requirements for this function are outlined in Part 1
 * of the assignment handout on Ethical Responsibility and Gerrymandering.
 */
bool isMapGerrymandered(Vector<District>& districts) {
    int totalVotes = 0;
    int totalRedWasted = 0;
    int totalBlueWasted = 0;

    for (District district: districts) {
        int currDistrictTot = district.numBlueVotes + district.numRedVotes;
        totalVotes += currDistrictTot;
        int majority = (currDistrictTot / 2) + 1;
        if (district.numBlueVotes > district.numRedVotes) {
            totalRedWasted  += district.numRedVotes;
            totalBlueWasted += (district.numBlueVotes - majority);
        } else {
            totalBlueWasted += district.numBlueVotes;
            totalRedWasted  += (district.numRedVotes - majority);
          }
    }
    if (districts.size() > 2 && totalVotes > 0) {
        double efficiencyGap = abs(totalBlueWasted - totalRedWasted) / (double)totalVotes;
        if (efficiencyGap > 0.07)
             return true;
    }
    return false;
}

// END PART 1 CODE


// BEGIN PART 2 CODE

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 *
 * Implementing this function is not strictly required (we won't be
 * testing it directly), but is strongly recommended. Doing all the district
 * size and majority calculations in this function can leave you to focus on
 * the interesting recursive parts in your other functions. If you choose
 * to implement this function we recommend following this spec:
 *    - If the districts are not the same size, this function should return false.
 *    - Otherwise, if the districts are the same size, you should count the total
 *      number of votes for each party in each district. This function should
 *      return true if the same party holds the majority in both districts and
 *      false otherwise.
 */
bool isDistrictSplitGerrymandered(Vector<Precinct> district1, Vector<Precinct> district2, int majority) {
    return false;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment. Requirements for this function are outlined in Part 2 of the
 * assignment handout on Ethical Responsibility and Gerrymandering.
 */
bool isMapSusceptible(Vector<Precinct>& precincts) {
    return false;
}

// END PART 2 CODE

// BEGIN PART 1 TESTS
PROVIDED_TEST("Example test case from handout – result is gerrymandered.") {
    Vector<District> districts;
    districts.add(District("1", 12, 13));
    districts.add(District("2", 17, 8));
    districts.add(District("3", 11, 14));
    districts.add(District("4", 10, 15));
    EXPECT(isMapGerrymandered(districts));
}

PROVIDED_TEST("Small example test case where result is not gerrymandered.") {
    Vector<District> districts;
    districts.add(District("1", 13, 12));
    districts.add(District("2", 12, 13));
    districts.add(District("3", 14, 11));
    districts.add(District("4", 11, 14));
    EXPECT(!isMapGerrymandered(districts));
}

PROVIDED_TEST("US District Examples: Too few districts to be gerrymandered.") {
    Map<string, Vector<District>> usDistrictMap = readUSDistrictDataFromFile("res/districts.txt");
    EXPECT(!isMapGerrymandered(usDistrictMap["wyoming"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["delaware"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["montana"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["south dakota"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["rhode island"]));
}

PROVIDED_TEST("Assorted US District examples, both positive and neagtive results.") {
    Map<string, Vector<District>> usDistrictMap = readUSDistrictDataFromFile("res/districts.txt");
    EXPECT(isMapGerrymandered(usDistrictMap["wisconsin"]));
    EXPECT(isMapGerrymandered(usDistrictMap["minnesota"]));
    EXPECT(isMapGerrymandered(usDistrictMap["michigan"]));
    EXPECT(isMapGerrymandered(usDistrictMap["kansas"]));
    EXPECT(isMapGerrymandered(usDistrictMap["iowa"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["arizona"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["new york"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["california"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["florida"]));
    EXPECT(!isMapGerrymandered(usDistrictMap["georgia"]));
}
// END PART 1 TESTS

// BEGIN PART 2 TESTS
PROVIDED_TEST("Example from handout – map is susceptible to being gerrymandered.") {
    Vector<Precinct> precincts;
    precincts.add(Precinct(35, 65));
    precincts.add(Precinct(40, 60));
    precincts.add(Precinct(55, 45));
    precincts.add(Precinct(53, 47));
    EXPECT(isMapSusceptible(precincts));

}

PROVIDED_TEST("Small example with 4 precincts, map is not susceptible.") {
    Vector<Precinct> precincts;
    precincts.add(Precinct(45, 55));
    precincts.add(Precinct(51, 49));
    precincts.add(Precinct(55, 45));
    precincts.add(Precinct(49, 51));
    EXPECT(!isMapSusceptible(precincts));
}

PROVIDED_TEST("Larger positive example with more precincts.") {
    Vector<Precinct> precincts;
    precincts.add(Precinct(35, 65));
    precincts.add(Precinct(40, 60));
    precincts.add(Precinct(55, 45));
    precincts.add(Precinct(53, 47));
    precincts.add(Precinct(35, 65));
    precincts.add(Precinct(40, 60));
    precincts.add(Precinct(55, 45));
    precincts.add(Precinct(53, 47));
    EXPECT(isMapSusceptible(precincts));
}

PROVIDED_TEST("Larger negative example with more precincts.") {
    Vector<Precinct> precincts;
    precincts.add(Precinct(45, 55));
    precincts.add(Precinct(51, 49));
    precincts.add(Precinct(55, 45));
    precincts.add(Precinct(49, 51));
    precincts.add(Precinct(45, 55));
    precincts.add(Precinct(51, 49));
    precincts.add(Precinct(55, 45));
    precincts.add(Precinct(49, 51));
    EXPECT(!isMapSusceptible(precincts));
}
// END PART 2 TESTS
