package com.company;

import java.util.ArrayList;

public abstract class Farmer {
    protected int costs;
    protected int income;
    protected int gardenCheckThresholdInSeconds;
    protected ArrayList<String> collectedVegetables;

    public Farmer(int gardenCheckThresholdInSeconds) {
        this.costs = 0;
        this.income = 0;
        this.collectedVegetables = new ArrayList<>();
        this.gardenCheckThresholdInSeconds = gardenCheckThresholdInSeconds;
    }

    public void printReport() {
        System.out.println("-------------------");
        System.out.println("Collected vegetables:");
        for (String v : this.collectedVegetables) {
            System.out.println("\t" + v);
        }
        System.out.println("Costs: " + this.costs);
        System.out.println("Income: " + this.income);
        System.out.println("-------------------");
    }

    public void printCollectedVegetable(String name, int value) {
        System.out.println("Collected: " + name + ". Value: " + value);
    }

    public void printPlantedVegetable(String name) {
        System.out.println("Planted: " + name);
    }

    public void simulate(Garden g, int time)
            throws InterruptedException {
        int elapsedTime = 0;

        this.prepareForSeason(g);

        while (time - elapsedTime >= 0) {
            Thread.sleep(this.gardenCheckThresholdInSeconds * 1000);
            elapsedTime += this.gardenCheckThresholdInSeconds;

            this.checkGarden(g);
        }

        this.printReport();
    }

    protected abstract void prepareForSeason(Garden g);

    protected abstract void checkGarden(Garden g);

    protected abstract void plant(Garden g, int place);

    protected abstract void harvest(Garden g, int place);


}
