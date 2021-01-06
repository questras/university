package com.company;

public abstract class Vegetable {
    protected String name;
    protected int value;
    protected int plantingPrice;
    protected boolean isPlanted;
    Thread growingThread;

    public Vegetable(String name, int plantingPrice) {
        this.name = name;
        this.value = 0;
        this.plantingPrice = plantingPrice;
        this.isPlanted = false;
        this.growingThread = null;
    }

    public int getValue() {
        return this.value;
    };

    public int getPlantingPrice() {
        return this.plantingPrice;
    };

    public String getName() {
        return this.name;
    }

    public void plant() {
        if (!this.isPlanted) {
            this.isPlanted = true;

            this.growingThread = new Thread(() -> {
                try {
                    this.grow();
                } catch (InterruptedException e) {

                }
            });
            this.growingThread.start();
        }
    }

    public void harvest() {
        if (this.isPlanted) {
            this.growingThread.interrupt();
        }
    }

    protected abstract void grow() throws InterruptedException;
}
