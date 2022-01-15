#ifndef FLATHASH_H
#define FLATHASH_H

#include <iostream>

// Flag(hint) for overflow handling
enum overflow_handle {
  LINEAR_PROBING = 0,
  QUADRATIC_PROBING
};

class FlatHash
{
private:
  unsigned int* hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size of hash table
  unsigned int table_size;
  // Nums of keys
  unsigned int num_of_keys;

public:
  FlatHash(enum overflow_handle _flag, float _alpha);

  ~FlatHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  const unsigned int* getHashtable() { return hashtable; }

  // Return time cost
  int insert(const unsigned int key);

  // Remove function in lecture. Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();
};

FlatHash::FlatHash(enum overflow_handle _flag, float _alpha)
{
  // Initial table size is 1000 
  table_size = 1000;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;
  hashtable = new unsigned int[table_size];
  for (unsigned int i = 0; i < table_size; i++) {
	  hashtable[i] = 0;
  }
}

FlatHash::~FlatHash()
{
	delete[]hashtable;
}

int FlatHash::insert(const unsigned int key)
{
	int tc = 0;
	int tc_result = 0;
	int ex = 0;
	if (key >= 1 && key <= 1000000) {
		int index = hashFunction(key);
		if (hashtable[index] == 0 || hashtable[index] > 1000000) {
			hashtable[index] = key;
			tc++;
			num_of_keys++;
			ex++;
			tc_result = tc;
		}
		else if (hashtable[index] == key) {
			tc++;
			return -tc;
		}
		else {
			// quadratic probing
			if (flag == QUADRATIC_PROBING) {
				tc++;
				for (unsigned int j = 1; j < table_size; j++) {
					tc++;
					int index1 = hashFunction(index + (j * j));
					if (hashtable[index1] == 0 || hashtable[index1] > 1000000) {
						hashtable[index1] = key;
						num_of_keys++;
						ex++;
						break;
					}
					else if (hashtable[index1] == key) {
						return -tc;
					}
				}

				if (ex != 0) {
					tc_result = tc;
				}
				else { // failed to find an empty bucket by quadratic probing
					tc++;
					for (unsigned int j2 = 1; j2 < table_size; j2++) {
						tc++;
						int index2 = hashFunction(index + j2);
						if (hashtable[index2] == 0 || hashtable[index2] > 1000000) {
							hashtable[index2] = key;
							num_of_keys++;
							ex++;
							break;
						}
						else if (hashtable[index2] == key) {
							return -tc;
						}
					}
					tc_result = tc;
				}
			}
			// linear probing
			else {
				tc++;
				for (unsigned int j3 = 1; j3 < table_size; j3++) {
					tc++;
					int index3 = hashFunction(index + j3);
					if (hashtable[index3] == 0 || hashtable[index3] > 1000000) {
						hashtable[index3] = key;
						num_of_keys++;
						ex++;
						break;
					}
					else if (hashtable[index3] == key) {
						return -tc;
					}
				}
				tc_result = tc;
			}
		}
	}

	//resizing regrading to loading factor
	float lf = num_of_keys / (float)table_size;
	
	if (lf >= alpha) {
		//array for existing valid key
		unsigned int* init_hashtable;
		init_hashtable = hashtable;
		unsigned int init_size = table_size;

		//resizing
		table_size = table_size * 2;
		unsigned int* new_hashtable;
		new_hashtable = new unsigned int[table_size];
		for (unsigned int i2 = 0; i2 < table_size; i2++) {
			new_hashtable[i2] = 0;
		}
		hashtable = new_hashtable;

		//rehashing
		num_of_keys = 0;
		for (unsigned int i = 0; i < init_size; i++) {
			if (init_hashtable[i] >= 1 && init_hashtable[i] <= 1000000) {
				this->insert(init_hashtable[i]);
			}
		}
		delete[]init_hashtable;
	}

	if (ex == 0) {
		return -(tc_result);
	}
	else {
		return tc_result;
	}
  // You have to implement two overflow handling by using flag
  // Write your code

}

