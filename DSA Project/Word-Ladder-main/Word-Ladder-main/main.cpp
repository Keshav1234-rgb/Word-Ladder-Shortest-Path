#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// ANSI escape codes for coloring terminal text
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";

class WordLadderGame {
private:
    unordered_set<string> dictionary;

    // Helper to convert string to lowercase for uniformity
    string toLower(const string& s) {
        string lowerStr = s;
        for (char& c : lowerStr) {
            c = tolower(c);
        }
        return lowerStr;
    }

    // Helper to check if two words differ by exactly one letter
    bool isOneLetterDifference(const string& word1, const string& word2) {
        if (word1.length() != word2.length()) return false;
        int diffCount = 0;
        for (size_t i = 0; i < word1.length(); ++i) {
            if (word1[i] != word2[i]) {
                diffCount++;
                if (diffCount > 1) return false;
            }
        }
        return diffCount == 1;
    }

public:
    // Load words from a text file into the hash set
    bool loadDictionary(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << RED << "Error: Could not open dictionary file '" << filename << "'." << RESET << endl;
            return false;
        }

        string word;
        while (file >> word) {
            // Only add lowercase words (and strip trailing whitespace if any)
            dictionary.insert(toLower(word));
        }
        file.close();
        return true;
    }

    bool isValidWord(const string& word) {
        return dictionary.find(toLower(word)) != dictionary.end();
    }

    size_t getDictionarySize() const {
        return dictionary.size();
    }

    // The BFS Algorithm to find the shortest path
    vector<string> findShortestPath(const string& start, const string& target, bool visualize = false) {
        string s = toLower(start);
        string t = toLower(target);

        if (s == t) return {s};
        if (!isValidWord(s) || !isValidWord(t)) return {};

        queue<string> q;
        q.push(s);

        // Keep track of visited words and their parent to reconstruct the path
        unordered_map<string, string> parentMap;
        parentMap[s] = "";

        if (visualize) {
            cout << YELLOW << "\n--- Starting Breadth-First Search (BFS) ---" << RESET << endl;
            cout << "We use a queue to explore level by level." << endl;
            cout << "Pushing start word: " << GREEN << s << RESET << endl;
        }

        bool found = false;
        int level = 0;
        
        while (!q.empty() && !found) {
            int levelSize = q.size();
            level++;
            
            if (visualize) {
                cout << "\n" << BOLD << CYAN << "[Level " << level << "]" << RESET << " Exploring " << levelSize << " word(s)..." << endl;
            }

            // Process all nodes at the current level
            for (int i = 0; i < levelSize; ++i) {
                string curr = q.front();
                q.pop();

                // Generate all possible valid next words
                string temp = curr;
                for (size_t j = 0; j < temp.length(); ++j) {
                    char originalChar = temp[j];
                    
                    // Try replacing with every other letter a-z
                    for (char c = 'a'; c <= 'z'; ++c) {
                        if (c == originalChar) continue;
                        
                        temp[j] = c;
                        
                        // O(1) lookup using unordered_set
                        if (isValidWord(temp) && parentMap.find(temp) == parentMap.end()) {
                            parentMap[temp] = curr; // Record where we came from
                            
                            if (temp == t) {
                                found = true;
                                break;
                            }
                            
                            q.push(temp);
                        }
                    }
                    temp[j] = originalChar; // Restore for next iteration
                }
                if (found) break;
            }
        }

        if (visualize) {
            if (found) cout << GREEN << "\nPath found! Reconstructing from target to start using parent map." << RESET << endl;
            else cout << RED << "\nQueue empty. No path exists." << RESET << endl;
        }

        if (!found) return {}; // No path found

        // Reconstruct path
        vector<string> path;
        string curr = t;
        while (curr != "") {
            path.push_back(curr);
            curr = parentMap[curr];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void playGame() {
        cout << BOLD << CYAN << "\n=== Interactive Game Mode ===" << RESET << endl;
        string start, target;
        
        cout << "Enter Start Word (4 letters): ";
        cin >> start;
        cout << "Enter Target Word (4 letters): ";
        cin >> target;

        start = toLower(start);
        target = toLower(target);

        if (start.length() != 4 || target.length() != 4) {
            cout << RED << "Please enter 4-letter words." << RESET << endl;
            return;
        }
        if (!isValidWord(start)) {
            cout << RED << "'" << start << "' is not in the dictionary!" << RESET << endl;
            return;
        }
        if (!isValidWord(target)) {
            cout << RED << "'" << target << "' is not in the dictionary!" << RESET << endl;
            return;
        }

        // Validate that a path actually exists before they play!
        vector<string> shortest = findShortestPath(start, target, false);
        if (shortest.empty()) {
            cout << RED << "Sorry, there is no valid path between '" << start << "' and '" << target << "'." << RESET << endl;
            return;
        }

        cout << YELLOW << "\nGoal: Transform " << GREEN << start << YELLOW << " to " << GREEN << target << YELLOW << "." << RESET << endl;
        cout << "Optimal path length is " << shortest.size() << " words." << endl;
        cout << "Enter 'quit' to give up." << endl;

        string currentWord = start;
        int steps = 1;

        while (currentWord != target) {
            cout << "\nCurrent Word: " << BOLD << currentWord << RESET << " (Step " << steps << ")" << endl;
            cout << "Next word: ";
            string nextWord;
            cin >> nextWord;
            
            nextWord = toLower(nextWord);

            if (nextWord == "quit") {
                cout << RED << "You gave up!" << RESET << endl;
                return;
            }

            if (nextWord.length() != currentWord.length()) {
                cout << RED << "Length must match the current word!" << RESET << endl;
                continue;
            }

            if (!isOneLetterDifference(currentWord, nextWord)) {
                cout << RED << "You can only change EXACTLY ONE letter at a time." << RESET << endl;
                continue;
            }

            if (!isValidWord(nextWord)) {
                cout << RED << "'" << nextWord << "' is not a valid dictionary word." << RESET << endl;
                continue;
            }

            currentWord = nextWord;
            steps++;
        }

        cout << BOLD << GREEN << "\n🎉 Congratulations! You reached '" << target << "' in " << steps << " steps!" << RESET << endl;
        if (steps == shortest.size()) {
            cout << MAGENTA << "PERFECT! You found the optimal path!" << RESET << endl;
        } else {
            cout << "The optimal path was " << shortest.size() << " steps." << endl;
        }
    }

    void solveAndLearn() {
        cout << BOLD << MAGENTA << "\n=== Auto-Solve & Learn Mode ===" << RESET << endl;
        string start, target;
        
        cout << "Enter Start Word (4 letters): ";
        cin >> start;
        cout << "Enter Target Word (4 letters): ";
        cin >> target;

        start = toLower(start);
        target = toLower(target);
        
        if (!isValidWord(start) || !isValidWord(target)) {
            cout << RED << "Both words must be valid dictionary words." << RESET << endl;
            return;
        }

        cout << YELLOW << "\n[Educational Note]" << RESET << endl;
        cout << "We are modeling this as an unweighted graph where:" << endl;
        cout << " - Nodes = Dictionary Words" << endl;
        cout << " - Edges = Words that differ by exactly 1 letter" << endl;
        cout << "Because all edges have a weight of 1, Breadth-First Search (BFS) is guaranteed to find the shortest path." << endl;
        cout << "Checking if a word is valid takes O(1) time thanks to std::unordered_set hashing!" << endl;
        
        vector<string> path = findShortestPath(start, target, true);

        if (path.empty()) {
            cout << RED << "\nNo transformation sequence exists." << RESET << endl;
        } else {
            cout << BOLD << GREEN << "\nShortest Path Found (" << path.size() << " steps):" << RESET << endl;
            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i];
                if (i < path.size() - 1) cout << " -> ";
            }
            cout << endl;
        }
    }
};

int main() {
    WordLadderGame game;
    cout << BOLD << CYAN << "Loading dictionary..." << RESET << endl;
    
    // We expect a words.txt file in the same directory
    if (!game.loadDictionary("words.txt")) {
        return 1;
    }
    
    cout << GREEN << "Loaded " << game.getDictionarySize() << " words successfully!" << RESET << endl;

    int choice;
    do {
        cout << "\n" << BOLD << "=== Word Ladder Main Menu ===" << RESET << endl;
        cout << "1. Play Interactive Game" << endl;
        cout << "2. Auto-Solve & Learn Mode (BFS Visualizer)" << endl;
        cout << "3. Exit" << endl;
        cout << "Select an option: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                game.playGame();
                break;
            case 2:
                game.solveAndLearn();
                break;
            case 3:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << RED << "Invalid choice." << RESET << endl;
        }
    } while (choice != 3);

    return 0;
}
