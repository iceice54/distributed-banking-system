public class BankResponse {
    public int methodId;
    public int reqId;
    public int status;
    public String message;

    // Optional fields (some might be null/0 depending on the method)
    public int accNum;
    public double balance;

    // Constructor for easy setup
    public BankResponse() {}
}