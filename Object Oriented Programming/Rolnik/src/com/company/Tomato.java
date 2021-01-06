package com.company;

public class Tomato extends Vegetable {

    public Tomato() {
        super("Tomato", 3);
    }

    @Override
    protected void grow() throws InterruptedException {
        // Tomato is growing.
        Thread.sleep(5000);

        // Tomato's values rises for 10 seconds.
        for (int i = 0; i < 5; i++) {
            Thread.sleep(2000);
            this.value += 2;
        }

        // Tomato's value lowers for 10 seconds.
        for (int i = 0; i < 5; i++) {
            Thread.sleep(2000);
            this.value -= 2;
        }
    }
}