int FlatHash::remove(const unsigned int key)
{
  // Write your code
	int tc = 0;
	int des = hashFunction(key);
	if (key >= 1 && key <= 1000000) {
		if (hashtable[des] == 0) {
			tc++;
			return -tc;
		}
		else {
			if (flag == QUADRATIC_PROBING) {
				int tombstone = 2000000;
				if (hashtable[des] == key) {
					hashtable[des] = 0;
					num_of_keys--;
					tc++;
					return tc;
				}
				else {
					tc++;
					for (unsigned int j = 1; j < table_size; j++) {
						tc++;
						int index1 = hashFunction(des + (j * j));
						if (hashtable[index1] == key) {
							hashtable[index1] = tombstone;
							num_of_keys--;
							return tc;
						}
						else if (hashtable[index1] == 0) {
							return -tc;
						}
					}
					// failed to find the key by quadratic probing
					tc++;
					for (unsigned int j2 = 1; j2 < table_size; j2++) {
						tc++;
						int index2 = hashFunction(des + j2);
						if (hashtable[index2] == key) {
							hashtable[index2] = tombstone;
							num_of_keys--;
							return tc;
						}
						else if (hashtable[index2] == 0) {
							return -tc;
						}
					}
				}
			}
			// linear probing
			else {
				int shift_index = des;
				if (hashtable[des] == key) {
					hashtable[des] = 0;
					num_of_keys--;
					tc++;
				}
				else {
					tc++;
					for (unsigned int j3 = 1; j3 < table_size; j3++) {
						tc++;
						int index3 = hashFunction(des + j3);
						if (hashtable[index3] == key) {
							hashtable[index3] = 0;
							num_of_keys--;
							shift_index = index3;
							break;
						}
						else if (hashtable[index3] == 0) {
							return -tc;
						}
					}
				}
				//shifting
				int probe = 1;
				unsigned int shifter = hashtable[shift_index + probe];
				while (shifter != 0) {
					if (hashFunction(shifter) != hashFunction(shift_index + probe)) {
						for (int empty = probe - 1; empty >= 0; empty--) {
							if (hashtable[hashFunction(shift_index + empty)] == 0) {
								hashtable[hashFunction(shift_index + empty)] = shifter;
								hashtable[hashFunction(shift_index + probe)] = 0;
							}
						}
					}
					probe++;
					shifter = hashtable[hashFunction(shift_index + probe)];
				}
				return tc;
			}
		}
	}
	return -tc;
}

int FlatHash::search(const unsigned int key)
{
  // Write your code
	int tc = 0;
	int des = hashFunction(key);
	if (key >= 1 && key <= 1000000) {
		if (flag == QUADRATIC_PROBING) {
			if (hashtable[des] == key) {
				tc++;
				return tc;
			}
			tc++;
			for (unsigned int j = 1; j < table_size; j++) {
				tc++;
				int index1 = hashFunction(des + (j * j));
				if (hashtable[index1] == key) {
					return tc;
				}
				else if (hashtable[index1] == 0) {
					return -tc;
				}
			}
			// failed to find the key by quadratic probing
			tc++;
			for (unsigned int j2 = 1; j2 < table_size; j2++) {
				tc++;
				int index2 = hashFunction(des + j2);
				if (hashtable[index2] == key) {
					return tc;
				}
				else if (hashtable[index2] == 0) {
					return -tc;
				}
			}
		}
		else {
			if (hashtable[des] == key) {
				tc++;
				return tc;
			}
			else {
				tc++;
				for (unsigned int j3 = 1; j3 < table_size; j3++) {
					tc++;
					int index3 = hashFunction(des + j3);
					if (hashtable[index3] == key) {
						return tc;
					}
					else if (hashtable[index3] == 0) {
						return -tc;
					}
				}
			}
		}
	}
	return -tc;
}

void FlatHash::clearTombstones()
{
  // Write your code
	if (flag == QUADRATIC_PROBING) {
		unsigned int* init_hashtable;
		init_hashtable = hashtable;

		unsigned int* new_hashtable;
		new_hashtable = new unsigned int[table_size];
		for (unsigned int i = 0; i < table_size; i++) {
			new_hashtable[i] = 0;
		}
		hashtable = new_hashtable;

		//rehashing
		num_of_keys = 0;
		for (unsigned int i1 = 0; i1 < table_size; i1++) {
			if (init_hashtable[i1] >= 1 && init_hashtable[i1] <= 1000000) {
				this->insert(init_hashtable[i1]);
			}
		}
		delete[]init_hashtable;
	}
}

void FlatHash::print()
{
  // Print valid key pair - (index1:key1,index2:key2)
  // Give **NO** space between each character
  // e.g., (1:3,3:7,5:1)
  std::cout << "(";

  unsigned int pr = 0;
  for (unsigned int i = 0; i < table_size; i++) {
	  unsigned int val = hashtable[i];
	  if (val >= 1 && val<=1000000) {
		  std::cout << i << ":" << val;
		  pr++;
		  if (pr != num_of_keys) {
			  std::cout << ",";
		  }
	  }
  }
  // Write your code


  std::cout << ")" << std::endl;
}

#endif
