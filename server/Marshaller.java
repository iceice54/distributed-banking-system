import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class Marshaller {

    public static BankRequest unmarshall(byte[] payload) {
        //TODO
        return BankRequest();
    }
    // Packs a simple Success/Fail response with a message
    public static byte[] packResponse(int reqId, int status, String message) {
        byte[] msgBytes = message.getBytes(StandardCharsets.UTF_8);

        // Calculate size: 4 (ReqID) + 4 (Status) + 4 (StrLen) + N (StrBytes)
        int totalSize = 4 + 4 + 4 + msgBytes.length;

        ByteBuffer buf = ByteBuffer.allocate(totalSize);

        buf.putInt(reqId);
        buf.putInt(status); // e.g., 1=Success, 0=Fail

        // Pack String
        buf.putInt(msgBytes.length);
        buf.put(msgBytes);

        return buf.array();
    }

    // Packs a response that includes a Balance (for Deposit/Withdraw)
    public static byte[] packBalanceResponse(int reqId, int status, double balance) {
        ByteBuffer buf = ByteBuffer.allocate(16); // Fixed size is fine here if no string

        buf.putInt(reqId);
        buf.putInt(status);
        buf.putDouble(balance);

        return buf.array();
    }
}