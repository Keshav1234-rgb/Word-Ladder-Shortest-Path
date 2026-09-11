# Word Ladder Project Walkthrough

The Word Ladder C++ Console Application is now complete! It successfully combines an interactive puzzle game with an educational BFS visualizer, fitting perfectly into the hackathon theme.

## 🛠️ What We Built

1.  **Dictionary Parser (`words.txt`)**
    *   I fetched a comprehensive list of English words and filtered it down to exactly **7,186 four-letter words**. This ensures a rich, solvable puzzle experience.
    *   The dictionary is loaded into a `std::unordered_set<string>` at startup. This data structure hashes the words, providing lightning-fast $O(1)$ lookups, which is critical for validating thousands of word transitions during the BFS traversal.

2.  **Core BFS Algorithm (`findShortestPath`)**
    *   The algorithm models the dictionary as an implicit, unweighted graph.
    *   It uses a `std::queue<string>` to explore neighboring words level by level.
    *   A `std::unordered_map<string, string>` keeps track of "which word brought us here" so that we can backtrack and reconstruct the exact sequence of transformations once the target word is reached.

3.  **Interactive Game Mode**
    *   You are prompted for a Start Word and Target Word.
    *   You enter words one by one. The program strictly enforces the rules: the word must exist in the dictionary, and it must change *exactly* one letter from the previous word.
    *   It gives you real-time feedback and tells you if you found the optimally shortest path!

4.  **Auto-Solve & Learn Mode**
    *   Designed specifically for the educational aspect of the hackathon.
    *   It prints educational tooltips explaining the data structures used.
    *   It prints a live feed of the BFS queue size at each depth/level, visually demonstrating how BFS exponentially explores the graph until it finds the target.

## 🚀 How to Run It

Everything you need is located in the `c:\Users\krpri\DSAProject` folder.

**1. Compile the code:**
Open a terminal in the project folder and run the `g++` compiler:
```powershell
g++ main.cpp -o wordladder.exe
```

**2. Run the executable:**
```powershell
.\wordladder.exe
```

## 🎮 Demo Screenshots

When you run the game in **Auto-Solve Mode** (e.g. going from `cold` to `warm`), the terminal output beautifully illustrates the graph traversal:

> [!NOTE]
> **Output Example**
> ```text
> [Educational Note]
> We are modeling this as an unweighted graph where:
>  - Nodes = Dictionary Words
>  - Edges = Words that differ by exactly 1 letter
> Because all edges have a weight of 1, Breadth-First Search (BFS) is guaranteed to find the shortest path.
> Checking if a word is valid takes O(1) time thanks to std::unordered_set hashing!
> 
> --- Starting Breadth-First Search (BFS) ---
> We use a queue to explore level by level.
> Pushing start word: cold
> 
> [Level 1] Exploring 1 word(s)...
> [Level 2] Exploring 23 word(s)...
> [Level 3] Exploring 232 word(s)...
> [Level 4] Exploring 1222 word(s)...
> 
> Path found! Reconstructing from target to start using parent map.
> 
> Shortest Path Found (5 steps):
> cold -> wold -> word -> ward -> warm
> ```
