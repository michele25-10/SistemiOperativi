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
    PipedOutputStream pos = new PipedOutputStream();
    PipedInputStream pis = new PipedInputStream(pos);

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
    private final AtomicBoolean running = new AtomicBoolean(false);
```
