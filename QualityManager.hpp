#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Quality.hpp"

/**
 * Manages a dictionary of chord qualities (singleton).
 */

class QualityManager {
public:
    // Singleton accessor
    static QualityManager& Instance();

    // Load default chord qualities from the global DEFAULT_QUALITIES
    void loadDefaultQualities();

    // Return a (dynamically created) Quality with optional inversion
    std::shared_ptr<Quality> getQuality(const std::string& name, int inversion = 0);

    // Set or add a custom quality
    void setQuality(const std::string& name, const std::vector<int>& components);

    // Find a quality whose intervals match exactly
    std::shared_ptr<Quality> findQualityFromComponents(const std::vector<int>& components);

private:
    QualityManager(); // private constructor
    QualityManager(const QualityManager&) = delete;
    QualityManager& operator=(const QualityManager&) = delete;

    std::map<std::string, std::shared_ptr<Quality>> m_qualities;
};
