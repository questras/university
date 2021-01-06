package com.company;

public class PracownikPGR extends Farmer {

    public PracownikPGR() {
        super(10);
    }

    @Override
    protected void plant(Garden garden, int place) {
        garden.plantRandomVegetable(place);
        this.costs += garden.getVegetablePlantingPrice(place);
        this.printPlantedVegetable(garden.getVegetableName(place));
    }

    @Override
    protected void harvest(Garden garden, int place) {
        String name = garden.getVegetableName(place);
        int value = garden.harvestVegetable(place);

        this.income += value;
        this.collectedVegetables.add(name);

        this.printCollectedVegetable(name, value);
    }

    @Override
    protected void prepareForSeason(Garden g) {
        int gardenSize = g.getGardenSize();
        for (int i = 0; i < gardenSize; i++) {
            this.plant(g, i);
        }
    }

    @Override
    protected void checkGarden(Garden g) {
        int gardenSize = g.getGardenSize();

        for (int i = 0; i < gardenSize; i++) {
            this.harvest(g, i);
        }
        for (int i = 0; i < gardenSize; i++) {
            this.plant(g, i);
        }
    }
}
