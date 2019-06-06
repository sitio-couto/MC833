# variables
CC=javac
JAR=jar cvf
RUN=java
RM=rm -f
HOSTNAME=localhost
CODEBASE_INTERNAL=compute/compute.jar
CODEBASE_EXTERNAL=compute/compute.jar
REGISTRY=rmiregistry

all: compute/compute.jar compute/*.class engine/*.class client/*.class

compute/%.class: compute/%.java
	$(CC) compute/$*.java

compute/compute.jar: compute/*.class 
	$(JAR) compute/compute.jar compute/*.class

engine/*.class: compute/compute.jar engine/*.java
	$(CC) -cp compute/compute.jar engine/*.java

client/*.class: compute/compute.jar compute/*.java
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
