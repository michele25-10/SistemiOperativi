import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

class Sensor implements Runnable {
    private final AtomicBoolean running = new AtomicBoolean(false);
    PipedOutputStream pos;

    public Sensor(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        running.set(true);
        Random random = new Random();
        float temperatura;
        while (running.get()) {
            temperatura = 18 + random.nextFloat() * (21 - 18);

            try {
                BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pos));
                String result = temperatura + "";
                bw.write(result);
                bw.newLine();
                bw.flush();
            } catch (IOException e) {
                System.err.println("Errore scrittura su pipe: " + e.getMessage());
            }

            try {
                Thread.sleep(300);
            } catch (InterruptedException e) {
                System.out.println("Interrotto thread " + Thread.currentThread() + " di sensor");
            }
        }
    }

    public void stop() {
        running.set(false);
        Thread.currentThread().interrupt();
    }

}

class Actuator implements Runnable {
    private final int N = 10;
    private final float soglia;
    private PipedInputStream pis;
    private Sensor sensor;

    public Actuator(float soglia, PipedInputStream pis, Sensor sensor) {
        this.soglia = soglia;
        this.pis = pis;
        this.sensor = sensor;
    }

    public void run() {
        int i = 0;

        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(pis));
            while (i < N) {
                float tmp = Float.parseFloat(br.readLine());
                if (tmp < soglia) {
                    System.out.println("La temperatura è di " + tmp + " è necessario accendere il riscaldamento");
                } else {
                    System.out.println("La temperatura attuale" + tmp + " > " + soglia + " (soglia) ");
                }
                i++;
            }

            sensor.stop();

        } catch (IOException e) {
            System.out.println("Errore lettura su pipe" + e.getMessage());
        }

    }

}

public class Esercizio1 {
    public static void main(String[] args) {
        // Input valore soglia richiesto all'utente da tastiera
        float soglia = 0;
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            soglia = Float.parseFloat(br.readLine());
            br.close();
        } catch (Exception e) {
            System.err.println("Errore lettura del float da tastiera");
            System.exit(-1);
        }

        PipedInputStream pis = new PipedInputStream();
        try {
            PipedOutputStream pos = new PipedOutputStream(pis);

            // istanza oggetto Sensor e Thread
            Sensor s = new Sensor(pos);
            Thread st = new Thread(s);

            // istanza oggetto Actuator e Thread
            Actuator a = new Actuator(soglia, pis, s);
            Thread at = new Thread(a);

            // Avvio i thread
            st.start();
            at.start();

            at.join();
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
