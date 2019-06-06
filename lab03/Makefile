# variables
CC=javac
JAR=jar cvf
RUN=java
RM=rm -f
HOSTNAME=localhost
CODEBASE_INTERNAL=compute/compute.jar
CODEBASE_EXTERNAL=compute/compute.jar
REGISTRY=rmiregistry

all: compute engine client

compute: compute/Compute.java
	$(CC) compute/Compute.java
	$(JAR) compute/compute.jar compute/*.class

engine: engine/ComputeEngine.java engine/Profile.java engine/DataGen.java compute/compute.jar
	$(CC) -cp compute/compute.jar engine/ComputeEngine.java engine/Profile.java engine/DataGen.java

client: client/*.java
	$(CC) -cp compute/compute.jar client/*.java

clean:
	$(RM) compute/compute.jar
	$(RM) compute/*.class
	$(RM) engine/*.class
	$(RM) client/*.class

registry:
	$(REGISTRY) &

run-server:
	$(RUN) -cp engine:compute/compute.jar -Djava.rmi.server.codebase=$(CODEBASE_INTERNAL) -Djava.rmi.server.hostname=$(HOSTNAME) -Djava.security.policy=server.policy engine/ComputeEngine

run-client:
	$(RUN) -cp client:compute/compute.jar -Djava.rmi.server.codebase=$(CODEBASE_EXTERNAL) -Djava.security.policy=client.policy client/Client $(HOSTNAME) 45
