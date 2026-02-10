import java.nio.ByteBuffer;

public class Marshaller {

    // Packs the response into a 16-byte array
    public static byte[] marshallResponse(int reqId, int status, double balance) {
        // Allocate exactly 16 bytes
        ByteBuffer buffer = ByteBuffer.allocate(16);

        buffer.putInt(reqId);    // 4 bytes
        buffer.putInt(status);   // 4 bytes
        buffer.putDouble(balance); // 8 bytes

        return buffer.array(); // Returns the raw byte[] to send via UDP
    }
}