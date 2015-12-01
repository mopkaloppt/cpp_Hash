#include "Hashtable.h"
#include <cstdlib>
#include <iostream>
#include <cassert> 

using namespace std; 

/*
	(i) Under what load factor is Linear just as good as Quadratic?
		- load factor < 1/2, Linear is just as good as Quadratic
		When deos Quadratic begin to win out?
		- when load factor is close or equal to 1, Quadratic begins to win out

	(ii) How does the choice of hash fn affect double hashing?
		- If you use the same hash function for the second hash, it would create the same sequence
		of probing. 
		
		Can you make a Double hash function that is just 
		as good as Quadratic or make it even better?
		- The double hash function is already better than the Quadratic function.
		The avg. of probe/insert for the load factor >= 1 is lower than Quadratic. 

		Can you make the Double hash worse than the Quadratic?
		- Make the hash2 be a constant then it will be the same as Linear


	(iii) When does Double hashing begin to win the other two?
		- when LF is close to 1/2 (4999 items 10000 slots)
	
	(iv) What open adressing scheme would you choose, why?
		- Quadratic, although the avg. probe/insert is higher, 
			less failures, higher chance to success



    Linear:
		Linear success guaranteed : load factor < 1 
		Performance quickly degrades : load factor > 1/2

	Quadratic:
		First size/2 probes are distinct 
		First size/2 slots in quadratic probe sequence 
		are distinct the rest is duplicates (repeating same probe sequence)
	Success : load factor < 1/2
	May not succeed : lf > 1/2

	Double Hashing:
		Success : load factor < 1
*/




Hashtable::Hashtable(int size) {
	//constructor
	int prime = nextPrime( size ); 
	_size = size;
	if (prime != size) { 
	  cout << "Warning: size = " << size << " is not a prime number." << endl; 
/* uncomment these if you want */ 
//	  cout << "Using " << prime << " instead." << endl; 
//	  _size = prime; 
	}
	_totalProbes = 0;
	_numInserts = 0;
	_numFailures = 0; 
	_table = new int [ _size ];
	for (int ii=0; ii < _size; ii++) {
		_table[ ii ] = EMPTY;
	}
}
void Hashtable::clear(void) {
	_totalProbes = 0;
	_numInserts = 0;
	_numFailures = 0; 
	for (int ii=0; ii < _size; ii++) {
		_table[ ii ] = EMPTY;
	}
}    
    

Hashtable::~Hashtable() {
	//deconstructor
	delete[] _table;
}

void Hashtable::tallyProbes(int p) {
	// Add the number of probes, p, for one insert operation to the tally.
	_totalProbes += p;
	_numInserts += 1;
}

void Hashtable::printStats() {
	cout << "Average probes/insert = " <<
			probeRate() << " = " <<
			_totalProbes << "/" << _numInserts;
	cout <<", size = "<< _size;
	cout <<", failures = " << _numFailures << endl;
}

float Hashtable::probeRate() {
	return (float)_totalProbes / (float)_numInserts;
}

int Hashtable::hash(int k) {
	return k % _size;
}

// hash2(k) = prime - (k mod prime) 
int Hashtable::hash2(int k) {
	// prime < _size
	int m = _size - 1;
	int prime;
	// never be 0 (mod m)
	while (m != k && m > 0) {
		if(isPrime(m)) { // put it in the constructor
			prime = m;
			return prime - (k % prime);  //prime is prime < _size  
		}
			m--;
	}	
}

void Hashtable::qinsert(int k) {
	// Insert k in the hash table.
	// Use open addressing with quadratic probing and hash(k) = k % _size.

	// Tips:
	// - Look at the lecture slides if you don't remember how to implement these.
	// - You need to prevent against infinite loops. You should limit the number
	// of times you probe and print an error message upon exceeding this limit.
	// - You're welcome to add new declarations to hash.h, add to the constructor,
	// include libraries, or anything else you need. 
	// - You're also welcome to modify the main() method to automate your testing.

    // ************* ADD YOUR CODE HERE *******************

    int probe = 0;
    int pos = hash(k) % _size;
    // _size/2 to guarantee unique ...
    for (int i=0; i <= _size/2; i++) {
    	probe++;
    	if(_table[pos] == EMPTY) {
    		_table[pos] = k;
    		tallyProbes(probe);
    		return;
    	}
    	else {
    		pos = (pos + 2*i - 1) % _size;
    	}
    }
    
    
    // Your method should return after it stores the value in an EMPTY slot 
    // and calls tallyProbes, so if it gets here, it didn't find an EMPTY slot 
    _numFailures += 1; 
    //cout << "Warning: qinsert(" << k << ") found no EMPTY slot, so no insert was done." << endl; 
}

void Hashtable::linsert(int k) {
	// Insert k in the hash table.
	// Use open addressing with linear probing and hash(k) = k % _size.

    // ************* ADD YOUR CODE HERE *******************
    int probe = 0;
    int pos = hash(k) % _size;

    for (int i=0; i <= _size; i++) {
    	probe++;
    	if(_table[pos] == EMPTY) {
    		_table[pos] = k;
    		tallyProbes(probe);
    		return;
    	}
    	else {
    		pos = (pos + 1) % _size;
    	}
    }
   
    // Your method should return after it stores the value in an EMPTY slot 
    // and calls tallyProbes, so if it gets here, it didn't find an EMPTY slot 

    _numFailures += 1; 
    //cout << "Warning: linsert(" << k << ") found no EMPTY slot, so no insert was done." << endl; 
}

void Hashtable::dinsert(int k) {
	// Insert k in the hash table.
	// Use open addressing with double hashing. Use the existing hash function
	// and also implement a second hash function.

    // ************* ADD YOUR CODE HERE *******************
    int probe = 0;
    int pos = hash(k) % _size;
    int h2  = hash2(k);
   
    for (int i=0; i <= _size; i++) {
    	probe++;
    	if(_table[pos] == EMPTY) {
    		_table[pos] = k;
    		tallyProbes(probe);
    		return;
    	}
    	else {
    		pos = (pos + h2) % _size;
    	}
    }
   
    // Your method should return after it stores the value in an EMPTY slot 
    // and calls tallyProbes, so if it gets here, it didn't find an EMPTY slot 
    _numFailures += 1; 
    //cout << "Warning: dinsert(" << k << ") found no EMPTY slot, so no insert was done." << endl; 
}

void Hashtable::print() {
	// Print the content of the hash table.

	for (int i=0; i<_size; i++) {
		cout<<"["<<i<<"] ";
		if (_table[i] != EMPTY)
			cout << _table[i];
		cout << endl;
	}
}

bool Hashtable::checkValue(int k, int i) {
	// Check if value k is at index i. Use this to help you with testing.

	return (_table[i] == k);
}

int Hashtable::nextPrime( int n ) { 
    int loops = (n < 100) ? 100 : n;
    for (int ii = 0; ii < loops; ii++ ){ 
      if ( isPrime( n + ii ) ) return (n + ii); 
    }
    assert( false ); // logic error 
}
bool Hashtable::isPrime( int n ) { 
    if (n < 2) return false; 
    if (n == 2) return true;
    if (n % 2 == 0) return false;  
    return isPrime( n, 3 ); 
}
bool Hashtable::isPrime( int n, int divisor ){ 
    if ((divisor * divisor) > n) return true; 
    if ((n % divisor) == 0) return false; 
    return isPrime( n, divisor + 2 ); 
}


