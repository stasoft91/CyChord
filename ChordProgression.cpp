#include "ChordProgression.hpp"
#include <stdexcept>
#include <sstream>

// Helper to convert a string or chord into a Chord
static Chord asChord(const std::string& c) {
    return Chord(c);
}
static Chord asChord(const Chord& c) {
    return c;
}

ChordProgression::ChordProgression() {}

ChordProgression::ChordProgression(const std::string& singleChord) {
    m_chords.push_back(asChord(singleChord));
}

ChordProgression::ChordProgression(const Chord& singleChord) {
    m_chords.push_back(singleChord);
}

ChordProgression::ChordProgression(const std::vector<std::string>& chordNames) {
    m_chords.reserve(chordNames.size());
    for (auto &cn : chordNames) {
        m_chords.push_back(asChord(cn));
    }
}

ChordProgression::ChordProgression(const std::vector<Chord>& chords) {
    m_chords = chords;
}

void ChordProgression::append(const Chord& chord) {
    m_chords.push_back(chord);
}

void ChordProgression::insert(std::size_t index, const Chord& chord) {
    if (index > m_chords.size()) {
        throw std::runtime_error("Index out of range in ChordProgression::insert.");
    }
    m_chords.insert(m_chords.begin() + index, chord);
}

Chord ChordProgression::pop(std::size_t index) {
    if (m_chords.empty()) {
        throw std::runtime_error("Cannot pop from empty ChordProgression.");
    }
    if (index == static_cast<std::size_t>(-1)) {
        index = m_chords.size() - 1; // last
    }
    if (index >= m_chords.size()) {
        throw std::runtime_error("Index out of range in ChordProgression::pop.");
    }
    Chord removed = m_chords[index];
    m_chords.erase(m_chords.begin() + index);
    return removed;
}

void ChordProgression::transpose(int semitones) {
    for (auto &c : m_chords) {
        c.transpose(semitones);
    }
}

std::vector<Chord>& ChordProgression::chords() {
    return m_chords;
}

const std::vector<Chord>& ChordProgression::chords() const {
    return m_chords;
}

Chord& ChordProgression::operator[](std::size_t index) {
    if (index >= m_chords.size()) {
        throw std::runtime_error("Index out of range in ChordProgression::operator[]");
    }
    return m_chords[index];
}

const Chord& ChordProgression::operator[](std::size_t index) const {
    if (index >= m_chords.size()) {
        throw std::runtime_error("Index out of range in ChordProgression::operator[] const");
    }
    return m_chords[index];
}

ChordProgression& ChordProgression::operator+=(const ChordProgression& other) {
    m_chords.insert(m_chords.end(), other.m_chords.begin(), other.m_chords.end());
    return *this;
}

bool ChordProgression::operator==(const ChordProgression& other) const {
    if (m_chords.size() != other.m_chords.size()) {
        return false;
    }
    for (size_t i = 0; i < m_chords.size(); ++i) {
        if (m_chords[i] != other.m_chords[i]) {
            return false;
        }
    }
    return true;
}

std::string ChordProgression::toString() const {
    std::ostringstream oss;
    for (size_t i = 0; i < m_chords.size(); ++i) {
        oss << m_chords[i].chordName();
        if (i + 1 < m_chords.size()) {
            oss << " | ";
        }
    }
    return oss.str();
}
