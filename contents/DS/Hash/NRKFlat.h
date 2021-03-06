#ifndef NRKFLAT_H
#define NRKFLAT_H

#include "FlatHash.h"
#include <iostream>
#include <stdint.h>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

class NRKFlat : public FlatHash
{
private:
  // Counter array
  unsigned int* counters;
  // Size of NRK filter (the number of counters)
  unsigned int filter_size;
  float alpha;

public:
  NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size);

  ~NRKFlat();
  
  unsigned int hashFunction(const unsigned int key) { return key % filter_size; }
  
  // Hash function
  unsigned int murmurHash2(const void* key);

  void getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3);

  bool filter(const unsigned int key);

  // Overwriting
  int insert(const unsigned int key);
  
  // Overwriting
  int remove(const unsigned int key);

  // Overwriting
  int search(const unsigned int key);
};

NRKFlat::NRKFlat(enum overflow_handle _flag, float _alpha, unsigned int _filter_size) : FlatHash(_flag, _alpha)
{
  filter_size = _filter_size;
  // Write your code
  counters = new unsigned int[filter_size];
  for (unsigned int i = 0; i < filter_size; i++) {
      counters[i] = 0;
  }

  alpha = _alpha;
}

NRKFlat::~NRKFlat()
{
  // Write your code
    delete[]counters;
}

unsigned int NRKFlat::murmurHash2(const void* key){
  int len = 4;
  unsigned int seed = 2019;

  const unsigned int m = 0x5bd1e995;
  const int r = 24;

  unsigned int h = seed ^ len;

  const unsigned char * data = (const unsigned char *)key;

  while(len >= 4)
  {
    unsigned int k = *(unsigned int *)data;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h *= m; 
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch(len)
  {
  case 3: h ^= data[2] << 16;
  case 2: h ^= data[1] << 8;
  case 1: h ^= data[0];
          h *= m;
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

void NRKFlat::getMMHashValue(const unsigned int key, unsigned int& h1, unsigned int& h2, unsigned int& h3)
{ 
  // You can use h1, h2 and h3 as hashing results which you have to use to decide counter locations
  h1 = murmurHash2(INT2VOIDP(&key));
  h2 = murmurHash2(INT2VOIDP(&h1));
  h3 = murmurHash2(INT2VOIDP(&h2));
  
}

bool NRKFlat::filter(const unsigned int key)
{
  // You can use h1, h2 and h3 as hashing results which you have to use to decide counter locations
  unsigned int h1, h2, h3;
  getMMHashValue(key, h1, h2, h3);

  // Write your code
  int h1_index = hashFunction(h1);
  int h2_index = hashFunction(h2);
  int h3_index = hashFunction(h3);

  if (counters[h1_index] > 0 && counters[h2_index] > 0 && counters[h3_index] > 0) {
      return true;
  }
  else {
      return false;
  }

}

int NRKFlat::insert(const unsigned int key)
{
  // Write your code
    unsigned int h1, h2, h3;
    getMMHashValue(key, h1, h2, h3);

    int h1_index = hashFunction(h1);
    int h2_index = hashFunction(h2);
    int h3_index = hashFunction(h3);

    counters[h1_index] += 1;
    counters[h2_index] += 1;
    counters[h3_index] += 1;

    float init_table_size = (float)getTableSize();

    int tc = FlatHash::insert(key);

    float lf = getNumofKeys() / init_table_size;

    if (lf >= alpha) {
        unsigned int* init_counters;
        init_counters = counters;

        filter_size = filter_size * 2;
        unsigned int* new_counters;
        new_counters = new unsigned int[filter_size];
        for (unsigned int i = 0; i < filter_size; i++) {
            new_counters[i] = 0;
        }
        counters = new_counters;
        delete[]init_counters;

        for (unsigned int j = 0; j < getTableSize(); j++) {
            if (getHashtable()[j] >= 1 && getHashtable()[j] <= 1000000) {
                unsigned int new_key = getHashtable()[j];
                unsigned int n_h1, n_h2, n_h3;
                getMMHashValue(new_key, n_h1, n_h2, n_h3);

                int n_h1_index = hashFunction(n_h1);
                int n_h2_index = hashFunction(n_h2);
                int n_h3_index = hashFunction(n_h3);

                counters[n_h1_index] += 1;
                counters[n_h2_index] += 1;
                counters[n_h3_index] += 1;
                
            }
        }
    }

    return tc;
}

int NRKFlat::remove(const unsigned int key)
{
  // Write your code
    unsigned int h1, h2, h3;
    getMMHashValue(key, h1, h2, h3);

    int h1_index = hashFunction(h1);
    int h2_index = hashFunction(h2);
    int h3_index = hashFunction(h3);

    counters[h1_index] -= 1;
    counters[h2_index] -= 1;
    counters[h3_index] -= 1;

    if (filter(key)) {
        return FlatHash::remove(key);
    }
    else {
        return 0;
    }
}

int NRKFlat::search(const unsigned int key)
{
  // Write your code
    if (filter(key)) {
        return FlatHash::search(key);
    }
    else {
        return 0;
    }
}

#endif
