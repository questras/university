package com.company;

public class Potato extends Vegetable {

    public Potato() {
        super("Potato", 1);
    }

    @Override
    protected void grow() throws InterruptedException {
        // Potato is growing.
        Thread.sleep(5000);

        // Potato is fresh for 10 seconds.
        this.value = 5;
        Thread.sleep(10000);

        // Potato is almost fresh for 3 seconds.
        this.value = 2;
        Thread.sleep(3000);

        // Potato is not fresh.
        this.value = 0;
    }
}