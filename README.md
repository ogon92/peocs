https://docs.google.com/document/d/1InCJcTayB7OIpp6NDCqiKIcg3mSb9r7XI-Rnp7vApig/edit

# zad1 #
Prostej kolejki ze stanowiskiem obsługi z wykładniczym czasem obsługi i ograniczonym buforze N. Źródło też ma rozkład wykładniczy. Wykonać symulację dla:  
Lambda =1 (intensywność źródła); 

Mi=0.01;Mi=0.5;Mu=1.1;Mu=2;Mu=10 (intensywność obsługi). 

Policzyć 

* średnią liczbę zadań w systemie(kolejka+stanowisko - liczymy na podstawie histogramu rozkładu liczby zadań w systemie) , 
* średni czas pobytu zadania w systemie(mierzymy czas pobytu każdego zadania w systemie – metoda setTimestamp i timestamp wywołana na objekcie cMessage ). 
* Zbudować histogram na podstawie stanu kolejki i czasu pobytu w tym stanie(czas ten mierzymy na podstawie czasu symulacji simTime – czas twania każdej zmiany długości kolejki spowodowanej przyjściem lub zakończeniem obsługi zadania.

Histogram może być prostą tablicą długości N+1. Na podstawie histogramu policzyć 

* prawdopodobieństwa: p(o) (system był pusty) ; 
* p(1) (jedno zadanie w systemie- na stanowisku obsługi) ; 
* p(2) (dwa zadanie w systemie- jedno w kolejce); 
* p(N);. 
    
Eksperyment przeprowadzić dla N=5, N=50; N=500; N=5000000; system przed eksperymentem ma być pusty.

Wersję końcową (release) symulacji uruchomić w trybie konsolowym (opp_makemake –f – u cmdenv; Makefile ) i wykonywać około 3min (czasu procesora , nie symulacji)

Wyniki porównać ze wzorami analitycznymi systemu MM1 (Napisać odpowiedniefunkcje w c++). Dla jakich wartości Lambda i Mi wyniki będą poprawne ? Dla jakie musi być N .
Wyniki zapisujemy do pliku (klasa ofstream)

```
c/c++ dodatek  
dodac do includePath sciezke do include z omneta  
"C:/omnetpp-5.2.1/include",  
"C:/omnetpp-5.2.1/include/*"  
```