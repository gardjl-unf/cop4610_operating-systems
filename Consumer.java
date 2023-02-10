public class Consumer implements Runnable {
    public void run() {
        try {
            while (true) {
                System.out.println("Consumed: " + SharedQueue.take());
                Thread.sleep(1000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
