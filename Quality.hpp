#pragma once

#include <string>
#include <vector>

/**
 * Represents a chord quality, e.g. "maj7", with intervals from the root.
 */

class Quality {
public:
    Quality(const std::string& name, const std::vector<int>& components);

    // Accessors
    std::string getQualityName() const;
    std::vector<int> getComponents(const std::string& root, bool visible = false) const;

    // For slash chords: modifies the internal intervals so that the slash note is "lowest".
    void appendOnChord(const std::string& onChord, const std::string& root);

    // Operators
    bool operator==(const Quality& other) const;
    bool operator!=(const Quality& other) const { return !(*this == other); }

private:
    std::string m_qualityName;
    std::vector<int> m_components; // intervals from root

    // Helpers
    int rootVal(const std::string& root) const;
};
