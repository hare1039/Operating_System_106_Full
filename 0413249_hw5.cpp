#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>  //C++ 2011 STANDARD
#include <map>
#include <list>
#include <vector>
#include <algorithm>


///////////////////////////////////////////
///////////////////////////////////////////
/*
must be enabled with the -std=c++11 or -std=gnu++11 compiler options.
*/
///////////////////////////////////////////
using namespace std;

int main(){

  fstream file_;
  file_.open("trace.txt");

  ////////////////////////////////////////
  // FIFO

  cout << "FIFO---" << endl;
	cout << "size" << setw(9) << "miss" << setw(10) << "hit";
	cout << setw(23) << "page fault ratio" << endl;

  for(int frame = 64; frame <= 512; frame *= 2)
  {
      int hit_count = 0, miss_count = 0, frame_next = 0;

      /*enormous improvement in performance
       when using an unordered_map instead of a map
       in a look-up table.
      */
      unordered_map<string, int> table;

      string check[513];
      string line, page;

      while(getline(file_, line))
      {
          page = line.substr(3, 5);
          if(table.find(page) == table.end())
          {
              if(table.find(check[frame_next])!=table.end())
                  table.erase(table.find(check[frame_next]));

              check[frame_next] = page;
              table[page] = frame_next;
              frame_next = (frame_next + 1) % frame;

              miss_count++;
          }
          else
          {
              hit_count++;
          }
      }
      cout << frame << '\t' << miss_count << '\t' << hit_count;
      cout << '\t' << fixed << setprecision(9) << miss_count/(double)(hit_count+miss_count) << endl;
      file_.clear();
      file_.seekg(0);

  }

  ////////////////////////////////////////////////////
  /*
    When I use the data structure which I use in FIFO above , it would have
    a small problem in LRU , so I finally change a different way , using a
    different data structure to implement LRU !!
  */
  /////////////////////////////////////////
  //  LRU

  cout << "LRU---" << endl;
	cout << "size" << setw(9) << "miss" << setw(10) << "hit";
	cout << setw(23) << "page fault ratio" << endl;

  for (int frame = 64; frame <= 512; frame *= 2)
  {

		string line, page, victim;

	  int hit_count = 0, miss_count = 0 , index = 0;

		vector<string> arr;
    arr.resize(frame);

		map<string, int> table;

		while (getline(file_, line))
    {
			page = line.substr(3, 5);

			if (table.find(page) == table.end())
      {

				if (miss_count >= frame)
        {

					victim = arr[0];
					table.erase(table.find(victim));

					for (int i = 0; i < frame - 1; i++)
						arr[i] = arr[i + 1];

					table[page] = frame - 1;
					arr[frame - 1] = page;

					for (int i = 0; i < frame - 1; i++)
						table[arr[i]]--;
				}

				else
        {
					table[page] = index;
					arr[index] = page;
					index++;

					index %= frame;
				}

				miss_count++;
			}
			else
      {

				int page_index = table[page];

				for (int i = page_index; i < table.size() - 1; i++)
					arr[i] = arr[i + 1];

				arr[table.size() - 1] = page;
				for (int i = page_index; i < table.size() - 1; i++)
					table[arr[i]]--;

				table[page] = table.size() - 1;

				hit_count++;
			}

		}

    cout << frame << '\t' << miss_count << '\t' << hit_count;
    cout << '\t' << fixed << setprecision(9) << miss_count/(double)(hit_count+miss_count) << endl;
    file_.clear();
    file_.seekg(0);
	}

  file_.close();
	return 0;
}
