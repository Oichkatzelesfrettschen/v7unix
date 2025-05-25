---- MODULE TicketLock ----
EXTENDS Naturals

CONSTANT N \* number of threads

VARIABLES next, serving, pc, ticket

vars == << next, serving, pc, ticket >>

Init ==
    /\ next = 0
    /\ serving = 0
    /\ pc = [i \in 0..N-1 |-> "start"]
    /\ ticket = [i \in 0..N-1 |-> -1]

Acquire(i) ==
    /\ pc[i] = "start"
    /\ ticket' = [ticket EXCEPT ![i] = next]
    /\ next' = next + 1
    /\ pc' = [pc EXCEPT ![i] = "wait"]
    /\ UNCHANGED serving

Wait(i) ==
    /\ pc[i] = "wait"
    /\ ticket[i] = serving
    /\ pc' = [pc EXCEPT ![i] = "cs"]
    /\ UNCHANGED << next, serving, ticket >>

Release(i) ==
    /\ pc[i] = "cs"
    /\ pc' = [pc EXCEPT ![i] = "start"]
    /\ serving' = serving + 1
    /\ UNCHANGED << next, ticket >>

Next ==
    \/ \E i \in 0..N-1: Acquire(i)
    \/ \E i \in 0..N-1: Wait(i)
    \/ \E i \in 0..N-1: Release(i)

TypeOK ==
    /\ next \in Nat
    /\ serving \in Nat
    /\ next >= serving
    /\ pc \in [0..N-1 -> {"start", "wait", "cs"}]
    /\ ticket \in [0..N-1 -> Nat]

NoDoubleCS ==
    \A i \in 0..N-1: pc[i] = "cs" => \A j \in 0..N-1: j = i \/ pc[j] # "cs"

Fairness ==
    \A i \in 0..N-1: pc[i] = "wait" => <> (pc[i] = "cs")

Spec == Init /\ [][Next]_vars /\ \A i \in 0..N-1: WF_vars(Wait(i))

====
