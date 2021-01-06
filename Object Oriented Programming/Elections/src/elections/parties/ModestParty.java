package elections.parties;

import elections.ElectoralDistrict;
import elections.Action;
import elections.ActionChoice;

import java.util.ArrayList;


public class ModestParty extends Party {

    public ModestParty(String name, int budget) {
        super(name, budget);
    }


    /**
     * Get district from districts with least number of voters.
     *
     * @param districts - list of ElectoralDistricts to choose from.
     * @return District with least number of voters.
     */
    private ElectoralDistrict getSmallestElectoralDistrict(
            ArrayList<ElectoralDistrict> districts
    ) {

        ElectoralDistrict smallestDistrict = districts.get(0);

        for (int i = 1; i < districts.size(); i++) {
            int votersCount = districts.get(i).getNumberOfAllVoters();
            int bestVotersCount = smallestDistrict.getNumberOfAllVoters();
            if (votersCount < bestVotersCount) {
                smallestDistrict = districts.get(i);
            }
        }

        return smallestDistrict;
    }


    /**
     * Get the cheapest Action from actions.
     *
     * @param actions - list of Actions to choose from.
     * @return The cheapest Action.
     */
    private Action getCheapestAction(ArrayList<Action> actions) {
        Action cheapestAction = actions.get(0);

        for (int i = 1; i < actions.size(); i++) {
            if (actions.get(i).getCost() < cheapestAction.getCost()) {
                cheapestAction = actions.get(i);
            }
        }

        return cheapestAction;
    }


    /**
     * Get best ActionChoice choosing from actions and districts
     * based on modest strategy i.e choosing affordable ActionChoice
     * that is the cheapest.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return The cheapest ActionChoice.
     */
    @Override
    protected ActionChoice getBestActionChoice(
            ArrayList<Action> actions,
            ArrayList<ElectoralDistrict> districts
    ) {

        ActionChoice bestActionChoice = new ActionChoice(
                this.getSmallestElectoralDistrict(districts),
                this.getCheapestAction(actions)
        );

        if (this.canAfford(bestActionChoice)) {
            return bestActionChoice;
        }

        return null;
    }
}
