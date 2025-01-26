#pragma once

#include <vector>
#include <string>
#include "Chord.hpp"

/**
 * Represents a progression of Chords.
 */

class ChordProgression {
public:
    // Constructors
    ChordProgression(); // empty
    explicit ChordProgression(const std::string& singleChord);
    explicit ChordProgression(const Chord& singleChord);
    explicit ChordProgression(const std::vector<std::string>& chordNames);
    explicit ChordProgression(const std::vector<Chord>& chords);

    // Adding / removing
    void append(const Chord& chord);
    void insert(std::size_t index, const Chord& chord);
    Chord pop(std::size_t index = static_cast<std::size_t>(-1));

    // Transpose
    void transpose(int semitones);

    // Access
    std::vector<Chord>& chords();
    const std::vector<Chord>& chords() const;

    // Operators
    Chord& operator[](std::size_t index);
    const Chord& operator[](std::size_t index) const;

    ChordProgression& operator+=(const ChordProgression& other);
    bool operator==(const ChordProgression& other) const;
    bool operator!=(const ChordProgression& other) const { return !(*this == other); }

    // Info
    std::string toString() const;

private:
    std::vector<Chord> m_chords;
};
