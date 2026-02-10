import java.nio.ByteBuffer;
import java.util.Arrays;

public class Unmarshaller {

    // Returns a "Request Object" (you can define a simple class for this)
    public static BankRequest unmarshall(byte[] data) {
        // Wrap the byte array to easily read integers and doubles
        ByteBuffer buffer = ByteBuffer.wrap(data);

        // Java reads Big-Endian by default, matching C++'s htonl()
        int methodId = buffer.getInt();   // Offset 0
        int reqId    = buffer.getInt();   // Offset 4
        int accId    = buffer.getInt();   // Offset 8
        double amount = buffer.getDouble(); // Offset 12

        return new BankRequest(methodId, reqId, accId, amount);
    }
}

// Simple helper class to hold the data
class BankRequest {
    public int methodId, reqId, accId;
    public double amount;

    public BankRequest(int m, int r, int a, double amt) {
        this.methodId = m; this.reqId = r; this.accId = a; this.amount = amt;
    }
}