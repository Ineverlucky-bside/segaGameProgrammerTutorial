#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

void readFile(char** buffer, int* size, const char* filename);

template<class T> class Array2D
{
public:
	Array2D() :mArray(0) {}
	~Array2D()
	{
		delete[] mArray;
		mArray = 0;
	}
	void setSize(int size0, int size1)
	{
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}
	T& operator()(int index0, int index1)
	{
		return mArray[index1 * mSize0 + index0];
	}
	const T& operator()(int index0, int index1) const
	{
		return mArray[index1 * mSize0 + index0];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};

class State
{
public:
	State(const char* stageData, int size);
	void update(char input);
  void draw()const;
  bool hasCleared() const;
private:
	enum Object
	{
		OBJ_SPACE,
    OBJ_WALL,
    OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
  void setSize(const char* stageData, int size);
	
	int mWidth;
  int mHeight;
	Array2D<Object> mObjects;
	Array2D<bool> mGoalFlags;
};

int main(int argc, char** argv)
{
	const char* filename = "stageData.txt";
	if (argc >= 2)
	{
		filename = argv[1];
	}
	char* stageData;
	int fileSize;
  readFile(&stageData, &fileSize, filename);
	if (!stageData)
	{
		cout << "stage file could not be read." << endl;
		return 1;
	}
  State* state = new State(stageData, fileSize);

  //main loop
	while (true)
	{
		state->draw();
		if (state->hasCleared())
		{
			break;
		}
    cout << "a:left d:right w:up s:down. command?" << endl;
		char input;
    cin >> input;
		state->update(input);
	}
	cout << "Congratulation's! you won." << endl;
	delete[] stageData;
	stageData = 0;

	while (true)
	{
		;
	}
	return 0;
}

void readFile(char** buffer, int* size, const char* filename)
{
	ifstream in(filename);
	if (!in)
	{
		*buffer = 0;
		*size = 0;
	}
	else
	{
		in.seekg(0, ifstream::end);
		*size = static_cast<int>(in.tellg());
    in.seekg(0, ifstream::beg);
		*buffer = new char[*size];
		in.read(*buffer, *size);
	}
}