#include "Utils.hpp"
#include "Constants.hpp"
#include <stdexcept>

/**
 * Convert a note (e.g. "C", "F#", "Bb") to its semitone integer value (0..11).
 */
int noteToVal(const std::string& note) {
    auto it = NOTE_VAL_DICT.find(note);
    if (it == NOTE_VAL_DICT.end()) {
        throw std::runtime_error("Unknown note: " + note);
    }
    return it->second;
}

/**
 * Convert an integer value to a note name, according to the scale chosen by scaleRoot.
 * e.g. valToNote(0,"C") -> "C", valToNote(1,"A") -> "A#" or "Bb", depending on dictionary.
 */
std::string valToNote(int val, const std::string& scaleRoot) {
    val = val % 12;
    auto scaleIt = SCALE_VAL_DICT.find(scaleRoot);
    if (scaleIt == SCALE_VAL_DICT.end()) {
        // fallback to "C" scale if scaleRoot not found
        return FLATTED_SCALE.at(val);
    }
    // use the chosen scale's mapping
    const auto& chosenScale = scaleIt->second;
    return chosenScale.at(val);
}

/**
 * Transpose a given note by some number of semitones, returning the new note name.
 */
std::string transposeNote(const std::string& note, int semitones, const std::string& scale) {
    int val = noteToVal(note);
    val += semitones;
    return valToNote(val, scale);
}

std::string displayAppended(const std::vector<std::string>& appended) {
    // TODO: In Python code, appended was not fully implemented, so for now just join them
    // In more advanced logic, you might parse them like "(add9)" or "add9".
    if (appended.empty()) {
        return std::string();
    }
    // Example minimal approach: "(" + appended[0] + appended[1] + ... + ")"
    // or just a naive join with nothing in-between
    // We'll do a minimal approach:
    std::string result;
    for (const auto& app : appended) {
        result += app;
    }
    return result;
}

std::string displayOn(const std::string& onNote) {
    if (!onNote.empty()) {
        return "/" + onNote;
    }
    return std::string();
}
