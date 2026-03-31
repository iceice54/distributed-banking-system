import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class Marshaller {

    // Method for marshalling all responses except monitor updates
    public static byte[] marshall(BankResponse resp) {
        final String message  = resp.message != null ? resp.message : "";
        final byte[] msgBytes = message.getBytes(StandardCharsets.UTF_8);
        final int totalSize   = Integer.BYTES + Integer.BYTES + Integer.BYTES + msgBytes.length;
        final ByteBuffer buf  = ByteBuffer.allocate(totalSize);

        buf.putInt(resp.reqId);
        buf.putInt(resp.status);
        buf.putInt(msgBytes.length);
        buf.put(msgBytes);

        return buf.array();
    }

    // Method for marshalling monitor updates
    public static byte[] marshallUpdate(String message) {
        final byte[] msgBytes = message.getBytes(StandardCharsets.UTF_8);
        final int totalSize   = Integer.BYTES + msgBytes.length;
        final ByteBuffer buf  = ByteBuffer.allocate(totalSize);

        buf.putInt(msgBytes.length);
        buf.put(msgBytes);

        return buf.array();
    }
}