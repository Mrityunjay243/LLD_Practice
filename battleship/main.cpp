#include <bits/stdc++.h>
using namespace std; 

struct Coordinate{
    int r{}, c{}; 
    Coordinate() = default; 
    Coordinate(int row, int col): r(row), c(col) {}
};

static inline string coordToString(const Coordinate& p, int nCols){
    char row = char('A' + p.r); 
    return string(1, row) + to_string(p.c + 1); 
}

static inline bool parseCoord(const string& s, int N, Coordinate& out){
    if (s.size() < 2 || s.size() > 3) return false; 
    char L = toupper(s[0]); 
    if (L<'A' || L>='A' + N) return false; 
    string num = s.substr(1);
    if (!all_of(num.begin(), num.end(), ::isdigit)) return false; 
    int col = stoi(num); 

    if (col<1 || col>N) return false; 
    out = Coordinate(L-'A', col-1); 
    return true; 
}

enum class AttackResult{ Invalid, Repeat, Miss, Hit, Sunk, Win}; 

class Ship{
public:
    Ship(string name, int length): name_{ move(name) }, length_{ move(length) }{ }
    
    const string& name() const { return name_; }
    int length() const { return length_; }
    bool isSunk() const { return hits_==length_; }
    
    void occupy(const vector<Coordinate>& cells) { cells_ = cells; }
    bool occupies(const Coordinate& p) const {
        return any_of(cells_.begin(), cells_.end(), [&](const Coordinate& q){
            return q.r == p.r && q.c == p.c; 
        });
    }

    void registerHit() { hits_++; }
    
private: 
    string name_; 
    int length_{}; 
    int hits_{0};
    vector<Coordinate> cells_;      
};

class Board{
public:
    struct Cell{
        int shipIdx = -1; // empty otherwise index into ships
        bool hit = false; 
    };

    explicit Board(int N=10): 
    N_{ N }, grid_(N, vector<Cell>(N)){}
    
    int size() const { return N_; }

    // Place who fleet randomly, no overlaps, and no touching
    void placeFleetRandom(unsigned seed = random_device{}()){
        static const vector<pair<string, int>> kFleet = {
            {"Carrier", 5}, {"Battleship", 4}, {"Cruiser", 3}, 
            {"Submarine", 2}, {"Destroyer", 1}
        }; 

        ships_.clear(); 
        for (auto& s: kFleet) ships_.emplace_back(s.first, s.second); 

        mt19937 rng(seed); 
        uniform_int_distribution<int> coin(0,1); 

        for (int i=0; i<(int)ships_.size(); i++){
            bool placed = false; 
            for (int tries=0; tries<10000 && !placed; tries++){
                bool horiz = coin(rng); 
                int len = ships_[i].length(); 
                int maxr = horiz ? N_-1: N_-len; 
                int maxc = horiz ? N_-len: N_-1; 
                uniform_int_distribution<int> rdist(0, maxr), cdist(0, maxc); 
                int r = rdist(rng), c = cdist(rng); 
                if (canPlace(r, c, len, horiz)){
                    vector<Coordinate> occ; 
                    for(int k=0; k<len; ++k){
                        int rr = r + (horiz?0:k);
                        int cc = c + (horiz?k:0); 
                        grid_[rr][cc].shipIdx = i; 
                        occ.emplace_back(rr, cc); 
                    }

                    ships_[i].occupy(occ); 
                    placed = true; 
                }
            }
            if (!placed) throw runtime_error("Failed to place fleet"); 
        }
    }

    void printFog(ostream& os) const { 
        os << " " ; 
        for (int c=0; c<N_; c++){
            os << setw(2) << (c+1) << ' '; 
        }

        os << "\n"; 

        for (int r=0; r<N_; r++){
            os << ' ' << char('A'+r) << ' '; 
            for (int c=0; c<N_; c++){
                char ch = '.'; 
                if (grid_[r][c].hit) ch = (grid_[r][c].shipIdx==-1 ? 'o': 'X'); 
                os << ' ' << ch << ' '; 
            }
            os << "\n";
        }
    }

