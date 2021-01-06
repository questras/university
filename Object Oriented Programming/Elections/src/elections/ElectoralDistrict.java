package elections;

import elections.voters.Voter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class ElectoralDistrict {
    private final int number;
    private ArrayList<Voter> allVoters;
    private Map<String, ArrayList<Candidate>> candidatesByParty;

    public ElectoralDistrict(int number, ArrayList<Voter> allVoters,
                             Map<String, ArrayList<Candidate>> candidatesByParty) {
        this.number = number;
        this.allVoters = allVoters;
        this.candidatesByParty = candidatesByParty;
    }

    public int getNumberOfAllVoters() {
        return this.allVoters.size();
    }

    public ArrayList<Voter> getAllVoters() {
        return new ArrayList<>(this.allVoters);
    }

    public Map<String, ArrayList<Candidate>> getCandidatesByParty() {
        return new HashMap<>(this.candidatesByParty);
    }

    public int getNumberOfCandidatesInParty(String partyName) {
        return this.candidatesByParty.get(partyName).size();
    }

    public ArrayList<Candidate> getCandidatesFromParty(String partyName) {
        return new ArrayList<>(this.candidatesByParty.get(partyName));
    }

    public Candidate getCandidate(String partyName, int numberOnList) {
        return this.candidatesByParty.get(partyName).get(numberOnList - 1);
    }

    public int getNumber() {
        return number;
    }


    /**
     * Get number of mandates in district i.e
     * number of voters divided by 10.
     *
     * @return Number of mandates in district.
     */
    public int getNumberOfMandates() {
        return this.getNumberOfAllVoters() / 10;
    }


    /**
     * Merge with given ElectoralDistrict i.e add and update all its voters
     * and candidates to current ElectoralDistrict.
     *
     * @param anotherElectoralDistrict - ElectoralDistrict to merge with.
     */
    public void mergeWithElectoralDistrict(
            ElectoralDistrict anotherElectoralDistrict
    ) {

        anotherElectoralDistrict.getMergedWithElectoralDistrict(this);
        this.allVoters.addAll(anotherElectoralDistrict.getAllVoters());

        // There are the same parties in each district.
        for (String partyName : this.candidatesByParty.keySet()) {
            this.candidatesByParty.get(partyName).addAll(
                    anotherElectoralDistrict.getCandidatesFromParty(partyName)
            );
        }
    }


    /**
     * Get merged with given ElectoralDistrict i.e update current
     * district's voters and candidates based on given ElectoralDistrict.
     *
     * @param anotherElectoralDistrict - ElectoralDistrict to be merged to.
     */
    public void getMergedWithElectoralDistrict(
            ElectoralDistrict anotherElectoralDistrict
    ) {
        for (Voter voter : this.allVoters) {
            voter.changeElectoralDistrict(anotherElectoralDistrict);
        }
        for (ArrayList<Candidate> candidates : this.candidatesByParty.values()) {
            for (Candidate candidate : candidates) {
                candidate.changeElectoralDistrict(anotherElectoralDistrict);
            }
        }
    }
}
