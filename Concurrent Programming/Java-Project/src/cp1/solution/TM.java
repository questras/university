package cp1.solution;

import cp1.base.*;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;


public class TM implements TransactionManager {
    private final ConcurrentHashMap<ResourceId, Resource> resources;
    private final ConcurrentHashMap<ResourceId, Long> resourcesToOwners;
    private final ConcurrentHashMap<ResourceId, Semaphore> resourceToSemaphore;

    private final ConcurrentHashMap<Long, TransactionInformation> transactionToInformation;
    private final ConcurrentHashMap<Long, ResourceId> waitingTransactionToResourceId;

    private final LocalTimeProvider timeProvider;
    private final Semaphore mutex;

    public TM(Collection<Resource> resources, LocalTimeProvider timeProvider) {
        this.timeProvider = timeProvider;
        this.resources = new ConcurrentHashMap<>();
        this.resourcesToOwners = new ConcurrentHashMap<>();
        this.transactionToInformation = new ConcurrentHashMap<>();
        this.mutex = new Semaphore(1, true);
        this.resourceToSemaphore = new ConcurrentHashMap<>();
        this.waitingTransactionToResourceId = new ConcurrentHashMap<>();

        for (Resource r : resources) {
            this.resources.put(r.getId(), r);
            this.resourceToSemaphore.put(r.getId(), new Semaphore(1, true));
        }
    }

    @Override
    public void startTransaction(

    ) throws AnotherTransactionActiveException {
        long currentThreadId = Thread.currentThread().getId();

        if (this.transactionToInformation.containsKey(currentThreadId)) {
            throw new AnotherTransactionActiveException();
        }

        long currentTime = this.timeProvider.getTime();
        this.transactionToInformation.put(currentThreadId, new TransactionInformation(currentTime));
    }

    // Break a cycle of waiting threads leading to deadlock starting from
    // current thread if there is risk of deadlock.
    private void breakCycleIfNeeded() {
        long currentThreadId = Thread.currentThread().getId();
        ArrayList<Long> chainOfWaitingThreads = new ArrayList<>();

        long currentLookedUpThread = currentThreadId;
        chainOfWaitingThreads.add(currentLookedUpThread);


        boolean cycleFound = false;
        ResourceId currentlyWantedRId;
        while (this.waitingTransactionToResourceId.containsKey(currentLookedUpThread)) {
            currentlyWantedRId = this.waitingTransactionToResourceId.get(currentLookedUpThread);
            if (this.resourcesToOwners.containsKey(currentlyWantedRId)) {
                currentLookedUpThread = this.resourcesToOwners.get(currentlyWantedRId);
            }
            else {
                // Current resource doesn't have owner so there is no cycle.
                break;
            }

            if (currentLookedUpThread == currentThreadId) {
                // Chain of waiting threads led to the starting thread, so there is a cycle.
                cycleFound = true;
                break;
            }

            chainOfWaitingThreads.add(currentLookedUpThread);
        }

        if (cycleFound) {
            long latestTime = Long.MIN_VALUE;
            long blockingThreadId = chainOfWaitingThreads.get(0);

            for (long threadId : chainOfWaitingThreads) {
                long threadTime = this.transactionToInformation.get(threadId).getTransactionStartTime();

                if (threadTime > latestTime) {
                    latestTime = threadTime;
                    blockingThreadId = threadId;
                }
                else if (threadTime == latestTime) {
                    if (blockingThreadId < threadId) {
                        blockingThreadId = threadId;
                    }
                }
            }

            // Cancel thread in order to prevent deadlock.
            this.waitingTransactionToResourceId.remove(blockingThreadId);
            this.transactionToInformation.get(blockingThreadId).cancel();

            Set<Thread> setOfThreads = Thread.getAllStackTraces().keySet();
            for (Thread t : setOfThreads) {
                if (t.getId() == blockingThreadId) {
                    t.interrupt();
                }
            }

        }
    }

