package com.company;

import java.util.Random;

class Nukleotyd {
    private char typ;

    public Nukleotyd(char typ) {
        this.typ = typ;
    }

    public char getTyp() {
        return this.typ;
    }

    public static Nukleotyd getRandomNukleotyd() {
        Random generator = new Random();
        int randomNumber = generator.nextInt(4);
        switch (randomNumber) {
            case 0: return new Nukleotyd('A');
            case 1: return new Nukleotyd('C');
            case 2: return new Nukleotyd('G');
            case 3: return new Nukleotyd('T');
        }

        return null;
    }
}

class KwasNukleinowy {
    private Nukleotyd[] nukleotydy;

    public KwasNukleinowy(Nukleotyd[] nukleotydy) {
        this.nukleotydy = nukleotydy;
    }

    public int getLength() {
        return this.nukleotydy.length;
    }

    public void setNukleotyd(int index, Nukleotyd nukleotyd) {
        this.nukleotydy[index] = nukleotyd;
    }

    public Nukleotyd getNukleotyd(int index) {
        return this.nukleotydy[index];
    }

    public void swapNukleotydy(int index1, int index2) {
        Nukleotyd temp = this.nukleotydy[index1];
        this.nukleotydy[index1] = this.nukleotydy[index2];
        this.nukleotydy[index2] = temp;
    }

    public boolean czyZawieraPodciag(Nukleotyd[] nukleotydy) {
        if (nukleotydy.length > this.nukleotydy.length) {
            return false;
        }

        for (int i = 0; i < this.nukleotydy.length - nukleotydy.length; i++) {
            boolean flaga = true;
            for (int k = i; k < nukleotydy.length; k++) {
                if (this.nukleotydy[k] != nukleotydy[k]) {
                    flaga = false;
                    break;
                }
            }
            if (flaga) {
                return true;
            }
        }

        return false;
    }
}

class Szczepionka {
    private Nukleotyd[] nukleotydy;

    public Szczepionka(Nukleotyd[] nukleotydy) {
        this.nukleotydy = nukleotydy;
    }

    public Nukleotyd[] getNukleotydy() {
        return this.nukleotydy;
    }
}

class Przeciwcialo {
    private Nukleotyd[] nukleotydyKtorePokonuje;

    public Przeciwcialo(Nukleotyd[] nukleotydyKtorePokonuje) {
        this.nukleotydyKtorePokonuje = nukleotydyKtorePokonuje;
    }

    public boolean czyNiszczyWirusa(Wirus wirus) {
        KwasNukleinowy kwasNukleinowyWirusa = wirus.zwrocKwasNukleinowy();

        return kwasNukleinowyWirusa.czyZawieraPodciag(this.nukleotydyKtorePokonuje);
    }
}

class Organizm {
    private Przeciwcialo wytworzonePrzeciwcialo;

    public Organizm() {
        this.wytworzonePrzeciwcialo = null;
    }

    private void wytworzPrzeciwcialo(Nukleotyd[] nukleotydy) {
        this.wytworzonePrzeciwcialo = new Przeciwcialo(nukleotydy);
    }

    public void zaszczep(Szczepionka szczepionka) {
        this.wytworzPrzeciwcialo(szczepionka.getNukleotydy());
    }

    public Przeciwcialo getWytworzonePrzeciwcialo() {
        return wytworzonePrzeciwcialo;
    }
}

abstract class Wirus {
    protected int liczbaMutacjiNaMiesiac;
    protected KwasNukleinowy kwasNukleinowy;

    public Wirus(KwasNukleinowy kwasNukleinowy, int liczbaMutacjiNaMiesiac) {
        this.kwasNukleinowy = kwasNukleinowy;
        this.liczbaMutacjiNaMiesiac = liczbaMutacjiNaMiesiac;
    }

    public KwasNukleinowy zwrocKwasNukleinowy() {
        return kwasNukleinowy;
    }

    public boolean symulacja(int miesiace, Organizm organizm, Szczepionka szczepionka) {
        organizm.zaszczep(szczepionka);
        while (miesiace - this.liczbaMutacjiNaMiesiac >= 0) {
            miesiace -= this.liczbaMutacjiNaMiesiac;
            this.mutuj();
        }

        return organizm.getWytworzonePrzeciwcialo().czyNiszczyWirusa(this);
    }

    protected abstract void mutuj();
}

class WirusDziwny extends Wirus {

    public WirusDziwny(KwasNukleinowy kwasNukleinowy, int liczbaMutacji) {
        super(kwasNukleinowy, liczbaMutacji);
    }

    @Override
    protected void mutuj() {
        Random generator = new Random();
        int pozycja1 = generator.nextInt(this.kwasNukleinowy.getLength());
        int pozycja2 = pozycja1;
        while (pozycja2 == pozycja1) {
            pozycja2 = generator.nextInt(this.kwasNukleinowy.getLength());
        }

        this.kwasNukleinowy.swapNukleotydy(pozycja1, pozycja2);
    }
}

class WirusSpecyficzny extends Wirus {
    private int[] okreslonePozycje;
    private Nukleotyd[] okresloneNukleotydy;

    public WirusSpecyficzny(KwasNukleinowy kwasNukleinowy, int liczbaMutacji, int[] okreslonePozycje, Nukleotyd[] okresloneNukleotydy) {
        super(kwasNukleinowy, liczbaMutacji);
        this.okreslonePozycje = okreslonePozycje;
        this.okresloneNukleotydy = okresloneNukleotydy;
    }

    private boolean jestWOkreslonych(Nukleotyd nukleotyd) {
        for (Nukleotyd n : this.okresloneNukleotydy) {
            if (nukleotyd.getTyp() == n.getTyp()) {
                return true;
            }
        }

        return false;
    }

    @Override
    protected void mutuj() {
        Random generator = new Random();
        int losowaPozycja = this.okreslonePozycje[generator.nextInt(this.okreslonePozycje.length)];
        Nukleotyd doZamiany = this.kwasNukleinowy.getNukleotyd(losowaPozycja);
        Nukleotyd losowy = doZamiany;
        while (losowy.getTyp() == doZamiany.getTyp() || !jestWOkreslonych(losowy)) {
            losowy = Nukleotyd.getRandomNukleotyd();
        }
        this.kwasNukleinowy.setNukleotyd(losowaPozycja, losowy);
    }
}

