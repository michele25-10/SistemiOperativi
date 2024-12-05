class Accumulatore {
    private double counter;

    public Accumulatore() {
        this.counter = 0.0;
    }

    public Accumulatore(double x) {
        this.counter = x;
    }

    public void addValue(double value) {
        this.counter += value;
    }

    public double getValue() {
        return this.counter;
    }
}

class CounterThread implements Runnable {
    private Accumulatore accumulatore;

    public CounterThread(Accumulatore accumulatore) {
        this.accumulatore = accumulatore;
    }

    public void run() {
        double value = (double) (Math.random() * 10);
        accumulatore.addValue(value);
        System.out.println("Thread " + Thread.currentThread() + "accumulatore: " + accumulatore.getValue());
    }
}

public class Esercizio1 {
    public static void main(String[] args) {
        Accumulatore acc = new Accumulatore();
        int nThread = 0;
        try {
            nThread = Integer.parseInt(args[0]);
        } catch (Exception e) {
            System.err.println(e);
        }

        Thread ct[] = new Thread[nThread];
        for (int i = 0; i < nThread; i++) {
            ct[i] = new Thread(new CounterThread(acc)); // Creo un thread
            ct[i].start();
        }

        for (int i = 0; i < nThread; i++) {
            try {
                ct[i].join(); // aspetto un thread alla volta
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Valore accumulatore: " + acc.getValue());
    }
}