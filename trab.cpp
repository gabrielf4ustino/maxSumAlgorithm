#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define MAX_SIZE -2147483648

inline uint64_t Rdtsc()
{
    union
    {
        uint64_t x64;
        struct
        {
            uint32_t lo;
            uint32_t hi;
        } loHi;
    } time;
    __asm__ __volatile__("rdtsc\n"
                         : "=a"(time.loHi.lo), "=d"(time.loHi.hi));
    return time.x64;
}

vector<vector<int>> readFile(string fileName)
{
    fstream file;
    string line;
    file.open(fileName);
    if (file.is_open())
    {
        getline(file, line);
        vector<vector<int>> out;
        for (int i = 0; i < stoi(line); i++)
        {
            vector<int> in;
            int aux;
            file >> aux;
            for (int j = 0; j < aux; j++)
            {
                double tmp;
                file >> tmp;
                in.push_back(tmp);
            }
            out.push_back(in);
        }
        file.close();
        return out;
    }
    else
    {
        cout << "Unable to open file";
        return vector<vector<int>>();
    }
}

vector<int> bruteForce(vector<int> in)
{

    int max = MAX_SIZE;
    int start, end, sum;
    vector<int> result;

    for (long unsigned i = 0; i < in.size(); i++)
    {
        if (in[i] > max)
        {
            start = i;
            end = i;
            max = in[i];
        }
        sum = in[i];
        for (long unsigned j = i + 1; j < in.size(); j++)
        {
            sum += in[j];
            if (sum > max)
            {
                start = i;
                end = j;
                max = sum;
            }
        }
    }

    result.insert(result.end(), {start + 1, end + 1, max});

    return result;
}

vector<int> maxSegment(vector<int> in, int start, int mid, int end)
{
    int leftSum = MAX_SIZE, rightSum = MAX_SIZE, leftMax, rightMax, sum = 0;
    vector<int> result;

    for (int i = mid; i >= start; i--)
    {
        sum += in[i];
        if (sum > leftSum)
        {
            leftSum = sum;
            leftMax = i;
        }
    }

    sum = 0;

    for (int i = mid + 1; i <= end; i++)
    {
        sum += in[i];
        if (sum > rightSum)
        {
            rightSum = sum;
            rightMax = i;
        }
    }

    result.insert(result.end(), {leftMax + 1, rightMax + 1, leftSum + rightSum});
    return result;
}

vector<int> divideAndConquer(vector<int> in, int start, int end)
{
    if (start == end)
    {
        vector<int> out;
        out.insert(out.end(), {start + 1, end + 1, in[start]});
        return out;
    }

    int mid = (start + end) / 2;
    vector<int> left = divideAndConquer(in, start, mid);
    vector<int> right = divideAndConquer(in, mid + 1, end);
    vector<int> cross = maxSegment(in, start, mid, end);
    if ((left[2] >= right[2]) && (left[2] >= cross[2]))
    {
        return left;
    }
    else if ((right[2] >= left[2]) && (right[2] >= cross[2]))
    {
        return right;
    }
    else
    {
        return cross;
    }
}



int main(int argc, char *argv[])
{

    // uint32_t a, b, c, d;
    uint64_t y;
    // __cpuid(0, a, b, c, d);
    y = Rdtsc();

    vector<vector<int>> matriz = readFile(argv[1]), result;
    ofstream file;

    file.open("result.txt");

    for (long unsigned int i = 0; i < matriz.size(); i++)
    {
        result.push_back(bruteForce(matriz[i]));
    }

    file << "Força bruta"
         << endl
         << endl;

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < result[i].size(); j++)
            file << result[i][j] << " ";
        file << endl;
    }

    file << endl
         << "Tempo de execução: "
         << Rdtsc() - y
         << endl
         << endl;

    y = Rdtsc();

    result.clear();

    file << "Divisão e conquista"
         << endl
         << endl;

    for (long unsigned int i = 0; i < matriz.size(); i++)
    {
        result.push_back(divideAndConquer(matriz[i], 0, matriz[i].size() - 1));
    }

    for (long unsigned int i = 0; i < result.size(); i++)
    {
        for (long unsigned int j = 0; j < result[i].size(); j++)
            file << result[i][j] << " ";
        file << endl;
    }

    file << endl
         << "Tempo de execução: "
         << Rdtsc() - y
         << endl
         << endl;

    y = Rdtsc();

    file << "Método trivial"
         << endl
         << endl;

    

    file.close();

    return 0;
}