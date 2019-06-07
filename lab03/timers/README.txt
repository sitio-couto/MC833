How to run the tests:

cd ..

Run the server with this command:
java engine/ComputeEngine -Djava.rmierver.hostname=localhost > timers/s(n)_out.txt

Run the client with this command:
java client/Client localhost timers/c(n)_out.txt < timers/case(n)_in.txt

Change (n) to match with the operation number.
