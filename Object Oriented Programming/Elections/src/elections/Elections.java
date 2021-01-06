package elections;

import elections.parties.Party;
import elections.voters.Voter;
import elections.votesCountingMethods.DHondtMethod;
import elections.votesCountingMethods.HareNiemeyerMethod;
import elections.votesCountingMethods.SainteLagueMethod;
import elections.votesCountingMethods.VotesCountingMethod;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;


public class Elections {
    private final Map<Integer, ElectoralDistrict> districtsByNumber;
    private final Map<String, Party> partiesByName;
    private final ArrayList<Action> actions;

    public Elections(Map<Integer, ElectoralDistrict> districtsByNumber,
                     Map<String, Party> partiesByName,
                     ArrayList<Action> actions) {

        this.districtsByNumber = districtsByNumber;
        this.partiesByName = partiesByName;
        this.actions = actions;
    }


    /**
     * Start campaign of parties i.e parties run campaign as
     * long as they can afford it.
     */
    public void startCampaign() {
        int successfulCampaigns = 1;

        while (successfulCampaigns > 0) {
            successfulCampaigns = 0;
            for (Party party : this.partiesByName.values()) {
                boolean success = party.runCampaign(
                        this.actions,
                        new ArrayList<>(this.districtsByNumber.values())
                );
                if (success) {
                    successfulCampaigns++;
                }
            }
        }
    }


    /**
     * Collect votes from all voters in all districts and group
     * them by district number.
     *
     * @return Votes grouped by district number.
     */
    private Map<Integer, ArrayList<Vote>> collectVotes() {
        Map<Integer, ArrayList<Vote>> votesByDistrict = new HashMap<>();

        for (ElectoralDistrict district : this.districtsByNumber.values()) {
            votesByDistrict.put(district.getNumber(), new ArrayList<>());

            for (Voter voter : district.getAllVoters()) {
                votesByDistrict.get(district.getNumber()).add(
                        voter.vote(district.getCandidatesByParty())
                );
            }
        }

        return votesByDistrict;
    }


    /**
     * Run elections i.e collect votes from voters and count all votes
     * and mandates and print report of elections.
     */
    public void runElections() {
        Map<Integer, ArrayList<Vote>> votesByDistrict = this.collectVotes();
        Map<Integer, VotesCounter> votesCounterByDistrict = new HashMap<>();

        // Create voteCounter for each district and count votes in each counter.
        for (int districtNumber : votesByDistrict.keySet()) {
            VotesCounter votesCounter = new VotesCounter(
                    districtNumber,
                    this.partiesByName.keySet(),
                    votesByDistrict.get(districtNumber)
            );
            votesCounter.countVotes();
            votesCounterByDistrict.put(districtNumber, votesCounter);
        }

        // Methods to count mandates.
        VotesCountingMethod[] methods = {
                new DHondtMethod(),
                new HareNiemeyerMethod(),
                new SainteLagueMethod()
        };

        // Count mandates for each counting method
        for (VotesCountingMethod method : methods) {
            Map<Integer, Map<String, Integer>> mandatesByDistrictAndParty =
                    new HashMap<>();

            for (int districtNumber : votesByDistrict.keySet()) {
                votesCounterByDistrict.get(districtNumber).countMandates(method);
                mandatesByDistrictAndParty.put(
                        districtNumber,
                        votesCounterByDistrict
                                .get(districtNumber)
                                .getMandatesByParty()
                );
            }

            Map<String, Integer> allMandatesByParty =
                    this.calculateAllMandatesForParties(
                            mandatesByDistrictAndParty
                    );
            this.printElectionsReport(
                    method,
                    new ArrayList<>(votesCounterByDistrict.values()),
                    allMandatesByParty
            );
        }

    }


    /**
     * Count sum of mandates for each party.
     *
     * @param mandatesForPartiesInDistricts - mandates of each party by district.
     * @return Sum of mandates for each party.
     */
    private Map<String, Integer> calculateAllMandatesForParties(
            Map<Integer, Map<String, Integer>> mandatesForPartiesInDistricts
    ) {
        Map<String, Integer> allMandatesForParties = new HashMap<>();

        for (String partyName : this.partiesByName.keySet()) {
            int sumOfVotes = 0;
            for (Map<String, Integer> mandates : mandatesForPartiesInDistricts.values()) {
                sumOfVotes += mandates.get(partyName);
            }
            allMandatesForParties.put(partyName, sumOfVotes);
        }

        return allMandatesForParties;
    }


