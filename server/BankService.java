import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class BankService {

    private static final int STATUS_OK    = 1;
    private static final int STATUS_ERROR = 0;

    private static final Map<Integer, Account> accounts = new HashMap<>();
    private static int nextAccNum = 1000;

    public static BankResponse process(BankRequest req, InetAddress clientIP, int clientPort, DatagramSocket socket) {
        BankResponse resp = new BankResponse();
        resp.reqId = req.reqId;

        if (req.methodId == null) {
            resp.status  = STATUS_ERROR;
            resp.message = "Error: Malformed or unknown request";
            return resp;
        }

        if ((req.methodId == MethodId.OPEN_ACCOUNT
                || req.methodId == MethodId.WITHDRAW
                || req.methodId == MethodId.DEPOSIT)
                && req.currency == null) {
            resp.status  = STATUS_ERROR;
            resp.message = "Error: Invalid currency code";
            return resp;
        }

        // Handlers for all requests
        switch (req.methodId) {
            case OPEN_ACCOUNT: {
                accounts.put(nextAccNum, new Account(req.name, req.password, req.currency, req.amount));
                resp.status = STATUS_OK;
                resp.message = "Account created. Account number: " + nextAccNum + ". Balance: " + req.amount + " " + req.currency;
                MonitorManager.notifyClients(socket, Marshaller.marshallUpdate(
                        "Account opened: " + nextAccNum + ". Balance: " + req.amount + " " + req.currency));
                nextAccNum++;
                break;
            }
            case CLOSE_ACCOUNT: {
                if (!accounts.containsKey(req.accNum)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account number does not exist";
                    break;
                }
                final Account acc = accounts.get(req.accNum);
                if (!Objects.equals(acc.getName(), req.name)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account does not belong to user";
                    break;
                }
                if (!Objects.equals(acc.getPassword(), req.password)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Wrong password";
                    break;
                }
                accounts.remove(req.accNum);
                resp.status = STATUS_OK;
                resp.message = "Account with account number " + req.accNum + " closed.";
                MonitorManager.notifyClients(socket, Marshaller.marshallUpdate(
                        "Account with account number " + req.accNum + " closed."));
                break;
            }
            case WITHDRAW: {
                if (!accounts.containsKey(req.accNum)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account number does not exist";
                    break;
                }
                final Account acc = accounts.get(req.accNum);
                if (!Objects.equals(acc.getName(), req.name)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account does not belong to user";
                    break;
                }
                if (!Objects.equals(acc.getPassword(), req.password)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Wrong password";
                    break;
                }
                if (!Objects.equals(acc.getCurrency(), req.currency)) {
                    resp.status  = STATUS_ERROR;
                    resp.message = "Error: Invalid currency. Account currency is " + acc.getCurrency();
                    break;
                }
                if (acc.getBalance() < req.amount) {
                    resp.status  = STATUS_ERROR;
                    resp.message = "Error: Insufficient funds. Account balance: " + acc.getBalance() + " Requested amount: " + req.amount;
                    break;
                }
                acc.balance -= req.amount;
                resp.status  = STATUS_OK;
                resp.message = "Withdrawal of " + req.amount + " successful. New balance: " + acc.getBalance() + " " + acc.getCurrency();
                MonitorManager.notifyClients(socket, Marshaller.marshallUpdate(
                        "Account " + req.accNum + " withdrew " + req.amount + ". New balance: " + acc.getBalance() + " " + acc.getCurrency()));
                break;
            }
            case DEPOSIT: {
                if (!accounts.containsKey(req.accNum)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account number does not exist";
                    break;
                }
                final Account acc = accounts.get(req.accNum);
                if (!Objects.equals(acc.getName(), req.name)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account does not belong to user";
                    break;
                }
                if (!Objects.equals(acc.getPassword(), req.password)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Wrong password";
                    break;
                }
                if (!Objects.equals(acc.getCurrency(), req.currency)) {
                    resp.status  = STATUS_ERROR;
                    resp.message = "Error: Invalid currency. Account currency is " + acc.getCurrency();
                    break;
                }
                acc.balance += req.amount;
                resp.status  = STATUS_OK;
                resp.message = "Deposit of " + req.amount + " successful. New balance: " + acc.getBalance() + " " + acc.getCurrency();
                MonitorManager.notifyClients(socket, Marshaller.marshallUpdate(
                        "Account " + req.accNum + " deposited " + req.amount + ". New balance: " + acc.getBalance() + " " + acc.getCurrency()));
                break;
            }
            case MONITOR: {
                MonitorManager.register(clientIP, clientPort, req.monitorInterval);
                resp.status = STATUS_OK;
                resp.message = "Monitoring started for " + req.monitorInterval + "s";
                break;
            }
            case CHECK_BALANCE: {
                if (!accounts.containsKey(req.accNum)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Account number does not exist";
                    break;
                }
                final Account acc = accounts.get(req.accNum);
                resp.status  = STATUS_OK;
                resp.message = "Account owner: " + acc.getName() + ". Balance: " + acc.getBalance() + " " + acc.getCurrency();
                break;
            }
            case TRANSFER: {
                if (!accounts.containsKey(req.accNum)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Sending Account number does not exist";
                    break;
                }
                final Account acc = accounts.get(req.accNum);
                if (!Objects.equals(acc.getName(), req.name)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Sending Account does not belong to user";
                    break;
                }
                if (!Objects.equals(acc.getPassword(), req.password)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Wrong password";
                    break;
                }
                if (!Objects.equals(acc.getCurrency(), req.currency)) {
                    resp.status  = STATUS_ERROR;
                    resp.message = "Error: Invalid currency. Sending Account currency is " + acc.getCurrency();
                    break;
                }
                if (acc.getBalance() < req.amount) {
                    resp.status  = STATUS_ERROR;
                    resp.message = "Error: Insufficient funds. Sending Account balance: " + acc.getBalance() + " Requested amount: " + req.amount;
                    break;
                }
                if (!accounts.containsKey(req.accNum2)) {
                    resp.status = STATUS_ERROR;
                    resp.message = "Error: Receiving Account number does not exist";
                    break;
                }
                final Account acc2 = accounts.get(req.accNum2);
                if (!Objects.equals(acc2.getCurrency(), req.currency)) {
                    resp.status  = STATUS_ERROR;
                    resp.message = "Error: Currency mismatch. Sending Account currency is " + acc.getCurrency() + " but Receiving Account currency is " + acc2.getCurrency();
                    break;
                }
                acc.balance -= req.amount;
                acc2.balance += req.amount;
                resp.status = STATUS_OK;
                resp.message = "Transfer of " + req.amount + " " + req.currency + " successful. Sending Account balance: " + acc.getBalance() + " " + acc.getCurrency();
                MonitorManager.notifyClients(socket, Marshaller.marshallUpdate(
                        "Account " + req.accNum + " transferred " + req.amount + " " + req.currency + " to account " + req.accNum2));
                break;
            }
            default: {
                resp.status  = STATUS_ERROR;
                resp.message = "Error: Unknown method ID " + req.methodId;
            }
        }

        return resp;
    }
}
