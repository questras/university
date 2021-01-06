package elections.votesCountingMethods;

import java.util.*;


public abstract class QuotientMethod extends VotesCountingMethod {

    /**
     * Get dividers used in quotient method.
     *
     * @param numberOfMandates - number of mandates to give in elections.
     * @return Dividers used in quotient method.
     */
    protected abstract ArrayList<Integer> getDividers(
            int numberOfMandates
    );


    /**
     * Calculate number of mandates for each party in votesCountByParty
     * based on quotient method i.e divide each party's number of votes
     * by each divider from method's dividers and choose numberOfMandates
     * biggest quotients and give one mandate for each quotient.
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

        Map<String, Queue<Integer>> quotientsOfParties = new HashMap<>();
        ArrayList<Integer> dividers = this.getDividers(numberOfMandates);

        // Calculate quotients for all parties.
        for (String partyName : votesCountByParty.keySet()) {
            quotientsOfParties.put(partyName, new LinkedList<>());
            int votesCount = votesCountByParty.get(partyName);

            for (int divider : dividers) {
                quotientsOfParties.get(partyName).add(
                        votesCount / divider
                );
            }
        }

        // Set all parties mandates to 0.
        Map<String, Integer> mandatesForParties = new HashMap<>();
        for (String partyName : votesCountByParty.keySet()) {
            mandatesForParties.put(partyName, 0);
        }

        // Calculate mandates for each party.
        for (int i = 0; i < numberOfMandates; i++) {
            int bestQuotient = 0;
            String bestPartyName = null;

            for (String partyName : quotientsOfParties.keySet()) {
                int biggestQuotient = quotientsOfParties.get(partyName).peek();
                if (biggestQuotient > bestQuotient) {
                    bestQuotient = biggestQuotient;
                    bestPartyName = partyName;
                }
            }

            // Increment number of mandates for party with biggest quotient.
            mandatesForParties.put(
                    bestPartyName,
                    mandatesForParties.get(bestPartyName) + 1
            );
            // Remove this biggest quotient.
            quotientsOfParties.get(bestPartyName).remove();
        }

        return mandatesForParties;
    }
}