    /**
     * Print report of elections i.e for each district all voters with
     * their votes, all candidates and number of votes they collected,
     * number of mandates each party collected and overall sum of
     * mandates of each party.
     *
     * @param method              - method used to count mandates,
     * @param votesCounters       - counters of votes,
     * @param allMandatesPerParty - number of mandates of each party.
     */
    private void printElectionsReport(
            VotesCountingMethod method,
            ArrayList<VotesCounter> votesCounters,
            Map<String, Integer> allMandatesPerParty
    ) {
        System.out.println(method.toString());

        // Print info about each district.
        for (VotesCounter counter : votesCounters) {

            System.out.println("District: " + counter.getDistrictNumber());
            this.printVotes(counter);
            this.printCandidates(counter);
            this.printMandates(counter);
        }

        System.out.println("For all districts:");
        for (String party : allMandatesPerParty.keySet()) {
            System.out.print(party + ": ");
            System.out.println(allMandatesPerParty.get(party) + " mandates.");
        }

    }


    /**
     * Print number of mandates each party collected in counter.
     *
     * @param counter - counter of votes.
     */
    private void printMandates(VotesCounter counter) {
        Map<String, Integer> mandates = counter.getMandatesByParty();
        for (String party : mandates.keySet()) {
            System.out.print(party + ": ");
            System.out.println(mandates.get(party) + " mandates.");
        }
    }


    /**
     * Print each candidate with number of votes collected in counter.
     *
     * @param counter - counter of votes.
     */
    private void printCandidates(VotesCounter counter) {
        int districtNumber = counter.getDistrictNumber();
        Map<String, ArrayList<Integer>> votesByParty =
                counter.getCandidatesVotesByParty();

        for (String partyName : votesByParty.keySet()) {
            ArrayList<Integer> votes = votesByParty.get(partyName);
            for (int listNumber = 1; listNumber <= votes.size(); listNumber++) {
                printCandidate(
                        districtNumber,
                        listNumber,
                        partyName,
                        votes.get(listNumber - 1)
                );
            }
        }
    }


    /**
     * Print info about candidate from given district from given party
     * on given number on list.
     *
     * @param districtNumber - district number of candidate,
     * @param numberOnList   - number on list of candidate,
     * @param partyName      - party name of candidate,
     * @param numberOfVotes  - number of votes the candidate collected.
     */
    private void printCandidate(int districtNumber,
                                int numberOnList,
                                String partyName,
                                int numberOfVotes) {

        Candidate candidate = this.districtsByNumber
                .get(districtNumber)
                .getCandidate(partyName, numberOnList);

        String name = candidate.getName();
        String surname = candidate.getSurname();
        System.out.print(name + " ");
        System.out.print(surname + " number ");
        System.out.print(numberOnList + " in ");
        System.out.print(partyName + " got ");
        System.out.println(numberOfVotes + " votes.");
    }


    /**
     * Print all votes from counter.
     *
     * @param counter - counter of votes.
     */
    private void printVotes(VotesCounter counter) {
        ArrayList<Vote> votes = counter.getAllVotes();
        for (Vote vote : votes) {
            this.printVote(vote);
        }
    }


    /**
     * Print information of one vote i.e who voted on whom.
     *
     * @param vote - Vote to print.
     */
    private void printVote(Vote vote) {
        int districtNumber = vote.getElectoralDistrictNumber();
        String name = vote.getVoterName();
        String surname = vote.getVoterSurname();
        String candidateParty = vote.getPartyName();
        int candidateNumberOnList = vote.getNumberOnList();
        Candidate candidate = this.districtsByNumber
                .get(districtNumber)
                .getCandidate(candidateParty, candidateNumberOnList);
        String candidateName = candidate.getName();
        String candidateSurname = candidate.getSurname();

        System.out.print(name + " ");
        System.out.print(surname + " voted for ");
        System.out.print(candidateName + " ");
        System.out.println(candidateSurname);
    }
}
