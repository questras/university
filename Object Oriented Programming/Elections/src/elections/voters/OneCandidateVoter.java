package elections.voters;

import elections.Candidate;
import elections.ElectoralDistrict;
import elections.voters.Voter;

import java.util.ArrayList;
import java.util.Map;


public class OneCandidateVoter extends Voter {
    private final String candidatePartyName;
    private int candidateNumberOnList;

    public OneCandidateVoter(String name, String surname,
                             int electoralDistrictNumber,
                             String candidatePartyName,
                             int candidateNumberOnList) {

        super(name, surname, electoralDistrictNumber);
        this.candidatePartyName = candidatePartyName;
        this.candidateNumberOnList = candidateNumberOnList;
    }


    /**
     * Change electoral district to newElectoralDistrict and
     * update voter's candidate number on list.
     *
     * @param newElectoralDistrict - ElectoralDistrict to change to.
     */
    @Override
    public void changeElectoralDistrict(
            ElectoralDistrict newElectoralDistrict
    ) {

        super.changeElectoralDistrict(newElectoralDistrict);
        this.candidateNumberOnList +=
                newElectoralDistrict.getNumberOfCandidatesInParty(
                        this.candidatePartyName
                );
    }


    /**
     * Get candidate that the voter always votes on.
     *
     * @param candidatesByParty - candidates grouped by parties.
     * @return Candidate that the voter always votes on
     */
    @Override
    protected Candidate chooseCandidate(
            Map<String, ArrayList<Candidate>> candidatesByParty
    ) {
        return candidatesByParty
                .get(this.candidatePartyName)
                .get(this.candidateNumberOnList - 1);
    }
}
