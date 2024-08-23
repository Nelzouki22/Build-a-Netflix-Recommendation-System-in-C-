#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

// Struct to hold movie data
struct Movie {
    std::string title;
    std::string description;
};

// Load data from CSV
std::vector<Movie> load_data(const std::string& filename) {
    std::vector<Movie> movies;
    std::ifstream file(filename);
    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string title, description;
        std::getline(ss, title, ',');
        std::getline(ss, description, ',');
        movies.push_back({ title, description });
    }
    return movies;
}

// Extract keywords
std::unordered_map<std::string, int> extract_keywords(const std::string& text) {
    std::unordered_map<std::string, int> keywords;
    std::stringstream ss(text);
    std::string word;
    while (ss >> word) keywords[word]++;
    return keywords;
}

// Calculate cosine similarity
double cosine_similarity(const std::unordered_map<std::string, int>& vec1, const std::unordered_map<std::string, int>& vec2) {
    double dot_product = 0, magnitude1 = 0, magnitude2 = 0;
    for (const auto& pair : vec1) {
        if (vec2.find(pair.first) != vec2.end()) dot_product += pair.second * vec2.at(pair.first);
        magnitude1 += pair.second * pair.second;
    }
    for (const auto& pair : vec2) magnitude2 += pair.second * pair.second;
    magnitude1 = std::sqrt(magnitude1);
    magnitude2 = std::sqrt(magnitude2);
    return dot_product / (magnitude1 * magnitude2);
}

// Recommend movies
std::vector<std::string> recommend_movies(const std::string& title, const std::vector<Movie>& movies) {
    std::vector<std::pair<double, std::string>> scores;
    std::unordered_map<std::string, int> target_keywords = extract_keywords(title);
    for (const auto& movie : movies) {
        if (movie.title != title) {
            std::unordered_map<std::string, int> movie_keywords = extract_keywords(movie.description);
            double score = cosine_similarity(target_keywords, movie_keywords);
            scores.emplace_back(score, movie.title);
        }
    }
    std::sort(scores.begin(), scores.end(), std::greater<>());
    std::vector<std::string> recommendations;
    for (const auto& score : scores) recommendations.push_back(score.second);
    return recommendations;
}

int main() {
    std::string filename = "netflix_titles.csv";
    std::vector<Movie> movies = load_data(filename);
    std::string input_title = "Some Movie";
    std::vector<std::string> recommendations = recommend_movies(input_title, movies);
    std::cout << "Recommendations:\n";
    for (const auto& rec : recommendations) std::cout << rec << std::endl;
    return 0;
}
