package elections;

import elections.parties.Party;
import elections.voters.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;


public class InputReader {
    private final Scanner scanner;
    private final InputParser parser;

    public InputReader(String filepath) throws FileNotFoundException {
        File file = new File(filepath);
        this.scanner = new Scanner(file);
        this.parser = new InputParser();
    }


    /**
     * Get data from file specified in scanner and parse it to
     * Elections class with information about elections.
     *
     * @return Elections class with information about elections.
     */
    public Elections getAndParseData() {
        int districtsCount = scanner.nextInt();
        int partiesCount = scanner.nextInt();
        int actionsCount = scanner.nextInt();
        int featuresCount = scanner.nextInt();

        ArrayList<Integer> districtsToMerge = this.readDistrictsToBeMerged();
        Map<String, Party> partiesByName = this.readAndParseParties(partiesCount);

        ArrayList<Integer> votersPerDistrict = new ArrayList<>();
        ArrayList<Integer> candidatesPerDistrict = new ArrayList<>();
        for (int i = 0; i < districtsCount; i++) {
            votersPerDistrict.add(scanner.nextInt());
            candidatesPerDistrict.add(votersPerDistrict.get(i) / 10);
        }

        Map<Integer, Map<String, ArrayList<Candidate>>> candidates =
                this.readCandidates(
                        candidatesPerDistrict,
                        partiesByName,
                        featuresCount
                );
        Map<Integer, ArrayList<Voter>> votersByDistrict =
                this.readVoters(votersPerDistrict, featuresCount);
        ArrayList<Action> actions =
                this.readActions(actionsCount, featuresCount);

        Map<Integer, ElectoralDistrict> districtsByNumber =
                parser.parseDistricts(
                        districtsCount,
                        votersByDistrict,
                        candidates,
                        districtsToMerge
                );

        return new Elections(
                districtsByNumber,
                partiesByName,
                actions
        );

    }


    /**
     * Read districts to be merged from input.
     *
     * @return List of numbers of districts to be merged.
     */
    private ArrayList<Integer> readDistrictsToBeMerged() {
        int mergesCount = scanner.nextInt();
        ArrayList<Integer> districtsToMerge = new ArrayList<>();

        for (int i = 0; i < mergesCount; i++) {
            String pair = scanner.next();
            int firstNumberOfPair =
                    Integer.parseInt(pair.split("\\(")[1].split(",")[0]);
            districtsToMerge.add(firstNumberOfPair);
        }

        return districtsToMerge;
    }


    /**
     * Read information about parties from input and parse this
     * information to Party classes.
     *
     * @param partiesCount - number of parties to read.
     * @return Parties read from input.
     */
    private Map<String, Party> readAndParseParties(int partiesCount) {
        ArrayList<String> names = new ArrayList<>();
        for (int i = 0; i < partiesCount; i++) {
            names.add(scanner.next());
        }

        ArrayList<Integer> budgets = new ArrayList<>();
        for (int i = 0; i < partiesCount; i++) {
            budgets.add(scanner.nextInt());
        }

        ArrayList<String> types = new ArrayList<>();
        for (int i = 0; i < partiesCount; i++) {
            types.add(scanner.next());
        }

        Map<String, Party> parties = new HashMap<>();
        for (int i = 0; i < partiesCount; i++) {
            Party party = this.parser.parseParty(
                    names.get(i),
                    budgets.get(i),
                    types.get(i)
            );
            parties.put(party.getName(), party);
        }

        return parties;
    }


    /**
     * Read and parse candidates from input and group them by district
     * and party.
     *
     * @param candidatesNumberPerDistrict - candidates number in each district,
     * @param parties                     - parties of candidates,
     * @param featuresCount               - features number for each candidate.
     * @return Candidates grouped by party and district number.
     */
    private Map<Integer, Map<String, ArrayList<Candidate>>> readCandidates(
            ArrayList<Integer> candidatesNumberPerDistrict,
            Map<String, Party> parties,
            int featuresCount
    ) {

        Map<Integer, Map<String, ArrayList<Candidate>>> candidates =
                new HashMap<>();

        // Read candidates for each district.
        for (int i = 0; i < candidatesNumberPerDistrict.size(); i++) {
            Map<String, ArrayList<Candidate>> candidatesByParty =
                    new HashMap<>();

            // Read candidates for each party.
            for (Party party : parties.values()) {
                candidatesByParty.put(party.getName(), new ArrayList<>());

                // Read party's candidates.
                for (int j = 0; j < candidatesNumberPerDistrict.get(i); j++) {
                    candidatesByParty.get(party.getName()).add(
                            this.readCandidate(featuresCount)
                    );
                }
            }

            // Add candidates grouped by party to district with number i+1.
            candidates.put(i + 1, candidatesByParty);
        }

        return candidates;
    }


