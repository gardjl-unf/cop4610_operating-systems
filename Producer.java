public class Producer implements Runnable {
    public void run() {
        try {
            while (true) {
                SharedQueue.put("Hello");
                Thread.sleep(1000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
