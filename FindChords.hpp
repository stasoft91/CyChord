#pragma once

#include <string>
#include <vector>
#include "Chord.hpp"

/**
 * Functions to discover possible Chords from a given set of note names.
 */

std::vector<Chord> findChordsFromNotes(const std::vector<std::string>& notes);
std::vector<Chord> findChordsFromNotes(const std::vector<int>& midiNotes);