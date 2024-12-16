import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;
import java.lang.InterruptedException;
import java.nio.charset.Charset;

class SimulaSensore implements Runnable {
    private final AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream pos;

    public SimulaSensore(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        running.set(true);
        Random random = new Random();
        while (running.get()) {
            double temperatura = -10 + random.nextDouble() * 50; // 50 = (40 - (-10)) intervallo di estrazione

            String result = temperatura + "";
            byte buffer[] = result.getBytes();
            try {
                pos.write(buffer, 0, buffer.length);
                pos.flush();
            } catch (IOException e) {
                System.out.println("Errore scrittura su pipe " + e.getMessage());
            }

            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                System.out.println("SimulaSensore ha ricevuto una interrupted singal ");
            }
        }
    }

    public void termina() {
        running.set(false);
        try {
            pos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}

class MonitoraSensore implements Runnable {
    private PipedInputStream pis;

    public MonitoraSensore(PipedInputStream pis) {
        this.pis = pis;
    }

    public void run() {
        byte[] bytes = new byte[128];

        try {
            // leggere in input
            int n_read = 0;
            while ((n_read = pis.read(bytes, 0, bytes.length)) > 0) {
                double value = Double.parseDouble(new String(bytes, 0, n_read, Charset.forName("UTF-8")));

                // scrittura dell'output
                if (value < 5) {
                    System.out.println("La temperatura è sotto i 5°C: " + value);
                } else if (value > 35) {
                    System.out.println("La temperatura è sopra i 35°C: " + value);
                } else {
                    System.out.println("La temperatura è: " + value);
                }
            }
        } catch (IOException e) {
            System.out.println("Errore lettura: " + e.getMessage());
        }

    }
}

public class Esercizio4 {
    static public void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;

        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            System.out.println("Errore creazione pipe: " + e.getMessage());
            System.exit(-1);
        }

        MonitoraSensore ms = new MonitoraSensore(pis);
        Thread tms = new Thread(ms);

        SimulaSensore ss = new SimulaSensore(pos);
        Thread tss = new Thread(ss);

        tms.start();
        tss.start();

        try {
            Thread.sleep(30000);
        } catch (InterruptedException e) {
        }

        ss.termina();

        try {
            tms.join();
            tss.join();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