    /**
     * Read and parse one candidate from input.
     *
     * @param featuresCount - number of features of candidate.
     * @return Candidate read from input.
     */
    private Candidate readCandidate(int featuresCount) {
        String name = scanner.next();
        String surname = scanner.next();
        int districtNum = scanner.nextInt();
        String partyName = scanner.next();
        int listNum = scanner.nextInt();
        ArrayList<Integer> candidateFeatures =
                new ArrayList<>();
        for (int j = 0; j < featuresCount; j++) {
            candidateFeatures.add(scanner.nextInt());
        }

        return new Candidate(
                name,
                surname,
                districtNum,
                partyName,
                listNum,
                candidateFeatures
        );
    }


    /**
     * Read and parse voters from input and group them by district.
     *
     * @param votersCountPerDistrict - number of voters in districts,
     * @param featuresCount          - number of features of one candidate.
     * @return Voters grouped by district.
     */
    private Map<Integer, ArrayList<Voter>> readVoters(
            ArrayList<Integer> votersCountPerDistrict,
            int featuresCount
    ) {

        Map<Integer, ArrayList<Voter>> votersPerDistrict = new HashMap<>();

        for (int i = 0; i < votersCountPerDistrict.size(); i++) {
            votersPerDistrict.put(i + 1, new ArrayList<>());
            for (int j = 0; j < votersCountPerDistrict.get(i); j++) {
                votersPerDistrict.get(i + 1).add(
                        this.readVoter(featuresCount)
                );
            }
        }

        return votersPerDistrict;
    }


    /**
     * Read and parse one voter from input.
     *
     * @param featuresCount - number of features of one candidate.
     * @return Voter read from input.
     */
    private Voter readVoter(int featuresCount) {
        String name = scanner.next();
        String surname = scanner.next();
        int districtNum = scanner.nextInt();
        int voterType = scanner.nextInt();

        if (voterType == 1) {
            String partyName = scanner.next();
            return new OnePartyVoter(
                    name,
                    surname,
                    districtNum,
                    partyName
            );
        }
        else if (voterType == 2) {
            String partyName = scanner.next();
            int numberOnListOfCandidate = scanner.nextInt();
            return new OneCandidateVoter(
                    name,
                    surname,
                    districtNum,
                    partyName,
                    numberOnListOfCandidate
            );
        }
        else if (voterType == 3) {
            int feature = scanner.nextInt();
            return new MinimumFeatureVoter(
                    name,
                    surname,
                    districtNum,
                    feature
            );
        }
        else if (voterType == 4) {
            int feature = scanner.nextInt();
            return new MaximumFeatureVoter(
                    name,
                    surname,
                    districtNum,
                    feature
            );
        }
        else if (voterType == 5) {
            ArrayList<Integer> weights = new ArrayList<>();
            for (int k = 0; k < featuresCount; k++) {
                weights.add(scanner.nextInt());
            }
            return new VersatileVoter(
                    name,
                    surname,
                    districtNum,
                    weights
            );
        }
        else if (voterType == 6) {
            int feature = scanner.nextInt();
            String partyName = scanner.next();
            return new OnePartyMinimumFeatureVoter(
                    name,
                    surname,
                    districtNum,
                    feature,
                    partyName
            );
        }
        else if (voterType == 7) {
            int feature = scanner.nextInt();
            String partyName = scanner.next();
            return new OnePartyMaximumFeatureVoter(
                    name,
                    surname,
                    districtNum,
                    feature,
                    partyName
            );
        }
        else {
            // One party versatile voter.
            ArrayList<Integer> weights = new ArrayList<>();
            for (int k = 0; k < featuresCount; k++) {
                weights.add(scanner.nextInt());
            }
            String partyName = scanner.next();
            return new OnePartyVersatileVoter(
                    name,
                    surname,
                    districtNum,
                    weights,
                    partyName
            );
        }
    }


    /**
     * Read and parse actions from input.
     *
     * @param actionsCount  - number of actions to read,
     * @param featuresCount - number of features of one candidate.
     * @return Actions read from input.
     */
    private ArrayList<Action> readActions(int actionsCount,
                                          int featuresCount) {

        ArrayList<Action> actionsList = new ArrayList<>();

        for (int i = 0; i < actionsCount; i++) {
            ArrayList<Integer> weights = new ArrayList<>();

            for (int j = 0; j < featuresCount; j++) {
                weights.add(scanner.nextInt());
            }

            actionsList.add(new Action(weights));
        }

        return actionsList;
    }
}
