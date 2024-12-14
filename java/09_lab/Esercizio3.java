import java.util.Random;

class Produzione {
    private int qta;

    public Produzione() {
        this.qta = 0;
    }

    public synchronized void incremento() {

        if (qta % 4 == 0) {
            try {
                Thread.currentThread().sleep(200);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        qta++;
    }

    public synchronized void stampaProduzione() {
        System.out.println("Pezzi prodotti: " + qta);
    }
}

class Macchina extends Thread {
    public static final int N = 35;
    private Produzione prod;

    public Macchina(Produzione prod) {
        this.prod = prod;
    }

    public void run() {
        int i = 0;
        Random random = new Random();
        while (i < N) {
            int pause = 500 + (random.nextInt(100));
            try {
                Thread.sleep(pause);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            prod.incremento();
            i++;
        }
    }
}

public class Esercizio3 {
    public static void main(String[] args) {
        Produzione p = new Produzione();
        Macchina m[] = new Macchina[10];

        for (int i = 0; i < 10; i++) {
            m[i] = new Macchina(p);
        }

        // Avvio tutti i thread
        for (int i = 0; i < 10; i++) {
            m[i].start();
        }

        // Mi metto in attesa della fine di tutti i thread
        try {
            for (int i = 0; i < 10; i++) {
                m[i].join();
            }
            p.stampaProduzione();
        } catch (Exception e) {
            System.err.println("Errore nell'attesa della fine dei thread: " + e.getMessage());
            System.exit(-1);
        }

    }
}
