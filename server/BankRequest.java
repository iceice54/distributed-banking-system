public class BankRequest {
    public MethodId methodId;
    public int reqId;

    public String name;
    public String password;
    public Currency currency;
    public int accNum;
    public int accNum2; // Used for transfers (accNum -> accNum2)
    public double amount;
    public int monitorInterval;

    public BankRequest() {}
}