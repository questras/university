package elections.parties;

import elections.ElectoralDistrict;
import elections.Action;
import elections.ActionChoice;

import java.util.ArrayList;


public class LavishParty extends Party {

    public LavishParty(String name, int budget) {
        super(name, budget);
    }

    /**
     * Get the most expensive ActionChoice from choices.
     *
     * @param choices - list of ActionChoices to choose from,
     * @return The most expensive ActionChoice from choices.
     */
    private ActionChoice getMostExpensiveActionChoice(
            ArrayList<ActionChoice> choices
    ) {

        if (choices.size() == 0) {
            return null;
        }

        ActionChoice bestChoice = choices.get(0);

        for (int i = 1; i < choices.size(); i++) {
            if (choices.get(i).getCost() > bestChoice.getCost()) {
                bestChoice = choices.get(i);
            }
        }

        return bestChoice;
    }


    /**
     * Get best ActionChoice choosing from actions and districts
     * based on lavish strategy i.e choosing affordable ActionChoice
     * that is the most expensive.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return The most expensive affordable ActionChoice.
     */
    @Override
    protected ActionChoice getBestActionChoice(
            ArrayList<Action> actions,
            ArrayList<ElectoralDistrict> districts
    ) {

        ArrayList<ActionChoice> affordableActionChoices =
                this.getAffordableActionChoices(actions, districts);

        return this.getMostExpensiveActionChoice(affordableActionChoices);
    }
}
