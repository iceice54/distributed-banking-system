public class BankRequest {
    public MethodId methodId;
    public int reqId;

    // Optional fields (some might be null/0 depending on the method)
    public String name;
    public String password;
    public Currency currency;
    public int accNum;
    public int accNum2; // Used for transfers (accNum -> accNum2)
    public double amount;
    public int monitorInterval;

    // Constructor for easy setup
    public BankRequest() {}
}