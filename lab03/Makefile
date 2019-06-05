# variables
CC=javac
JAR=jar cvf
RM=rm -f

all: compute engine client

compute: compute/Compute.java
	$(CC) compute/Compute.java
	$(JAR) compute/compute.jar compute/*.class

engine: engine/ComputeEngine.java compute/compute.jar
	$(CC) -cp compute/compute.jar engine/ComputeEngine.java

client: client/client.c
	$(CC) -cp compute/compute.jar client/*.java

clean:
	$(RM) compute/compute.jar
	$(RM) compute/*.class
	$(RM) engine/*.class
	$(RM) client/*.class
