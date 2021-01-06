package cover;

import java.util.ArrayList;


enum KrajPochodzenia {
    POLSKA, NIEMCY, CZECHY, UKRAINA;
}


class Przedmiot {
    private final int cena;
    private final KrajPochodzenia krajPochodzenia;

    public Przedmiot(int cena, KrajPochodzenia krajPochodzenia) {
        this.cena = cena;
        this.krajPochodzenia = krajPochodzenia;
    }

    public int getCena() {
        return cena;
    }

    public KrajPochodzenia getKrajPochodzenia() {
        return krajPochodzenia;
    }

    public String toString() {
        return "Przedmiot o cenie " + this.cena + " z " + this.krajPochodzenia;
    }
}


abstract class Uczestnik {
    private int budzet;
    private final String imie;

    public Uczestnik(int budzet, String imie) {
        this.budzet = budzet;
        this.imie = imie;
    }

    public String getImie() {
        return imie;
    }

    private boolean czyKupic(Przedmiot przedmiot) {
        if (przedmiot.getCena() > this.budzet) {
            return false;
        }

        return this.sprawdzPreferencje(przedmiot);
    }

    public boolean kup(Przedmiot przedmiot) {
        if (czyKupic(przedmiot)) {
            this.budzet -= przedmiot.getCena();
            return true;
        }

        return false;
    }

    protected abstract boolean sprawdzPreferencje(Przedmiot przedmiot);
}

/**
 * SENTYMENTALNI
 */

class Sentymentalny extends Uczestnik {
    private final KrajPochodzenia krajPochodzenia;

    public Sentymentalny(int budzet, String imie, KrajPochodzenia krajPochodzenia) {
        super(budzet, imie);
        this.krajPochodzenia = krajPochodzenia;
    }

    @Override
    protected boolean sprawdzPreferencje(Przedmiot przedmiot) {
        return this.krajPochodzenia == przedmiot.getKrajPochodzenia();
    }
}


class SentymentalnyZOgraniczeniem extends Sentymentalny {
    private final int limitPrzedmiotow;
    private int iloscPrzedmiotow;

    public SentymentalnyZOgraniczeniem(int budzet, String imie, KrajPochodzenia krajPochodzenia, int limitPrzedmiotow) {
        super(budzet, imie, krajPochodzenia);
        this.limitPrzedmiotow = limitPrzedmiotow;
        this.iloscPrzedmiotow = 0;
    }

    @Override
    protected boolean sprawdzPreferencje(Przedmiot przedmiot) {
        return super.sprawdzPreferencje(przedmiot) &&
                this.iloscPrzedmiotow < this.limitPrzedmiotow;
    }

    @Override
    public boolean kup(Przedmiot przedmiot) {
        if (super.kup(przedmiot)) {
            this.iloscPrzedmiotow++;
            return true;
        }

        return false;
    }
}

/**
 * OSZCZĘDNI
 */

abstract class Usredniacz extends Uczestnik {
    private final ArrayList<Integer> cenyPokazanych;
    private final int procentOdSredniej;

    public Usredniacz(int budzet, String imie, int procentOdSredniej) {
        super(budzet, imie);
        this.cenyPokazanych = new ArrayList<>();
        this.procentOdSredniej = procentOdSredniej;
    }

    private double policzSrednia() {
        double srednia = 0;
        for (int cena : this.cenyPokazanych) {
            srednia += cena;
        }

        return srednia / this.cenyPokazanych.size();
    }

    private double dajBrzegowaCene() {
        double srednia = this.policzSrednia();
        return (1.0 - (this.procentOdSredniej / 100.0)) * srednia;
    }

    @Override
    protected boolean sprawdzPreferencje(Przedmiot przedmiot) {
        if (this.cenyPokazanych.size() == 0) {
            this.cenyPokazanych.add(przedmiot.getCena());
            return false;
        }

        double brzegowaCena = this.dajBrzegowaCene();
        this.cenyPokazanych.add(przedmiot.getCena());

        return przedmiot.getCena() < brzegowaCena;
    }
}

class Oszczedny extends Usredniacz {
    public Oszczedny(int budzet, String imie) {
        super(budzet, imie, 0);
    }
}

class BardzoOszczedny extends Usredniacz {
    public BardzoOszczedny(int budzet, String imie) {
        super(budzet, imie, 10);
    }
}


public class Main {

    public static void main(String[] args) {
        ArrayList<Przedmiot> p = new ArrayList<>();
        p.add(new Przedmiot(10, KrajPochodzenia.POLSKA));
        p.add(new Przedmiot(5, KrajPochodzenia.CZECHY));
        p.add(new Przedmiot(7, KrajPochodzenia.POLSKA));
        p.add(new Przedmiot(12, KrajPochodzenia.NIEMCY));
        p.add(new Przedmiot(15, KrajPochodzenia.NIEMCY));
        p.add(new Przedmiot(3, KrajPochodzenia.UKRAINA));

        ArrayList<Uczestnik> u = new ArrayList<>();
        u.add(new Sentymentalny(20, "sentymentalny", KrajPochodzenia.POLSKA));
        u.add(new SentymentalnyZOgraniczeniem(30, "sentymentalnyZOgr", KrajPochodzenia.POLSKA, 1));
        u.add(new Oszczedny(15, "oszczedny"));
        u.add(new BardzoOszczedny(15, "bardzooszczedny"));

        for (Uczestnik uczestnik : u) {
            for (Przedmiot przedmiot : p) {
                if (uczestnik.kup(przedmiot)) {
                    System.out.println(uczestnik.getImie() + " kupil " + przedmiot.toString());
                }
                else {
                    System.out.println(uczestnik.getImie() + " nie kupil " + przedmiot.toString());
                }
            }
        }

    }
}
