#include <iostream>
#include <time.h>
#include <ncurses.h>
#include <string>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

class Task
{
	private:
		enum
		{
			NO_INPUT = -1
			, ITERATIONS_NUMBER = 10
			, SLEEP_TIME = 2000 / ITERATIONS_NUMBER
		};

		int internalSum = 0;
		int printSum = 0; // printable dynamic value
		bool work = true; // loop condition
		int step = 0; // iteration value for printSum

	private:
		void sleepCrossPlatform(int n)
		{
#ifdef WIN32
			Sleep(n);
#else
			usleep(n * 1000);
#endif
		}


		void help()
		{
			print("- Reset\n");
			print("+ Add\n");
			print("q Quit\n\n");
		}

		void start()
		{
			srand(time(NULL));
			// ncurses init
			initscr();  
			timeout(10);
			noecho();

			help();
		}

		void stop() { endwin(); }

		void print(const string &msg)
		{

			printw(msg.c_str());
			refresh();
		}

		void inputAction()
		{
			char input = getch();

			if (input == NO_INPUT) {
				return;
			}

			switch (input) {
				case '+':
					internalSum += rand() % 100 + 1;
					step = (internalSum - printSum) / ITERATIONS_NUMBER;
					break;
				case '-':
					// Process the situation when user press '-' several times in a row
					if (internalSum != 0) {
						step = printSum / ITERATIONS_NUMBER;
					}	

					internalSum = 0;
					break;
				case 'q':
					work = false;
					break;

			}
		}

		void calculate()
		{
			if (internalSum == printSum) {
				return;
			}

			if (printSum > internalSum) {
				printSum -= step;

				if (printSum < 0) {
				       	printSum = 0; 
				}
			} else {
				printSum += step;

				if (printSum > internalSum) {
					printSum = internalSum;
				}
			}
		}

	public:
		Task() { start(); }

		~Task() { stop(); }

		void run()
		{
			while (work) {
				if (printSum != internalSum) {
					print(to_string(printSum) + string(" "));
				}

				inputAction();
				calculate();
				sleepCrossPlatform(SLEEP_TIME);
			}
		}
};

int main(int argc , char *argv[])
{
	Task t;
	t.run();
	return 0;	
}
