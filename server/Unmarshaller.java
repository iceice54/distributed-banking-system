import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.logging.Logger;

public class Unmarshaller {

    private static final Logger logger = Logger.getLogger(Unmarshaller.class.getName());

    public static BankRequest unmarshall(byte[] payload) {
        final ByteBuffer buf = ByteBuffer.wrap(payload);
        final BankRequest req = new BankRequest();

        try {
            req.methodId = MethodId.fromInt(buf.getInt());
            req.reqId = buf.getInt();

            switch (req.methodId) {
                case OPEN_ACCOUNT:
                    req.name = getString(buf);
                    req.password = getString(buf);
                    req.currency = getCurrency(buf);
                    req.amount = buf.getDouble();
                    logger.info("Open account request from reqId: " + req.reqId);
                    break;
                case CLOSE_ACCOUNT:
                    req.name = getString(buf);
                    req.accNum = buf.getInt();
                    req.password = getString(buf);
                    logger.info("Close account request from reqId: " + req.reqId);
                    break;
                case WITHDRAW:
                case DEPOSIT:
                    req.name = getString(buf);
                    req.accNum = buf.getInt();
                    req.password = getString(buf);
                    req.currency = getCurrency(buf);
                    req.amount = buf.getDouble();
                    logger.info((req.methodId == MethodId.WITHDRAW ? "Withdraw" : "Deposit")
                            + " request from reqId: " + req.reqId);
                    break;
                case MONITOR:
                    req.monitorInterval = buf.getInt();
                    logger.info("Monitor request from reqId: " + req.reqId);
                    break;
                case CHECK_BALANCE:
                    req.accNum = buf.getInt();
                    logger.info("Check balance request from reqId: " + req.reqId);
                    break;
                case TRANSFER:
                    req.name = getString(buf);
                    req.accNum = buf.getInt();
                    req.password = getString(buf);
                    req.accNum2 = buf.getInt();
                    req.currency = getCurrency(buf);
                    req.amount = buf.getDouble();
                    logger.info("Transfer request from reqId: " + req.reqId);
                    break;
                default:
                    throw new IllegalArgumentException("Unknown methodId: " + req.methodId);
            }
        } catch (BufferUnderflowException e) {
            logger.warning("Malformed payload: " + e.getMessage());
            return req;
        } catch (IllegalArgumentException e) {
            logger.warning("Invalid request: " + e.getMessage());
            return req;
        }
        return req;
    }

    private static String getString(ByteBuffer buf) {
        final int len = buf.getInt();
        final byte[] bytes = new byte[len];
        buf.get(bytes);
        return new String(bytes, StandardCharsets.UTF_8);
    }

    private static Currency getCurrency(ByteBuffer buf) {
        final String raw = getString(buf);
        try {
            return Currency.valueOf(raw.trim().toUpperCase());
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("Unknown currency: " + raw, e);
        }
    }
}
