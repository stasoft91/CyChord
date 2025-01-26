#include "FindChords.hpp"
#include "Utils.hpp"
#include "QualityManager.hpp"
#include <stdexcept>
#include <algorithm>
#include "Constants.hpp"  

/**
 * Given a list of notes (e.g. {"C","Eb","G"}), find all chord(s) that match.
 * The Python approach:
 * 1) The root is first note
 * 2) Rotate the list in all possible ways, each time computing intervals
 * 3) Check if those intervals match a known Quality
 * 4) If yes, produce "RootQuality[/OriginalRootIfDifferent]"
 */
static std::vector<std::vector<std::string>> getAllRotatedNotes(const std::vector<std::string>& notes) {
    std::vector<std::vector<std::string>> result;
    result.reserve(notes.size());
    for (size_t i = 0; i < notes.size(); ++i) {
        std::vector<std::string> rotated;
        rotated.insert(rotated.end(), notes.begin() + i, notes.end());
        rotated.insert(rotated.end(), notes.begin(), notes.begin() + i);
        result.push_back(rotated);
    }
    return result;
}

static std::vector<int> notesToPositions(const std::vector<std::string>& notes, const std::string& root) {
    // Mimic Python logic:
    // - The first note is root
    // - Then for each subsequent note, if note val < current, add 12
    // - Then store offset from root
    int rootPos = noteToVal(root);
    int currentPos = rootPos;
    std::vector<int> positions;
    positions.reserve(notes.size());
    for (auto &note : notes) {
        int notePos = noteToVal(note);
        while (notePos < currentPos) {
            notePos += 12;
        }
        positions.push_back(notePos - rootPos);
        currentPos = notePos;
    }
    return positions;
}

std::vector<Chord> findChordsFromNotes(const std::vector<std::string>& notes) {
    if (notes.empty()) {
        throw std::runtime_error("Please specify notes which form a chord.");
    }
    std::string originalRoot = notes[0];

    // We'll gather (rotatedRoot, intervals)
    // Then see if QualityManager can find a match
    auto rotations = getAllRotatedNotes(notes);

    std::vector<Chord> results;
    for (auto &rot : rotations) {
        std::string rotRoot = rot[0];
        auto positions = notesToPositions(rot, rotRoot);
        // find a quality
        auto q = QualityManager::Instance().findQualityFromComponents(positions);
        if (q) {
            // We have a chord. If rotRoot == originalRoot, it's root,
            // else it's e.g. "rotRootQuality/originalRoot"
            // We'll build a chord string
            if (rotRoot == originalRoot) {
                results.push_back( Chord(rotRoot + q->getQualityName()) );
            } else {
                // e.g. "C#m7/G#"
                std::string chordExpr = rotRoot + q->getQualityName() + "/" + originalRoot;
                results.push_back( Chord(chordExpr) );
            }
        }
    }
    return results;
}


/**
 * Overload to handle MIDI notes. We'll:
 * 1) Sort the MIDI note numbers ascending
 * 2) Convert each to semitone (note % 12)
 * 3) Map that semitone to a default note name (e.g. 1 -> "Db")
 * 4) Forward the string-vector to the existing findChordsFromNotes(...)
 */
std::vector<Chord> findChordsFromNotes(const std::vector<int>& midiNotes)
{
    if (midiNotes.empty()) {
        throw std::runtime_error("Please specify at least one MIDI note.");
    }

    // 1) Sort
    std::vector<int> sortedNotes = midiNotes;
    std::unique(sortedNotes.begin(), sortedNotes.end());
    std::sort(sortedNotes.begin(), sortedNotes.end());

    // 2) Convert to semitone & pick a default note name for each
    std::vector<std::string> noteNames;
    noteNames.reserve(sortedNotes.size());

    for (int midi : sortedNotes) {
        int semitone = midi % 12;  // wrap to 0..11
        auto it = VAL_NOTE_DICT.find(semitone);
        if (it == VAL_NOTE_DICT.end() || it->second.empty()) {
            throw std::runtime_error("No known note name for semitone = " + std::to_string(semitone));
        }
        // pick the first name from e.g. { "Db", "C#" }
        const std::string& chosenName = it->second.front();
        noteNames.push_back(chosenName);
    }

    // 3) Pass them to the existing findChordsFromNotes with strings
    return findChordsFromNotes(noteNames);
}