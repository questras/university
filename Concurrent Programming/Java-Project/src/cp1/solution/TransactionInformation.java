package cp1.solution;

import cp1.base.ResourceId;
import cp1.base.ResourceOperation;

import java.util.ArrayList;
import java.util.HashMap;


// Class to hold information about transaction.
public class TransactionInformation {
    // Information whether the transaction is cancelled.
    private boolean cancelled;

    // Collection of successful operations on corresponding resources.
    private final HashMap<ResourceId, ArrayList<ResourceOperation>> successfulOperations;

    // Time when transaction started.
    private final long transactionStartTime;

    public TransactionInformation(long transactionStartTime) {
        this.cancelled = false;
        this.successfulOperations = new HashMap<>();
        this.transactionStartTime = transactionStartTime;
    }

    public boolean isCancelled() {
        return this.cancelled;
    }

    public void cancel() {
        this.cancelled = true;
    }

    public HashMap<ResourceId, ArrayList<ResourceOperation>> getSuccessfulOperations() {
        return this.successfulOperations;
    }

    public void putNewSuccessfulOperation(ResourceId rid, ResourceOperation operation) {
        if (!this.successfulOperations.containsKey(rid)) {
            this.successfulOperations.put(rid, new ArrayList<>());
        }

        this.successfulOperations.get(rid).add(operation);
    }

    public long getTransactionStartTime() {
        return this.transactionStartTime;
    }
}
