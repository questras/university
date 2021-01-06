package elections.votesCountingMethods;

import java.util.Map;


public abstract class VotesCountingMethod {

    /**
     * Calculate number of mandates for each party in votesCountByParty.
     *
     * @param votesCountByParty - votes count of each party,
     * @param numberOfMandates  - number of mandates to give in elections.
     * @return Number of mandates for each party.
     */
    public abstract Map<String, Integer> countMandates(
            Map<String, Integer> votesCountByParty,
            int numberOfMandates
    );
}
