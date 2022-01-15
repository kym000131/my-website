#ifndef HIERARCHYHASH_H
#define HIERARCHYHASH_H

#include <iostream>
#include "FlatHash.h"

class HierarchyHash
{
private:
  unsigned int** hashtable;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size(range) of full hash table. Initially 1000
  unsigned int table_size;
  // Size of subhash table. Fixed by 100
  unsigned int sub_table_size;
  // Nums of keys
  unsigned int num_of_keys;
 
public:
  HierarchyHash(enum overflow_handle _flag, float _alpha);

  ~HierarchyHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return the size of allocated sub hash table
  unsigned int getAllocatedSize();

  // Return time cost
  int insert(const unsigned int key);

  // Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();
};

HierarchyHash::HierarchyHash(enum overflow_handle _flag, float _alpha)
{
  // Initial map size is 1000
  table_size = 1000;
  // Table size is fixed to 100
  sub_table_size = 100;
  flag = _flag;
  alpha = _alpha;
  num_of_keys = 0;
  hashtable = new unsigned int* [table_size / sub_table_size];
  for (unsigned int i = 0; i < table_size / sub_table_size; i++) {
      hashtable[i] = NULL;
  }
  // Write your code

}

HierarchyHash::~HierarchyHash()
{
    for (unsigned int i = 0; i < table_size / sub_table_size; i++) {
		delete[]hashtable[i];
		hashtable[i] = NULL;
    }
    delete[]hashtable;
	hashtable = NULL;
  // Write your code
}

unsigned int HierarchyHash::getAllocatedSize()
{
  // Write your code
    unsigned int num = 0;
    for (unsigned int i = 0; i < table_size / sub_table_size; i++) {
        if (hashtable[i] != NULL) {
            num++;
        }
    }
    return num*sub_table_size;
}

int HierarchyHash::insert(const unsigned int key)
{
  // Write your code
    int tc = 0;
    int tc_result = 0;
	int ex = 0;
    int index = hashFunction(key);
    int sb_loc = index / sub_table_size;
    int sb_index = index % sub_table_size;
    if (key >= 1 && key <= 1000000) {
        if (hashtable[sb_loc] == NULL) {
            hashtable[sb_loc] = new unsigned int[sub_table_size];
            for (unsigned int i = 0; i < sub_table_size; i++) {
                hashtable[sb_loc][i] = 0;
            }
            tc++;
            hashtable[sb_loc][sb_index] = key;
			num_of_keys++;
			ex++;
            tc_result = tc;
        }
        else {
            if (hashtable[sb_loc][sb_index] == 0 || hashtable[sb_loc][sb_index] > 1000000) {
                hashtable[sb_loc][sb_index] = key;
                tc++;
				num_of_keys++;
				ex++;
                tc_result = tc;
            }
			else if (hashtable[sb_loc][sb_index] == key) {
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
						int sb_loc1 = index1 / sub_table_size;
						int sb_index1 = index1 % sub_table_size;
						if (sb_loc != sb_loc1) {
							if (hashtable[sb_loc1] == NULL) {
								hashtable[sb_loc1] = new unsigned int[sub_table_size];
								for (unsigned int i = 0; i < sub_table_size; i++) {
									hashtable[sb_loc1][i] = 0;
								}
							}
						}
						if (hashtable[sb_loc1][sb_index1] == 0 || hashtable[sb_loc1][sb_index1] > 1000000) {
							hashtable[sb_loc1][sb_index1] = key;
							num_of_keys++;
							ex++;
							break;
						}
						else if (hashtable[sb_loc1][sb_index1] == key) {
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
							int sb_loc2 = index2 / sub_table_size;
							int sb_index2 = index2 % sub_table_size;
							if (sb_loc != sb_loc2) {
								if (hashtable[sb_loc2] == NULL) {
									hashtable[sb_loc2] = new unsigned int[sub_table_size];
									for (unsigned int i = 0; i < sub_table_size; i++) {
										hashtable[sb_loc2][i] = 0;
									}
								}
							}
							if (hashtable[sb_loc2][sb_index2] == 0 || hashtable[sb_loc2][sb_index2] > 1000000) {
								hashtable[sb_loc2][sb_index2] = key;
								num_of_keys++;
								ex++;
								break;
							}
							else if (hashtable[sb_loc2][sb_index2] == key) {
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
						int sb_loc3 = index3 / sub_table_size;
						int sb_index3 = index3 % sub_table_size;
						if (sb_loc != sb_loc3) {
							if (hashtable[sb_loc3] == NULL) {
								hashtable[sb_loc3] = new unsigned int[sub_table_size];
								for (unsigned int i = 0; i < sub_table_size; i++) {
									hashtable[sb_loc3][i] = 0;
								}
							}
						}
						if (hashtable[sb_loc3][sb_index3] == 0 || hashtable[sb_loc3][sb_index3] > 1000000) {
							hashtable[sb_loc3][sb_index3] = key;
							num_of_keys++;
							ex++;
							break;
						}
						else if (hashtable[sb_loc3][sb_index3] == key) {
							return -tc;
						}
					}
					tc_result = tc;
				}
			}
        }
    }

	//resizing regrading to loading factor
	float lf = num_of_keys / (float)table_size;

	if (lf >= alpha) {
		//array for existing valid key
		unsigned int** init_hashtable;
		init_hashtable = hashtable;
		unsigned int init_size = table_size;

		//resizing
		table_size = table_size * 2;
		unsigned int** new_hashtable;
		new_hashtable = new unsigned int*[table_size/sub_table_size];
		for (unsigned int i2 = 0; i2 < table_size/sub_table_size; i2++) {
			new_hashtable[i2] = NULL;
		}
		hashtable = new_hashtable;

		//rehashing
		num_of_keys = 0;
		for (unsigned int i0 = 0; i0 < init_size/sub_table_size; i0++) {
			for (unsigned int i1 = 0; i1 < sub_table_size; i1++) {
				if (init_hashtable[i0] != NULL) {
					if (init_hashtable[i0][i1] >= 1 && init_hashtable[i0][i1] <= 1000000) {
						this->insert(init_hashtable[i0][i1]);
					}
				}
			}
		}
		for (unsigned int j = 0; j < init_size / sub_table_size; j++) {
			delete[]init_hashtable[j];
		}
		delete[]init_hashtable;
	}

	if (ex == 0) {
		return -(tc_result);
	}
	else {
		return tc_result;
	}
}

