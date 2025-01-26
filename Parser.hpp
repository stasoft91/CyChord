#pragma once

#include <string>
#include <vector>

/**
 * Functions to parse a chord string (e.g. "F#m7-5/A") into structured components.
 */

struct ChordTokens {
    std::string root;
    std::string qualityName;
    std::vector<std::string> appended;
    std::string slashNote;  // e.g. "G" for slash chord
    int inversion;
};

/**
 * Parse a chord name into tokens: root, quality, appended, slash, etc.
 */
ChordTokens parseChord(const std::string& chordExpression);
