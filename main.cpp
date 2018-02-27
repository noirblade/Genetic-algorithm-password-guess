#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
using namespace std;

#define POPULATION_COUNT 20

const string password = "ssssss";
const string charList = "abcdefghijklmnopqrstuvwxyz";
int maxScore = -40; // Set to minimum possible

// Ugly to set this globally but OK for the task
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_int_distribution<int> distribution(0, charList.size()-1);
uniform_int_distribution<int> distributionNumChars(1, 19);

void buildPopulation(vector<string>& population){

    for (int i = 0; i < POPULATION_COUNT; i++){
        string data;
        int stringSize = distributionNumChars(generator);
        for(int j = 0; j< stringSize; j++) {
            data += charList[distribution(generator)];
        }
        population.push_back(data);
    }
}

int getScore(string data){

    int score = 0;
    int i = 0;
    for (; i < data.size(); i++){
        if (i >= password.size()){
            score -= 2;
            continue;
        }
        if (data[i] == password[i]){
            score += 5;
            continue;
        }
        score -= 2;
    }

    for (; i < password.size(); i++){
        score -= 2;
    }

    return score;
}

void mutate(string& data){
    int stringSize = distributionNumChars(generator);

    int oldSize = data.size();
    data.resize(stringSize);
    for(int i = 0; i < stringSize; i++) {
        // Every even char is randomized
        if (i%2==0 && oldSize > i){
            data[i] = charList[distribution(generator)];
        } else if (oldSize <= i){
            data[i] = charList[distribution(generator)];
        }
    }
}

void crossOver(string& target, string merge){
    int i = 0;
    for (; i < target.size(); i++){
        if (i%2==0 && i < merge.size()){
            target[i] = merge[i];
        }
    }
}

void checkPassword(vector<string>& population){
    int scores[POPULATION_COUNT];
    int minScore = 100; // Set to max score

    while (maxScore < 100){
        // 100 max score for 20 char pass
        minScore = 100;
        // -40 minimum score for 20 char pass
        maxScore = -40;

        for (int i = 0; i < POPULATION_COUNT; i++){
            int score = getScore(population[i]);
            scores[i] = score;
            if (maxScore < score){
                maxScore = score;
            }
            if (minScore > score){
                minScore = score;
            }
        }

        float range = maxScore-minScore;
        for (int i = 0; i < POPULATION_COUNT; i++){
            if (scores[i] < minScore+(range/3)){
                // Bellow 2/3 score range
                mutate(population[i]);
            } else if (scores[i] < minScore+(range/1.5)){
                // Bellow 1/3 score range
                crossOver(population[i], population[(i == population.size()-1) ? i-1 : i+1]);
            }
            // else we will leave it unchanged
            // The 1/3 score range
        }
   }
}

void userEventHandler(){
    cout << "Write 's'+enter to show the current score 'q' to quit" << endl;

    char input;
    while (cin >> input){
        if (input == 'q'){
            cout << "Quiting interactive options" << endl;
            break;
        }
        cout << "Current score is: " << maxScore << endl;
    }
}

int main() {
    vector <string> population;
    buildPopulation(population);

    std::thread t(userEventHandler);
    checkPassword(population);
    t.join();
    return 0;
}