int HierarchyHash::remove(const unsigned int key)
{
  // Write your code
	int tc = 0;
	int index = hashFunction(key);
	int sb_loc = index / sub_table_size;
	int sb_index = index % sub_table_size;
	if (key >= 1 && key <= 1000000) {
		if (hashtable[sb_loc] == NULL) {
			tc++;
			return -tc;
		}
		else {
			if (hashtable[sb_loc][sb_index] == 0) {
				tc++;
				return -tc;
			}
			else {
				// quadratic probing
				if (flag == QUADRATIC_PROBING) {
					int tombstone = 2000000;
					if (hashtable[sb_loc][sb_index] == key) {
						hashtable[sb_loc][sb_index] = 0;
						unsigned int check = 0;
						for (unsigned int i = 0; i < sub_table_size; i++) {
							if (hashtable[sb_loc][i] != 0) {
								check++;
							}
						}
						if (check == 0) {
							delete[]hashtable[sb_loc];
							hashtable[sb_loc] = NULL;
						}
						tc++;
						num_of_keys--;
						return tc;
					}
					else {
						tc++;
						for (unsigned int j = 1; j < table_size; j++) {
							tc++;
							int index1 = hashFunction(index + (j * j));
							int sb_loc1 = index1 / sub_table_size;
							int sb_index1 = index1 % sub_table_size;
							if (hashtable[sb_loc1][sb_index1] == key) {
								hashtable[sb_loc1][sb_index1] = tombstone;
								num_of_keys--;
								return tc;
							}
							else if (hashtable[sb_loc1][sb_index1] == 0) {
								return -tc;
							}
						}
						// failed to find the key by quadratic probing
						tc++;
						for (unsigned int j2 = 1; j2 < table_size; j2++) {
							tc++;
							int index2 = hashFunction(index + j2);
							int sb_loc2 = index2 / sub_table_size;
							int sb_index2 = index2 % sub_table_size;
							if (hashtable[sb_loc2][sb_index2] == key) {
								hashtable[sb_loc2][sb_index2] = tombstone;
								num_of_keys--;
								return tc;
							}
							else if (hashtable[sb_loc2][sb_index2] == 0) {
								return -tc;
							}
						}
					}
				}
				// linear probing
				else {
					int nullifty = 0;
					int shift_index = index;
					if (hashtable[sb_loc][sb_index] == key) {
						hashtable[sb_loc][sb_index] = 0;
						unsigned int check = 0;
						for (unsigned int i = 0; i < sub_table_size; i++) {
							if (hashtable[sb_loc][i] != 0) {
								check++;
							}
						}
						if (check == 0) {
							delete[]hashtable[sb_loc];
							hashtable[sb_loc] = NULL;
							nullifty++;
						}
						num_of_keys--;
						tc++;
					}
					else {
						tc++;
						for (unsigned int j3 = 1; j3 < table_size; j3++) {
							tc++;
							int index3 = hashFunction(index + j3);
							int sb_loc3 = index3 / sub_table_size;
							int sb_index3 = index3 % sub_table_size;
							if (hashtable[sb_loc3][sb_index3] == key) {
								hashtable[sb_loc3][sb_index3] = 0;
								num_of_keys--;
								shift_index = (sb_loc3 * sub_table_size) + sb_index3;
								break;
							}
							else if (hashtable[sb_loc3][sb_index3] == 0) {
								return -tc;
							}
						}
					}
					//shifting
					if (nullifty == 0) {
						int probe = 1;
						unsigned int shifter = hashtable[hashFunction(shift_index + probe) / sub_table_size][hashFunction(shift_index + probe) % sub_table_size];
						while (shifter != 0) {
							if (hashFunction(shifter) / sub_table_size != hashFunction(shift_index + probe) / sub_table_size || hashFunction(shifter) % sub_table_size != hashFunction(shift_index + probe) % sub_table_size) {
								for (int empty = probe - 1; empty >= 0; empty--) {
									if (hashtable[hashFunction(shift_index + empty) / sub_table_size][hashFunction(shift_index + empty) % sub_table_size] == 0) {
										hashtable[hashFunction(shift_index + empty) / sub_table_size][hashFunction(shift_index + empty) % sub_table_size] = shifter;
										hashtable[hashFunction(shift_index + probe) / sub_table_size][hashFunction(shift_index + probe) % sub_table_size] = 0;
									}
								}
							}
							probe++; 
							shifter = hashtable[hashFunction(shift_index + probe) / sub_table_size][hashFunction(shift_index + probe) % sub_table_size];
						}
						for (unsigned int i = 0; i < table_size / sub_table_size; i++) {
							if (hashtable[i] != NULL) {
								int NullCheck = 0;
								for (unsigned int j = 0; j < sub_table_size; j++) {
									if (hashtable[i][j] != 0) {
										NullCheck++;
									}
								}
								if (NullCheck == 0) {
									delete[]hashtable[i];
									hashtable[i] = NULL;
								}
							}
						}
					}
					return tc;
				}
			}
		}
	}
	return -tc;
}

