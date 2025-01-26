#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Quality.hpp"

/**
 * Represents a chord. e.g. "F#m7-5/A".
 */

class Chord {
public:
    // Constructor from a chord string
    explicit Chord(const std::string& chordName);

    // Alternate constructor from python code: from_note_index
    static Chord fromNoteIndex(int note,
                               const std::string& quality,
                               const std::string& scale,
                               bool diatonic = false,
                               int chromatic = 0);

    // Inspectors
    std::string chordName() const;  // full chord name
    std::string root() const;
    std::shared_ptr<Quality> quality() const;
    std::vector<std::string> appended() const;
    std::string on() const;

    // Info
    std::string info() const;

    // Transpose chord
    void transpose(int semitones, const std::string& scale = "C");

    // Return numeric or string components
    std::vector<int> components(bool visible = false) const;

    /**
     * Return the chord's pitches, e.g. ["C4","E4","G4"] if root_pitch=4
     * (though we store pitch as the "octave" notion).
     */
    std::vector<std::string> componentsWithPitch(int rootPitch) const;

    // Operators
    bool operator==(const Chord& other) const;
    bool operator!=(const Chord& other) const { return !(*this == other); }

private:
    // data
    std::string m_chordName;             // e.g. "F#m7-5/A"
    std::string m_root;                  // e.g. "F#"
    std::shared_ptr<Quality> m_quality;  // e.g. "m7-5"
    std::vector<std::string> m_appended; // appended notes
    std::string m_on;                    // slash note

private:
    // Reconstruct m_chordName from pieces
    void reconfigureChord();
    // Adjust the Quality for slash chord
    void applyOnChord();
};
