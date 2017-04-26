NAZWA = rsa
$(NAZWA): $(NAZWA).o
	g++ -o $(NAZWA) $(NAZWA).o
$(NAZWA).o: $(NAZWA).cpp
	g++ -lgmp -Wall -c $(NAZWA).cpp
clean:
	rm -f $(NAZWA) *.o