int HierarchyHash::search(const unsigned int key)
{
  // Write your code
	int tc = 0;
	int index = hashFunction(key);
	int sb_loc = index / sub_table_size;
	int sb_index = index % sub_table_size;
	if (key >= 1 && key <= 1000000) {
		if (hashtable[sb_loc] == NULL) {
			tc++;
			return -tc;
		}
		else {
			if (hashtable[sb_loc][sb_index] == 0) {
				tc++;
				return -tc;
			}
			else {
				// quadratic probing
				if (flag == QUADRATIC_PROBING) {
					if (hashtable[sb_loc][sb_index] == key) {
						tc++;
						return tc;
					}
					else {
						tc++;
						for (unsigned int j = 1; j < table_size; j++) {
							tc++;
							int index1 = hashFunction(index + (j * j));
							int sb_loc1 = index1 / sub_table_size;
							int sb_index1 = index1 % sub_table_size;
							if (hashtable[sb_loc1][sb_index1] == key) {
								return tc;
							}
							else if (hashtable[sb_loc1][sb_index1] == 0) {
								return -tc;
							}
						}
						// failed to find the key by quadratic probing
						tc++;
						for (unsigned int j2 = 1; j2 < table_size; j2++) {
							tc++;
							int index2 = hashFunction(index + j2);
							int sb_loc2 = index2 / sub_table_size;
							int sb_index2 = index2 % sub_table_size;
							if (hashtable[sb_loc2][sb_index2] == key) {
								return tc;
							}
							else if (hashtable[sb_loc2][sb_index2] == 0) {
								return -tc;
							}
						}
					}
				}
				// linear probing
				else {
					if (hashtable[sb_loc][sb_index] == key) {
						tc++;
						return tc++;
					}
					else {
						tc++;
						for (unsigned int j3 = 1; j3 < table_size; j3++) {
							tc++;
							int index3 = hashFunction(index + j3);
							int sb_loc3 = index3 / sub_table_size;
							int sb_index3 = index3 % sub_table_size;
							if (hashtable[sb_loc3][sb_index3] == key) {
								return tc;
							}
							else if (hashtable[sb_loc3][sb_index3] == 0) {
								return -tc;
							}
						}
					}
				}
			}
		}
	}
	return -tc;
}

