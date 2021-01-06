package elections.voters;

import elections.Action;
import elections.Candidate;
import elections.ElectoralDistrict;
import elections.Vote;

import java.util.ArrayList;
import java.util.Map;
import java.util.Random;


public abstract class Voter {
    protected String name;
    protected String surname;
    protected int electoralDistrictNumber;

    public Voter(String name, String surname,
                 int electoralDistrictNumber) {

        this.name = name;
        this.surname = surname;
        this.electoralDistrictNumber = electoralDistrictNumber;
    }


    /**
     * Get best candidate from candidatesByParty based on
     * voter's strategy.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Best candidate based on voter's strategy.
     */
    protected abstract Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    );


    /**
     * Change voter's parameters based on action.
     * Initially voter cannot be influenced.
     *
     * @param action - Action to be influenced with.
     */
    public void getInfluenced(Action action) {
        return;
    }


    /**
     * Get weights of voter. Initially voter does not have
     * weights.
     *
     * @return Weights of the voter.
     */
    public ArrayList<Integer> getWeights() {
        return null;
    }


    /**
     * Get random candidate from candidates.
     *
     * @param candidates - list of Candidates to choose from.
     * @return Random candidate from list of candidates.
     */
    protected Candidate getRandomCandidate(
            ArrayList<Candidate> candidates
    ) {
        return candidates.get(new Random().nextInt(candidates.size()));
    }


    /**
     * Change voter's ElectoralDistrict to given ElectoralDistrict.
     *
     * @param newElectoralDistrict - ElectoralDistrict to change to.
     */
    public void changeElectoralDistrict(
            ElectoralDistrict newElectoralDistrict
    ) {
        this.electoralDistrictNumber = newElectoralDistrict.getNumber();
    }


    /**
     * Vote for candidate chosen from candidatesByParty based on
     * voter's strategy and return new Vote with information about
     * voter and candidate choice.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Vote with information about voter and candidate choice.
     */
    public Vote vote(Map<String, ArrayList<Candidate>> candidatesByParty) {
        Candidate chosenCandidate = this.chooseCandidate(candidatesByParty);

        return new Vote(
                chosenCandidate.getElectoralDistrictNumber(),
                chosenCandidate.getPartyName(),
                chosenCandidate.getNumberOnList(),
                this.name,
                this.surname
        );
    }


    public int getElectoralDistrictNumber() {
        return electoralDistrictNumber;
    }
}
