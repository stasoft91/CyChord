PyChord port to C++ 

CyChord is a C++ library to handle musical chords. Parse chords, name chords, inverse nodes, have fun!

Improved it a bit so it can handle more cases, like A7sus or Bbm7#5/Db

Original [PyChord](https://github.com/yuma-m/pychord)


Usage:

```cpp
#include <iostream>
#include "Chord.hpp"
#include "ChordProgression.hpp"
#include "FindChords.hpp"

int main() {
    // 1. Construct a chord from a name
    Chord c("F#m7-5/A");
    std::cout << "Chord c: " << c.chordName() << std::endl;
    std::cout << c.info() << std::endl;

    // 2. Transpose it
    c.transpose(2);
    std::cout << "Transposed up 2 semitones: " << c.chordName() << std::endl;

    // 3. Make a chord progression
    ChordProgression prog({"C", "G/B", "Am", "F"});
    std::cout << "Original progression: " << prog.toString() << std::endl;
    prog.transpose(-2);
    std::cout << "Transposed down 2: " << prog.toString() << std::endl;

    // 4. Find chords from notes
    std::vector<std::string> notes = {"C", "E", "G"};
    auto found = findChordsFromNotes(notes);
    std::cout << "Chords found for C-E-G: \n";
    for (auto &ch : found) {
        std::cout << " - " << ch.chordName() << std::endl;
    }

    // 5. fromNoteIndex example
    // Build III chord in F major => "Am" (since F major scale is F G A Bb C D E)
    auto chordIII = Chord::fromNoteIndex(3, "", "Fmaj");
    std::cout << "III chord in F major: " << chordIII.chordName() << std::endl;

    return 0;
}
```
