import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;

class Operatore extends Thread {
    private Magazzino mag;

    public Operatore(Magazzino mag) {
        this.mag = mag;
    }

    public void run() {
        int time = (int) (Math.random() * 1000);
        System.out.println(time);
        if (mag.existProduct("bulloni") > 0) {
            try {
                Thread.sleep(time);
            } catch (Exception e) {
                System.err.println(e.getMessage());
            }
            mag.addQtaToProduct("bulloni", 1000);
            mag.removeQtaToProduct("bulloni", 500);
        }
    }
}

class Magazzino {
    Map<String, Integer> mag;

    public Magazzino() {
        this.mag = new HashMap<String, Integer>();
    }

    /*
     * La parola chiave syncronizhed mette in coda gli oggetti che vogliono eseguire
     * il metodo e ci accedono uno alla volta
     */

    public synchronized int existProduct(String search) {

        if (mag.containsKey(search)) {
            return mag.get(search);
        }
        return 0;
    }

    public synchronized void addProduct(String name, int qta) {
        mag.put(name, qta);
    }

    public synchronized void addQtaToProduct(String name, int value) {
        int tot = value + existProduct(name);
        mag.put(name, tot);
    }

    public synchronized void removeQtaToProduct(String name, int value) {
        int tot = existProduct(name) - value;
        mag.put(name, tot);
    }

    public synchronized void print() {
        mag.forEach((name, qta) -> System.out.println(name + ": " + qta));
    }
}

public class Esercizio2 {
    public static void main(String[] args) {
        int nThread = Integer.parseInt(args[0]);

        Magazzino mag = new Magazzino();
        mag.addProduct("bulloni", 250);

        Operatore[] o = new Operatore[nThread];

        for (int i = 0; i < nThread; i++) {
            o[i] = new Operatore(mag);
            o[i].start();
        }

        for (int i = 0; i < nThread; i++) {
            try {
                // attesa del figlio
                o[i].join();
            } catch (InterruptedException e) {
                System.err.println(e.getMessage());
            }
        }

        mag.print();
    }
}
