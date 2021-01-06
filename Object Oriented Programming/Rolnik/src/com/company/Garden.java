package com.company;

import java.util.Random;

public class Garden {
    private int gardenSize;
    private Vegetable[] vegetables;

    public Garden(int gardenSize) {
        this.gardenSize = gardenSize;
        this.vegetables = new Vegetable[gardenSize];
    }

    public int getGardenSize() {
        return this.gardenSize;
    }

    private boolean correctPlace(int place) {
        return place >= 0 && place < this.gardenSize;
    }

    public void plantVegetable(Vegetable vegetable, int place) {
        if (this.correctPlace(place) && this.vegetables[place] == null) {
            this.vegetables[place] = vegetable;
            this.vegetables[place].plant();
        }
    }

    public void plantRandomVegetable(int place) {
        Random generator = new Random();
        int randomNumber = generator.nextInt(3);

        switch (randomNumber) {
            case 0:
                this.plantVegetable(new Potato(), place);
            case 1:
                this.plantVegetable(new Tomato(), place);
            case 2:
                this.plantVegetable(new Cucumber(), place);
        }
    }

    public int harvestVegetable(int place) {
        int value = this.getVegetableValue(place);
        if (this.correctPlace(place)) {
            this.vegetables[place].harvest();
            this.vegetables[place] = null;
        }

        return value;
    }

    public int getVegetableValue(int place) {
        if (this.correctPlace(place)) {
            if (this.vegetables[place] == null) {
                return 0;
            }
            else {
                return vegetables[place].getValue();
            }
        }
        return 0;
    }

    public String getVegetableName(int place) {
        if (this.correctPlace(place)) {
            if (this.vegetables[place] == null) {
                return "";
            }
            else {
                return vegetables[place].getName();
            }
        }
        return "";
    }

    public int getVegetablePlantingPrice(int place) {
        if (this.correctPlace(place)) {
            if (this.vegetables[place] == null) {
                return 0;
            }
            else {
                return vegetables[place].getPlantingPrice();
            }
        }
        return 0;
    }
}