void HierarchyHash::clearTombstones()
{
  // Write your code
	if (flag == QUADRATIC_PROBING) {
		//array for existing valid key
		unsigned int** init_hashtable;
		init_hashtable = hashtable;

		unsigned int** new_hashtable;
		unsigned int len = table_size / sub_table_size;
		new_hashtable = new unsigned int* [len];
		for (unsigned int i = 0; i < len; i++) {
			new_hashtable[i] = NULL;
		}
		hashtable = new_hashtable;

		//rehashing
		num_of_keys = 0;
		for (unsigned int i0 = 0; i0 < table_size / sub_table_size; i0++) {
			for (unsigned int i1 = 0; i1 < sub_table_size; i1++) {
				if (init_hashtable[i0] != NULL) {
					if (init_hashtable[i0][i1] >= 1 && init_hashtable[i0][i1] <= 1000000) {
						this->insert(init_hashtable[i0][i1]);
					}
				}
			}
		}
		for (unsigned int j = 0; j < table_size / sub_table_size; j++) {
			delete[]init_hashtable[j];
		}
		delete[]init_hashtable;
	}
}

void HierarchyHash::print()
{
  // Print valid key pair for each sub hash table - subtable_id:(index1:key1,index2:key2)
  // Seperate each sub table by endl
  // Give **NO** space between each character
  // e.g., 0:(1:3,3:7,5:1)
  //       1:(101:2,192:10)
  //       9:(902:90,938:82)
  // Exceptionally, keep this code only for the case there is no key in the table
  if (getNumofKeys() == 0) {
    std::cout << "()" << std::endl;
    return;
  }
  else {
	  for (unsigned int i = 0; i < table_size / sub_table_size; i++) {
		  if (hashtable[i] != NULL) {
			  int ts_check = 0;
			  for (unsigned int j = 0; j < sub_table_size; j++) {
				  if (hashtable[i][j] >= 1 && hashtable[i][j] <= 1000000) {
					  ts_check++;
				  }
			  }
			  if(ts_check != 0){
				  std::cout << i << ":";
				  std::cout << "(";
				  int valid_num = 0;
				  for (unsigned int i0 = 0; i0 < sub_table_size; i0++) {
					  unsigned int val = hashtable[i][i0];
					  if (val >= 1 && val <= 1000000) {
						  valid_num++;
					  }
				  }
				  int compare_num = 0;
				  for (unsigned int i1 = 0; i1 < sub_table_size; i1++) {
					  unsigned int val = hashtable[i][i1];
					  if (val >= 1 && val <= 1000000) {
						  std::cout << i1 + (100 * i) << ":" << val;
						  compare_num++;
						  if (compare_num < valid_num) {
							  std::cout << ",";
						  }
					  }
				  }
				  std::cout << ")" << std::endl;
			  }
		  }
	  }
  }

  // Write your code

}

#endif
