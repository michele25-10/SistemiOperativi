
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.HashMap;
import java.util.Map;

import java.lang.*;

class ThreadLoad {
    private Map<Integer, Double> load;

    public ThreadLoad() {
        this.load = new HashMap<Integer, Double>();
    }

    public synchronized void addLoad(int idThread, double cpuLoad) {
        this.load.put(idThread, cpuLoad);
    }

    public synchronized Map<Integer, Double> getLoad() {
        return this.load;
    }

}

class Sorter implements Runnable {
    PipedOutputStream pos;
    ThreadLoad tl;

    public Sorter(ThreadLoad tl, PipedOutputStream pos) {
        this.tl = tl;
        this.pos = pos;
    }

    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            try {
                Thread.sleep(150);
            } catch (InterruptedException e) {
                // Ripristina lo stato di interruzione dopo l'eccezione
                Thread.currentThread().interrupt();
            }
            Map<Integer, Double> load = tl.getLoad();
            int maxId[] = { 0 };
            double maxCpuLoad[] = { 0 };

            load.forEach((key, value) -> {
                if (value > maxCpuLoad[0]) {
                    maxId[0] = key;
                    maxCpuLoad[0] = value;
                }
            });
            try {
                BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pos));
                String toSend = maxId[0] + ", " + maxCpuLoad[0];
                bw.write(toSend);
                bw.newLine();
                bw.flush();
            } catch (IOException e) {
                try {
                    pos.close();
                } catch (Exception err) {
                    err.printStackTrace();
                }
                e.printStackTrace();
            }
        }
    }
}

class Monitor implements Runnable {
    ThreadLoad tl;

    public Monitor(ThreadLoad tl) {
        this.tl = tl;
    }

    public void run() {
        double cpuLoad;
        int idThread;
        while (!Thread.currentThread().isInterrupted()) {
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                // Ripristina lo stato di interruzione dopo l'eccezione
                Thread.currentThread().interrupt();
            }

            cpuLoad = Math.random() * 100;
            idThread = (int) (Math.random() * 10);

            tl.addLoad(idThread, cpuLoad);
        }
    }
}

class Manager implements Runnable {
    PipedInputStream pis;
    Thread sorter, monitor;

    public Manager(PipedInputStream pis, Thread sorter, Thread monitor) {
        this.pis = pis;
        this.sorter = sorter;
        this.monitor = monitor;
    }

    public void run() {
        int count = 0;
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(pis));
            String line = br.readLine();
            while (line != null && count < 10) {
                System.out.println((count + 1) + "Contenuto risposta: " + line);
                line = br.readLine();
                count++;
            }
            br.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            sorter.interrupt();
            monitor.interrupt();
            Thread.currentThread().interrupt();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}

public class Esercizio2 {
    static public void main(String[] args) {
        ThreadLoad tl = new ThreadLoad();
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = new PipedOutputStream();
        try {
            pis.connect(pos);
        } catch (IOException e) {
            e.printStackTrace();
        }

        Thread monitor = new Thread(new Monitor(tl));
        Thread sorter = new Thread(new Sorter(tl, pos));
        Thread manager = new Thread(new Manager(pis, sorter, monitor));

        sorter.start();
        monitor.start();
        manager.start();

        try {
            manager.join();
            monitor.join();
            sorter.join();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