    @Override
    public void operateOnResourceInCurrentTransaction(
            ResourceId rid,
            ResourceOperation operation
    ) throws
            NoActiveTransactionException,
            UnknownResourceIdException,
            ActiveTransactionAborted,
            ResourceOperationException,
            InterruptedException {

        long currentThreadId = Thread.currentThread().getId();

        if (!this.transactionToInformation.containsKey(currentThreadId)) {
            throw new NoActiveTransactionException();
        }
        if (!this.resources.containsKey(rid)) {
            throw new UnknownResourceIdException(rid);
        }
        if (this.transactionToInformation.get(currentThreadId).isCancelled()) {
            throw new ActiveTransactionAborted();
        }

        this.mutex.acquire();

        if (this.resourcesToOwners.containsKey(rid) &&
                this.resourcesToOwners.get(rid) == currentThreadId) {
            // Current thread is owner of this resource.
            this.mutex.release();
        }
        else {
            this.waitingTransactionToResourceId.put(currentThreadId, rid);
            breakCycleIfNeeded();

            if (Thread.currentThread().isInterrupted()) {
                // Thread was interrupted during looking for cycle.

                if (this.transactionToInformation.get(currentThreadId).isCancelled()) {
                    this.mutex.release();
                    throw new ActiveTransactionAborted();
                }

                this.mutex.release();
                throw new InterruptedException();
            }
            this.mutex.release();

            // Wait for wanted resource.
            try {
                this.resourceToSemaphore.get(rid).acquire();
            }
            catch (InterruptedException e) {
                if (this.transactionToInformation.get(currentThreadId).isCancelled()) {
                    throw new ActiveTransactionAborted();
                }

                throw e;
            }
            finally {
                // Remove current thread from waiting queue.
                this.mutex.acquire();
                this.waitingTransactionToResourceId.remove(currentThreadId);
                this.mutex.release();
            }

            // Semaphore was successfully acquired so set current
            // thread as resource owner.
            this.resourcesToOwners.put(rid, currentThreadId);
        }

        // Current thread has access to resource.
        operation.execute(this.resources.get(rid));
        this.transactionToInformation.get(currentThreadId).putNewSuccessfulOperation(rid, operation);
    }

    // Remove all information about current transaction.
    private void cleanAfterTransaction() {
        long currentThreadId = Thread.currentThread().getId();

        for (ResourceId rid : this.resourcesToOwners.keySet()) {
            if (this.resourcesToOwners.get(rid) == currentThreadId) {
                this.resourcesToOwners.remove(rid);
                this.resourceToSemaphore.get(rid).release();
            }
        }
        this.transactionToInformation.remove(currentThreadId);
    }

    @Override
    public void commitCurrentTransaction(

    ) throws NoActiveTransactionException, ActiveTransactionAborted {
        long currentThreadId = Thread.currentThread().getId();

        if (!this.transactionToInformation.containsKey(currentThreadId)) {
            throw new NoActiveTransactionException();
        }

        if (this.transactionToInformation.get(currentThreadId).isCancelled()) {
            throw new ActiveTransactionAborted();
        }

        // Commit cannot be interrupted.
        this.mutex.acquireUninterruptibly();
        this.cleanAfterTransaction();
        this.mutex.release();
    }

    @Override
    public void rollbackCurrentTransaction() {
        long currentThreadId = Thread.currentThread().getId();

        if (this.transactionToInformation.containsKey(currentThreadId)) {
            // Undo all successful operations.
            HashMap<ResourceId, ArrayList<ResourceOperation>> successfulOperations =
                    this.transactionToInformation.get(currentThreadId).getSuccessfulOperations();
            for (ResourceId rid : successfulOperations.keySet()) {
                Resource resource = this.resources.get(rid);
                ArrayList<ResourceOperation> operations = successfulOperations.get(rid);

                for (int i = operations.size() - 1; i >= 0; i--) {
                    operations.get(i).undo(resource);
                }
            }

            // Rollback cannot be interrupted.
            this.mutex.acquireUninterruptibly();
            this.cleanAfterTransaction();
            this.mutex.release();
        }
    }

    @Override
    public boolean isTransactionActive() {
        return this.transactionToInformation.containsKey(Thread.currentThread().getId());
    }

    @Override
    public boolean isTransactionAborted() {
        Thread currentThread = Thread.currentThread();
        return this.transactionToInformation.containsKey(currentThread.getId()) &&
                this.transactionToInformation.get(currentThread.getId()).isCancelled();
    }
}
