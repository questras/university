package elections.parties;

import elections.ElectoralDistrict;
import elections.Action;
import elections.ActionChoice;

import java.util.ArrayList;
import java.util.Random;


public class RandomParty extends Party {

    public RandomParty(String name, int budget) {
        super(name, budget);
    }


    /**
     * Get best ActionChoice choosing from actions and districts
     * based on random strategy i.e choosing random affordable
     * ActionChoice.
     *
     * @param actions   - actions to choose from,
     * @param districts - districts to choose from.
     * @return Random affordable ActionChoice
     */
    @Override
    protected ActionChoice getBestActionChoice(
            ArrayList<Action> actions,
            ArrayList<ElectoralDistrict> districts
    ) {

        ArrayList<ActionChoice> affordableActionChoices =
                this.getAffordableActionChoices(actions, districts);

        if (affordableActionChoices.size() == 0) {
            return null;
        }

        return affordableActionChoices.get(
                new Random().nextInt(affordableActionChoices.size())
        );
    }
}
