package com.company;

public class Gospodarz extends Farmer {
    private int[] lastVegetableValues;

    public Gospodarz() {
        super(1);
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
        this.lastVegetableValues = new int[gardenSize];

        for (int i = 0; i < gardenSize; i++) {
            this.plant(g, i);
            this.lastVegetableValues[i] = g.getVegetableValue(i);
        }
    }

    @Override
    protected void checkGarden(Garden g) {
        int gardenSize = g.getGardenSize();

        for (int i = 0; i < gardenSize; i++) {
            int currentValue = g.getVegetableValue(i);

            if (this.lastVegetableValues[i] > currentValue) {
                this.harvest(g, i);
                this.plant(g, i);
                this.lastVegetableValues[i] = g.getVegetableValue(i);
            }
            else {
                this.lastVegetableValues[i] = currentValue;
            }
        }
    }
}