    // Attack this board
    AttackResult attack(const Coordinate& p, string& sunkName){
        sunkName.clear(); 
        if (!in(p)) return AttackResult::Invalid; 
        Cell& cell = grid_[p.r][p.c]; 
        if (cell.hit) return AttackResult::Repeat; 
        cell.hit = true; 

        if (cell.shipIdx==-1) return AttackResult::Miss; 

        Ship& ship = ships_[cell.shipIdx]; 
        ship.registerHit(); 

        if (ship.isSunk()){
            sunkName = ship.name(); 
            if (allSunk()) return AttackResult::Win; 
            return AttackResult::Sunk; 
        }

        return AttackResult::Hit; 
    }

    void printReveal(ostream& os) const {
        os << " "; 
        for (int c=0; c<N_; c++){
            os << setw(2) << (c+1) << ' '; 
            os << "\n"; 
            for (int r=0; r<N_; r++){
                os << ' ' << char('A'+r) << ' '; 
                for (int c=0; c<N_; c++){
                    char ch = grid_[r][c].shipIdx==-1? '.' : '#'; 
                    if (grid_[r][c].hit) ch = (grid_[r][c].shipIdx==-1? 'o' : 'X'); 
                    os << ' ' << ch << ' '; 
                }
                os << "\n"; 
            } 
        }
    }

private:
    int N_; 
    vector<vector<Cell>> grid_;
    vector<Ship> ships_; 

    bool in(const Coordinate& p) const {
        return p.r>=0 && p.r<N_ && p.c>=0 && p.c<N_; 
    }

    bool canPlace(int r, int c, int len, bool horiz) const {
        for (int k=0; k<len; k++){
            int rr = r + (horiz?0:k);
            int cc = c + (horiz?k:0); 
            if (in({rr, cc}) && grid_[rr][cc].shipIdx!=-1) return false; 

            for (int dr=-1; dr<=1; dr++){
                for (int dc=-1; dc<=1; dc++){
                    int nr = rr+dr; 
                    int nc = cc+dc; 
                    if (in({nr, nc}) && grid_[nr][nc].shipIdx!=-1) return false; 
                }
            }
        }
        return true; 
    }

    bool allSunk() const {
        for (int r=0; r<N_; r++){
            for (int c=0; c<N_; c++){
                if (grid_[r][c].shipIdx!=-1 && !grid_[r][c].hit) return false; 
            }
        }

        return true; 
    }
}; 

class Player{
public:
    explicit Player(string name, int boardSize=10):
    name_{ move(name) }, board_{ boardSize } {}

    const string& name() const { return name_; }
    Board& board() { return board_; }
    const Board& board() const { return board_; }

private:    
    string name_; 
    Board board_; 
};

class Game{
public: 
    Game(int boardSize=10):
    human_("You", boardSize), computer_("Computer", boardSize) {}

    void setup(unsigned seed = random_device{}()){
        computer_.board().placeFleetRandom(seed); 
    }

    void run() {
        cout << "=== Battleship (OOP Single Player vs Computer Fleet) ===\n"; 
        cout << "Enter coordinates. Symbols: '.' unknown, 'o' miss, 'X' hit\n\n"; 

        int shots = 0; 
        while (true){
            computer_.board().printFog(cout); 
            cout << "\nFire at (e.g., A5): "; 

            string s; 
            if (!(cin>>s)) break; 

            Coordinate p; 
            if (!parseCoord(s, computer_.board().size(), p)){
                cout << "Invalid input. Try something else\n\n"; 
                continue;
            }

            string sunk; 
            AttackResult res = computer_.board().attack(p, sunk); 
            switch(res){
                case AttackResult::Invalid:
                    cout << "Out of bounds.\n\n"; 
                    break;
                case AttackResult::Repeat:
                    cout << "You already fired here, try some other place.\n\n"; 
                    break;
                case AttackResult::Miss:
                    shots++; 
                    cout << "Miss.\n\n"; 
                    break;
                case AttackResult::Hit:
                    shots++;
                    cout << "Hit!\n\n"; 
                    break; 
                case AttackResult::Sunk:
                    shots++; 
                    cout << "You sank the enemy " << sunk << "!\n\n"; 
                    break;
                case AttackResult::Win:
                    shots++; 
                    cout << "\nAll the enemy ships have sunk. You win bro in " << shots << " shots.\n"; 
                    return;   
            }
        }
    }

    void revealComputerBoard() const {
        computer_.board().printReveal(cout); 
    }

private:
    Player human_, computer_; 
};

int main(){
    //ios::sync_with_stdio(false); 
    //cin.tie(nullptr); 

    Game game(10); 
    game.setup(); 

    game.run(); 
    return 0; 
}