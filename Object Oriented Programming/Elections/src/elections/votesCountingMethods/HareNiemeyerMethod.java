package elections.votesCountingMethods;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;


public class HareNiemeyerMethod extends VotesCountingMethod {

    /**
     * Calculate number of mandates for each party in votesCountByParty
     * based on Hare-Niemeyer method i.e coefficient for party is
     * calculated by formula:
     * (votes-of-party) * (number-of-mandates-to-give) / (all-votes).
     * Then each party is given number of mandates specified by
     * integer part of party's coefficient. Rest of mandates are given
     * to parties with biggest values of difference:
     * (party-coefficient)-(integer-part-of-party-coefficient).
     *
     * @param votesCountByParty - votes count of each party,
     * @param numberOfMandates  - number of mandates to give in elections.
     * @return Number of mandates for each party.
     */
    @Override
    public Map<String, Integer> countMandates(
            Map<String, Integer> votesCountByParty,
            int numberOfMandates
    ) {

        Map<String, Integer> mandatesForParties = new HashMap<>();
        int mandatesGiven = 0;

        // Count overall sum of votes.
        int sumOfAllVotes = 0;
        for (int votesCount : votesCountByParty.values()) {
            sumOfAllVotes += votesCount;
        }

        // Count mandates for parties given by integer part of coefficient
        // and calculate coefficients' rests.
        Map<String, Double> partiesCoefficientRest = new HashMap<>();
        for (String partyName : votesCountByParty.keySet()) {
            double coefficient = countCoefficient(
                    votesCountByParty.get(partyName),
                    numberOfMandates,
                    sumOfAllVotes
            );
            int mandates = (int) coefficient;
            mandatesForParties.put(partyName, mandates);
            partiesCoefficientRest.put(partyName, coefficient - mandates);
            mandatesGiven += mandates;
        }

        // Give not given mandates to parties with biggest coefficient rest.
        for (int i = 0; i < numberOfMandates - mandatesGiven; i++) {
            String greatestRestPartyName = Collections.max(
                    partiesCoefficientRest.entrySet(),
                    Map.Entry.comparingByValue()
            ).getKey();
            mandatesForParties.put(
                    greatestRestPartyName,
                    mandatesForParties.get(greatestRestPartyName) + 1
            );
            partiesCoefficientRest.remove(greatestRestPartyName);
        }

        return mandatesForParties;
    }


    /**
     * Calculate party's coefficient.
     *
     * @param votesOfParty - votes of party.
     * @param mandates     - number of mandates to give in elections,
     * @param allVotes     - number of all votes.
     * @return Party's coefficient
     */
    private double countCoefficient(int votesOfParty, int mandates,
                                    int allVotes) {
        return (double) (votesOfParty * mandates) / allVotes;
    }


    @Override
    public String toString() {
        return "Hare-Niemeyer method.";
    }
}
