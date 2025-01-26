#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

/**
 * Global music-theory constants and dictionaries.
 *
 * We store these as extern in the header and define them
 * in the corresponding .cpp so we don't duplicate.
 */

// Mapping of note names to semitone values
extern const std::unordered_map<std::string, int> NOTE_VAL_DICT;

// Mapping from semitone values (0-11) to possible note names
extern const std::map<int, std::vector<std::string>> VAL_NOTE_DICT;

// Mapping from semitone values to "sharped" note names (for display)
extern const std::unordered_map<int, std::string> SHARPED_SCALE;

// Mapping from semitone values to "flatted" note names (for display)
extern const std::unordered_map<int, std::string> FLATTED_SCALE;

/**
 * Mapping from a "key root" (e.g. "C", "Db", "G#") to which scale (sharp or flat)
 * to use for display.
 */
extern const std::unordered_map<std::string, std::unordered_map<int, std::string>> SCALE_VAL_DICT;

/**
 * E.g. "maj" => Ionian, "min" => Aeolian. Each vector is a scale pattern in semitones.
 */
extern const std::unordered_map<std::string, std::vector<int>> RELATIVE_KEY_DICT;

/**
 * Default chord qualities:
 * e.g. { "m7", {0,3,7,10} }, etc.
 * Each entry is (qualityName, vectorOfIntervals).
 */
extern const std::vector<std::pair<std::string, std::vector<int>>> DEFAULT_QUALITIES;

