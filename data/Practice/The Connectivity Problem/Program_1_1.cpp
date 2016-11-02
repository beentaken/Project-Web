#include <iostream>

static const int N = 100;

int main()
{ 
  int i, first, last;
  int id[N];

  for(i = 0; i < N; i++) 
    id[i] = i;

  while(std::cin >> first >> last)
  { 
    int t = id[first];

    if(t == id[last]) 
      continue;

    for(i = 0; i < N; i++)
      if(id[i] == t) id[i] = id[last];
  
    std::cout << " " << first << " " << last << std::endl;
  }
}