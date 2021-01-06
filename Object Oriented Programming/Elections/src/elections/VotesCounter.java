package elections;

import elections.votesCountingMethods.VotesCountingMethod;

import java.util.*;


public class VotesCounter {

    private final int districtNumber;
    private final Set<String> partiesNames;
    private final ArrayList<Vote> allVotes;
    private Map<String, ArrayList<Integer>> candidatesVotesByParty;
    private Map<String, Integer> sumOfVotesByParty;
    private Map<String, Integer> mandatesByParty;

    public VotesCounter(int districtNumber, Set<String> partiesNames,
                        ArrayList<Vote> votes) {

        this.districtNumber = districtNumber;
        this.partiesNames = partiesNames;
        this.allVotes = votes;
        this.candidatesVotesByParty = null;
        this.sumOfVotesByParty = null;
        this.mandatesByParty = null;
    }

    public Map<String, ArrayList<Integer>> getCandidatesVotesByParty() {
        return candidatesVotesByParty;
    }


    public Map<String, Integer> getMandatesByParty() {
        return mandatesByParty;
    }

    public ArrayList<Vote> getAllVotes() {
        return allVotes;
    }

    public int getDistrictNumber() {
        return districtNumber;
    }


    /**
     * Count votes of each candidate and sum of votes of each party.
     */
    public void countVotes() {
        int candidatesCount = this.getNumberOfCandidatesInOneParty();
        this.candidatesVotesByParty = new HashMap<>();
        this.sumOfVotesByParty = new HashMap<>();

        // Set all votes to 0.
        for (String partyName : this.partiesNames) {
            this.candidatesVotesByParty.put(
                    partyName,
                    new ArrayList<>(Collections.nCopies(candidatesCount, 0))
            );
            this.sumOfVotesByParty.put(partyName, 0);
        }

        // Count votes for each candidate in each party.
        for (Vote vote : this.allVotes) {
            this.incrementCandidateVotes(
                    vote.getPartyName(),
                    vote.getNumberOnList()
            );
            this.incrementSumOfPartyVotes(vote.getPartyName());
        }
    }


    /**
     * Count number of mandates for each party based on given method.
     * To count mandates votes must be counted first.
     *
     * @param method - method to use when counting mandates.
     */
    public void countMandates(VotesCountingMethod method) {
        if (areVotesCounted()) {
            int mandatesCount = this.getNumberOfMandates();
            this.mandatesByParty =
                    method.countMandates(this.sumOfVotesByParty, mandatesCount);
        }
    }


    /**
     * Add 1 to votes of candidate in given party on given number on list.
     *
     * @param partyName    - name of party of the candidate,
     * @param numberOnList - number on list of candidate.
     */
    private void incrementCandidateVotes(String partyName, int numberOnList) {
        int listIndex = numberOnList - 1;
        this.candidatesVotesByParty.get(partyName).set(
                listIndex,
                this.candidatesVotesByParty.get(partyName).get(listIndex) + 1
        );
    }


    /**
     * Add 1 to votes of given party.
     *
     * @param partyName - name of party to add 1 vote to.
     */
    private void incrementSumOfPartyVotes(String partyName) {
        this.sumOfVotesByParty.put(
                partyName,
                this.sumOfVotesByParty.get(partyName) + 1
        );
    }


    /**
     * Check if votes are already counted.
     *
     * @return True if votes are already counted, false otherwise.
     */
    private boolean areVotesCounted() {
        return !(this.sumOfVotesByParty == null ||
                this.candidatesVotesByParty == null);
    }


    /**
     * Get number of candidates in one party.
     *
     * @return Number of candidates in one party.
     */
    public int getNumberOfCandidatesInOneParty() {
        return this.allVotes.size() / 10;
    }


    /**
     * Get number of mandates in district.
     *
     * @return Number of mandates in district.
     */
    public int getNumberOfMandates() {
        // Number of mandates is equal to number of candidates in one party.
        return this.getNumberOfCandidatesInOneParty();
    }
}
