package elections;

import elections.parties.*;
import elections.voters.Voter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class InputParser {

    /**
     * Return new Party based on given type.
     *
     * @param name   - name of the new party,
     * @param budget - budget of the new party,
     * @param type   - type of the new party.
     * @return A new party.
     */
    public Party parseParty(String name, int budget, String type) {
        Party party;
        switch (type) {
            case "R":
                party = new LavishParty(
                        name,
                        budget
                );
                break;
            case "S":
                party = new ModestParty(
                        name,
                        budget
                );
                break;
            case "Z":
                party = new GreedyParty(
                        name,
                        budget
                );
                break;
            default:
                party = new RandomParty(
                        name,
                        budget
                );
                break;
        }

        return party;
    }


    /**
     * Create ElectoralDistricts based on given data, and merge
     * ith district with ith+1 from districtsToMerge.
     *
     * @param districtsCount   - number of districts to create,
     * @param votersByDistrict - voters grouped by district,
     * @param candidates       - candidates grouped by district and partym
     * @param districtsToMerge - list of numbers of districts to merge.
     * @return Electoral Districts grouped by number.
     */
    public Map<Integer, ElectoralDistrict> parseDistricts(
            int districtsCount,
            Map<Integer, ArrayList<Voter>> votersByDistrict,
            Map<Integer, Map<String, ArrayList<Candidate>>> candidates,
            ArrayList<Integer> districtsToMerge
    ) {

        // Create districts.
        Map<Integer, ElectoralDistrict> districtsByNumber = new HashMap<>();
        for (int number = 1; number <= districtsCount; number++) {
            ElectoralDistrict district = new ElectoralDistrict(
                    number,
                    votersByDistrict.get(number),
                    candidates.get(number)
            );
            districtsByNumber.put(number, district);
        }

        // Merge districts
        for (int distNumToMerge : districtsToMerge) {
            districtsByNumber.get(distNumToMerge).mergeWithElectoralDistrict(
                    districtsByNumber.get(distNumToMerge + 1)
            );
            districtsByNumber.remove(distNumToMerge + 1);
        }

        return districtsByNumber;
    }
}
