import java.nio.*;

class project1 {
    public static void main (String args[]) {
        int array_size = 65536;
        Channel<int[]> channel = new MessageQueue<int[]>();
        final Producer producer = new Producer();
        final Consumer consumer = new Consumer();
        producer.run();
        consumer.run();
    }
}