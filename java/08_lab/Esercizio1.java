import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.concurrent.atomic.AtomicBoolean;

class WorkerThread extends Thread {
    private int id;
    private final AtomicBoolean running = new AtomicBoolean(false);

    public WorkerThread(int id) {
        this.id = id;
    }

    public void stopThread() {
        running.set(false);
    }

    public boolean isRunning() {
        return running.get();
    }

    public void run() {
        running.set(true);

        while (running.get()) {
            try {
                System.out.println("Sono il thread " + currentThread() + " con ID: " + this.id);
                Thread.sleep(1000);
            } catch (InterruptedException e) {

            }
        }
    }

    // Permette di non attendere la terminazione della sleep,
    // Bensì termina immediatamaente il thread senza attendere la fine della sleep
    public void interrupt() {
        running.set(false);
        Thread.currentThread().interrupt();
    }

}

public class Esercizio1 {
    public static void main(String[] args) {
        int nThreads = Integer.parseInt(args[0]);
        WorkerThread wt[] = new WorkerThread[nThreads];

        for (int i = 0; i < nThreads; i++) {
            wt[i] = new WorkerThread(i);
            wt[i].start();
        }

        System.out.println("Per semplicità l\'id del thread sarà un numero compreso tra 0 e nThread - 1");

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        try {
            String line = br.readLine();
            while (line != null && checkRunningThreads(wt) > 0) {
                try {
                    int i = Integer.parseInt(line);
                    if (i < 0 && i >= nThreads) {
                        throw new Exception("Il numero inserito deve essere compreso tra  0 e nThread - 1");
                    }
                    if (!wt[i].isRunning()) {
                        throw new Exception("Thread ha già concluso");
                    }
                    wt[i].stopThread();
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                } finally {
                    line = br.readLine();
                }
            }
        } catch (Exception e) {
            System.out.println(e.getStackTrace());
        }
    }

    public static int checkRunningThreads(WorkerThread wt[]) {
        int counter = 0;
        for (int i = 0; i < wt.length; i++) {
            if (wt[i].isRunning()) {
                counter++;
            }
        }
        return counter;
    }
}
