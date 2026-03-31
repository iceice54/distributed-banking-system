public enum MethodId {
    OPEN_ACCOUNT(1),
    CLOSE_ACCOUNT(2),
    WITHDRAW(3),
    DEPOSIT(4),
    MONITOR(5),
    CHECK_BALANCE(6),
    TRANSFER(7);

    public final int value;

    MethodId(int value) {
        this.value = value;
    }

    public static MethodId fromInt(int value) {
        for (MethodId id : values()) {
            if (id.value == value) return id;
        }
        throw new IllegalArgumentException("Unknown methodId: " + value);
    }
}