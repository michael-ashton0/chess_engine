# Progress as of 12/14 #

* [ ] Board representation
    * [x] FEN compatibility
        * [x] piece placement
        * [x] side-to-move
        * [x] castling rights
        * [x] en-passant target square
        * [ ] halfmove clock / fullmove number
        * [ ] FEN export matches internal state
    * [ ] Make/Unmake move
        * [x] Maintain piece bitboards correctly on capture
        * [x] Maintain universal occupancy / side occupancy
        * [x] Toggle side-to-move
        * [ ] Track captured piece type for unmake
        * [ ] Restore previous castling rights on unmake
        * [ ] Restore previous en-passant square on unmake
        * [ ] Handle promotions in make/unmake
        * [ ] Handle rook move during castling in make/unmake
        * [ ] Handle en-passant capture removal in make/unmake
        * [ ] Increment/decrement halfmove clock + fullmove (optional)

* [ ] Piece move generation
    * [x] Queen, Rook, Bishop
        * [x] Verify sliding attacks stop at blockers
        * [x] Distinguish quiet vs capture moves cleanly
    * [ ] King, Knight
        * [x] King adjacency moves exclude own pieces
        * [ ] King moves must not move into check (later)
    * [ ] Pawn
        * [x] Double pushes
            * [x]  Only from starting rank
            * [x] Both squares must be empty
            * [ ] Set en-passant target square on double push
        * [x] Captures
            * [x] if enemy piece exists
        * [x] Single pushes
            * [x] Blocked forward square prevents move
        * [ ] Promotions
            * [x] Quiet promotions (push into last rank)
            * [x] Capture promotions (capture into last rank)
            * [x] Generate N/B/R/Q promotion options
            * [ ] Make/unmake promotion piece changes
        * [ ] En passant
            * [ ] Generate EP capture only if EP square set
            * [ ] EP capture only if correct pawn is adjacent
            * [ ] Make: remove the captured pawn from behind EP square
            * [ ] Unmake: restore captured pawn correctly
            * [ ] EP capture legality: cannot expose own king to check (later)
    * [ ] Castling
        * [ ] Track castling rights
            * [ ] King move clears both rights for that side
            * [ ] Rook move clears that rook-side right
            * [ ] Loss of rook clears that right
        * [ ] Generate pseudolegal castles 
            * [ ] Squares between king/rook must be empty
        * [ ] Castling legality checks (legal move gen later)
            * [ ] King not currently in check
            * [ ] King does not pass through check
            * [ ] Destination square not in check
        * [ ] Make/unmake castling (king + rook movement)

* [ ] Store move list
    * [ ] Define `Move` encoding/struct
        * [x] from / to
        * [x] moved piece (optional but useful)
        * [ ] captured piece (or captured piece type)
        * [x] flags: capture, promotion, en-passant, castling, double-push
        * [x] promotion piece type
    * [ ] Move container strategy
        * [x] vector reserve sizing / per-node reuse (perf later)
        * [ ] clear vs reuse per ply
    * [ ] Generate pseudo-legal move list for side to move
        * [ ] separate quiet vs captures (does this matter?)
        * [ ] deterministic ordering for debugging (if needed)

* [ ] Player
    * [ ] Random / first-legal move player
    * [ ] Perft driver
        * [ ] Perft(1..N) from startpos matches known values
        * [ ] Perft from a few tricky FENs
        * [ ] Divide output for debugging move-gen mismatches
    * [ ] Legal move filtering
        * [ ] Make move, verify king not in check, unmake
        * [ ] OR implement check/pin logic and generate legal directly (would have to come after)
    * [ ] Basic search
        * [ ] Minimax + alpha-beta
        * [ ] Simple eval (material, maybe positional to follow)
        * [ ] Iterative deepening
        * [ ] Transposition table

