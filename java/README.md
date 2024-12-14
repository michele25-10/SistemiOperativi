# Thread in JAVA

- Implementare interfaccia Runnable --> implements

1. Definire una classe che implementi Runnable, quindi definendone il metodo run()
2. Creare una istanza di tale classe
3. Creare un'istanza della classe Thread, passando al costruttore un reference all'oggetto Runnable creato precedentemente
4. Invocare il metodo start() sull'oggetto Thread appena creato.

- Estendere classe Thread --> extends

1. Definire una sottoclasse della classe Thread, facendo un opportuno override del metodo run()
2. Creare un’istanza di tale sottoclasse.
3. Invocare il metodo start() su tale istanza, la quale a sua volta richiamerà il metodo run() in un thrad separato.

# Comunicazione tra thread

- Serializzazione --> non approfondita in questo corso
- Pipe

  - PipedOutputStream
  - PipedInputStream

  ```java
    PipedInputStream pis = new PipedInputStream();
    try {
        PipedOutputStream pos = new PipedOutputStream(pip);

        //Oppure per concatenare due pipe
        pis.connect(pos);

    } catch (Exception e) {
        e.printStackTrace();
    }

    BufferedReader br = new BufferedReader(new InputStreamReader(pis));
    BufferedWriter br = new BufferedWriter(new OutputStreamWriter(pos));
  ```

# Metodi utili della classe Thread

```java
    Thread.currentThread().sleep(1000);     //Ferma un processo per tot ms
    Thread.currentThread().join();          //Attende la terminazione di quel thread
    Thread.currentThread().interrupt();     //Forzo un thread a terminare

    while(!Thread.currentThread().isInterrupted()){
        /* Esegue il corpo di questo ciclo finchè ad un
        thread non viene inviato un segnale di interrupt*/
    }


```

# Implementazione di metodi thread-safe

Un metodo thread-safe è un metodo a cui hanno accesso i processi uno alla volta generando quindi code FIFO per ogni metodo.

```java
    public class Informazioni{
        private String blabla = "blablabla";

        public Informazioni(String blabla){
            this.blabla = blabla;
        }

        /* Metodo Thread-safe */
        public synchronized void printBlaBla(){
            System.out.println(blabla);
        }
    }
```

# Variabili atomiche

Le variabili atomiche in Java fanno parte del pacchetto java.util.concurrent.atomic e sono utilizzate per garantire operazioni di lettura e scrittura sicure in contesti multithreading senza bisogno di utilizzare esplicitamente la sincronizzazione con synchronized.

Esempio:

```java
public class WorkerThreadMetodo2 implements Runnable {

	// AtomicBoolean, "A boolean value that may be updated atomically"
	// to prevent conflicts in setting and checking the variable from different threads
	// https://docs.oracle.com/javase/7/docs/api/java/util/concurrent/atomic/AtomicBoolean.html
    private final AtomicBoolean running = new AtomicBoolean(false);

	private int i;
	public WorkerThreadMetodo2(int i){
		this.i = i;
	}

    // stop the thread
    public void stop() {
        running.set(false);
    }

    public void interrupt() {
        running.set(false);
        Thread.currentThread().interrupt();
    }

    public void run() {
        running.set(true);
        while(running.get()) {
            try {
				System.out.println("Thread: " + i);
                Thread.sleep(1000);
            }
			catch(InterruptedException e) {
                System.out.println(Thread.currentThread() + " interrupted, terminating...");
            }
        }
    }
}
```

Come si può notare in questo esempio la variabile AtomicBoolean consente di fermare il processo e di fare gli opportuni while senza usare Thread.currentThread().isInterrupted();

# Suggerimenti del tutor per affrontare l'esame

### Main

- istanzio oggetti comuni, thread e pipedStream
- aspetto comunicazioni o fine esecuzione
- join() --> attendo chiusura processo in un try catch

### Oggetti thread

- creati da MAIN
- eseguono qualcosa
- comunicano col MAIN quando hanno finito

### Oggetti comuni

- istanziati da MAIN, non messi su thread
- offrono metodi pubblici syncronizhed o variabili Atomic per variabili shared

# Codice Java ripetuto negli esercizi

```java
    // Lettura da tastiera di un intero
    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    int id = Integer.parseInt(br.readLine());
    br.close();

    /*Scrittura su una pipe*/
    BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pop));
        while(running.get()) {
            try {
                String result = "Thread " + Thread.currentThread() + " con cpuLoad: " + (int)(Math.random() * 100);
                bw.write(result);
                bw.newLine();
                bw.flush();

                Thread.sleep(150);
            } catch(InterruptedException | IOException e) {}
        }

    /* Lettura da una pipe */
    BufferedReader br = new BufferedReader(new InputStreamReader(pip));
    while (i < 10) {
        try {
		    String line = br.readLine();
			System.out.println(line);
        } catch(IOException e) {}
        i++;
    }

```
